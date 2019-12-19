#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <signal.h>
#include <string.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <unistd.h>
#include "socket.h"
#include "msg.h"
#include "console.h"
#include "../protocol/proto-server.h"
#include <pthread.h>

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"
#define MAX_CLIENTS 10

#define NO_SOCKET -1

#define SERVER_NAME "server"

int listen_sock;
socket_peer connection_list[MAX_CLIENTS];
pthread_t worker_thread;

void shutdown_properly(int code);

int start_listen_socket(int *listen_sock)
{

  *listen_sock = socket(AF_INET, SOCK_STREAM, 0);
  if (*listen_sock < 0) {
    perror("socket");
    return -1;
  }
 
  int reuse = 1;
  if (setsockopt(*listen_sock, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) != 0) {
    perror("setsockopt");
    return -1;
  }
  
  struct sockaddr_in my_addr;
  memset(&my_addr, 0, sizeof(my_addr));
  my_addr.sin_family = AF_INET;
  my_addr.sin_addr.s_addr = inet_addr(SERVER_IPV4_ADDR);
  my_addr.sin_port = htons(SERVER_LISTEN_PORT);
 
  if (bind(*listen_sock, (struct sockaddr*)&my_addr, sizeof(struct sockaddr)) != 0) {
    perror("bind");
    return -1;
  }

  if (listen(*listen_sock, 10) != 0) {
    perror("listen");
    return -1;
  }
  printf("[ACMS] Running on port %d.\n", (int)SERVER_LISTEN_PORT);
 
  return 0;
}

void shutdown_properly(int code)
{
  int i;
  close(listen_sock);
  for (i = 0; i < MAX_CLIENTS; ++i)
    if (connection_list[i].socket != NO_SOCKET)
      close(connection_list[i].socket);
    pthread_join(worker_thread, NULL);
  printf("[ACMS] Shutting down.\n");
  exit(code);
}

int build_fd_sets(fd_set *read_fds, fd_set *write_fds, fd_set *except_fds)
{
  int i;
  FD_ZERO(read_fds);
  FD_SET(STDIN_FILENO, read_fds);
  FD_SET(listen_sock, read_fds);
  for (i = 0; i < MAX_CLIENTS; ++i)
    if (connection_list[i].socket != NO_SOCKET)
      FD_SET(connection_list[i].socket, read_fds);
  FD_ZERO(write_fds);
  for (i = 0; i < MAX_CLIENTS; ++i)
    if (connection_list[i].socket != NO_SOCKET && connection_list[i].send_buffer.current > 0)
      FD_SET(connection_list[i].socket, write_fds);
  FD_ZERO(except_fds);
  FD_SET(STDIN_FILENO, except_fds);
  FD_SET(listen_sock, except_fds);
  for (i = 0; i < MAX_CLIENTS; ++i)
    if (connection_list[i].socket != NO_SOCKET)
      FD_SET(connection_list[i].socket, except_fds);
  return 0;
}  

int handle_new_connection()
{
  struct sockaddr_in client_addr;
  memset(&client_addr, 0, sizeof(client_addr));
  socklen_t client_len = sizeof(client_addr);
  int new_client_sock = accept(listen_sock, (struct sockaddr *)&client_addr, &client_len);
  if (new_client_sock < 0) {
    perror("accept()");
    return -1;
  }
  char client_ipv4_str[INET_ADDRSTRLEN];
  inet_ntop(AF_INET, &client_addr.sin_addr, client_ipv4_str, INET_ADDRSTRLEN);
  printf("[ACMS] %s:%d. connected\n", client_ipv4_str, client_addr.sin_port);
  for (int i = 0; i < MAX_CLIENTS; ++i) {
    if (connection_list[i].socket == NO_SOCKET) {
      connection_list[i].socket = new_client_sock;
      connection_list[i].address = client_addr;
      connection_list[i].current_sending_byte   = -1;
      connection_list[i].current_receiving_byte = 0;
      return 0;
    }
  }
  printf("[ACMS] Connection overflow, enough...: %s:%d.\n", client_ipv4_str, client_addr.sin_port);
  close(new_client_sock);
  return -1;
}

int close_client_connection(socket_peer *client)
{
  printf("[ACMS] Closing connection with %s.\n", get_ip_str(client));
  close(client->socket);
  client->socket = NO_SOCKET;
  mq_flush(&client->send_buffer);
  client->current_sending_byte   = -1;
  client->current_receiving_byte = 0;
}

int broadcast_pck(g_msg *pck) {
    for (int i = 0; i < MAX_CLIENTS; ++i) {
        if (connection_list[i].socket != NO_SOCKET) {
            if (mq_enqueue(&connection_list[i].send_buffer, pck) != 0) {
                //printf("Send buffer was overflowed, we lost this message!\n");
                continue;
            }
        }
    }
    return 0;
}

int stdin_handler()
{
    char read_buffer[DATA_MAXSIZE]; // buffer for stdin
    if (read_console(read_buffer, DATA_MAXSIZE) != 0) return -1;
//    g_msg new_message;
//    init_handlers();
//    p_auth p = {"Aziz", "aj", "123"};
//    prepare_packet(0, SERVER_NAME, &new_message, &p);
//    log_msg(&new_message);
//    broadcast_pck(&new_message);
    return 0;
}

void worker(void* args) {
    int c = 0;
    while(1) {
        g_msg msg;
        test_pck data = { 3};
        prepare_packet(2, "server-001", &msg, &data, sizeof(data));
        broadcast_pck(&msg);
        for (int i = 0; i < MAX_CLIENTS; ++i) {
            if(connection_list[i].socket != NO_SOCKET)
                if (sp_send(&connection_list[i]) != 0) {
                    close_client_connection(&connection_list[i]);
                    continue;
                }
        }
        sleep(2);
    }
}

 
int main(int argc, char **argv)
{
    printf("ACMS Server v.0.0.1 startup\n");
    if(start_listen_socket(&listen_sock) != 0)
        exit(EXIT_FAILURE);
    int flag = fcntl(STDIN_FILENO, F_GETFL, 0);
    flag |= O_NONBLOCK;
    fcntl(STDIN_FILENO, F_SETFL, flag);
    for (int i = 0; i < MAX_CLIENTS; ++i) {
        connection_list[i].socket = NO_SOCKET;
          sp_create(&connection_list[i]);
    }
    fd_set read_fds;
    fd_set write_fds;
    fd_set except_fds;
    int high_sock = listen_sock;
    //pthread_create(&worker_thread, NULL, worker, &connection_list);
    while (1) {
        build_fd_sets(&read_fds, &write_fds, &except_fds);
        high_sock = listen_sock;
        for(int i = 0; i < MAX_CLIENTS; ++i) {
            if(connection_list[i].socket > high_sock)
                high_sock = connection_list[i].socket;
        }
        int activity = select(high_sock + 1, &read_fds, &write_fds, &except_fds, NULL);
        switch (activity) {
          case -1:
            perror("select()");
            shutdown_properly(EXIT_FAILURE);
          case 0:
            printf("select() returns 0.\n");
            shutdown_properly(EXIT_FAILURE);
          default:
            if (FD_ISSET(STDIN_FILENO, &read_fds)) {
              if (stdin_handler() != 0)
                shutdown_properly(EXIT_FAILURE);
            }
            if (FD_ISSET(listen_sock, &read_fds)) {
              handle_new_connection();
            }
            if (FD_ISSET(STDIN_FILENO, &except_fds)) {
              printf("except_fds for stdin.\n");
              shutdown_properly(EXIT_FAILURE);
            }
            if (FD_ISSET(listen_sock, &except_fds)) {
              printf("Exception listen socket fd.\n");
              shutdown_properly(EXIT_FAILURE);
            }
            for (int i = 0; i < MAX_CLIENTS; ++i) {
              if (connection_list[i].socket != NO_SOCKET && FD_ISSET(connection_list[i].socket, &read_fds)) {
                if (sp_recv(&connection_list[i], &server_message_handler) != 0) {
                  close_client_connection(&connection_list[i]);
                  continue;
                }
              }
              if (connection_list[i].socket != NO_SOCKET && FD_ISSET(connection_list[i].socket, &write_fds)) {
                if (sp_send(&connection_list[i]) != 0) {
                  close_client_connection(&connection_list[i]);
                  continue;
                }
              }
              if (connection_list[i].socket != NO_SOCKET && FD_ISSET(connection_list[i].socket, &except_fds)) {
                printf("Exception client fd.\n");
                close_client_connection(&connection_list[i]);
                continue;
              }
            }
    }
  }
}

#pragma clang diagnostic pop