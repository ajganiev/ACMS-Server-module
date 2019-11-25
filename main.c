#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/time.h>


#include "protocols/protocol-app.h"

#define SERVER_PORT 8888
#define MAX_CL_NUM 30
#define MAX_MSG_SIZE 1025
/**
 * Core socket server that will connect all CLIENT modules to the system
 **/

char* log_protocol(struct APP_PROTO_MSG msg) {
    printf("ACMS APP PROTO v.0.0.1:\nHeader: ID => %d CMD => %d ST => %d ACK => %d RECV => %d",
            msg.id, msg.command, msg.status, msg.ack_id, msg.recv_id);
    char* prepare_body = msg.body;
    prepare_body[msg.size] = "\0"; // just in case
    printf("Body: [%s]", msg.body);
}

void recv_msg(int sd) {
    int msg_size;
    char buffer[MAX_MSG_SIZE];
    if(msg_size = read(sd , buffer, 1024) != 0)  {
        struct APP_PROTO_MSG msg = proto_decode_msg(buffer);
        log_protocol(msg);
    }
};

void send_msg();

void setup_socket_server() {
    int master_sock,
            addrlen,
            new_socket,
            client_socket[MAX_CL_NUM],
            activity, i,
            valread,
            sd,
            max_sd,
            opt = 1;

    struct sockaddr_in address;
    char buffer[1025];
    fd_set readfds;

    char *message = "[ACMS] Server Daemon v 0.0.1 \r\n";
    for (i = 0; i < MAX_CL_NUM; i++) client_socket[i] = 0;

    if( (master_sock = socket(AF_INET , SOCK_STREAM , 0)) == 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    if( setsockopt(master_sock, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt)) < 0 )
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(SERVER_PORT);

    if (bind(master_sock, (struct sockaddr *)&address, sizeof(address))<0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    printf("ACMS Server Daemon v 0.0.1\nRunning on port %d \n", SERVER_PORT);

    if (listen(master_sock, 3) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    //accept the incoming connection
    addrlen = sizeof(address);
    puts("[Daemon]: Waiting for connections...");


    /**
     * Main hanlder
     * Users select() to switch between socket descriptors
     **/
    while(1) {
        FD_ZERO(&readfds);
        FD_SET(master_sock, &readfds);
        max_sd = master_sock;

        for ( i = 0 ; i < MAX_CL_NUM ; i++)
        {
            sd = client_socket[i];
            if(sd > 0) FD_SET(sd , &readfds);
            if(sd > max_sd) max_sd = sd;
        }

        activity = select(max_sd + 1 , &readfds, NULL, NULL, NULL);

        if ((activity < 0) && (errno!=EINTR)) printf("select error"); // todo add logger

        //Master socket getting active, handle incoming client

        if (FD_ISSET(master_sock, &readfds))
        {
            if ((new_socket = accept(master_sock, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0)
            {
                perror("accept");
                exit(EXIT_FAILURE);
            }
            printf("[Daemon] New connection! socket desc: %d , ip is : %s , port : %d\n" , new_socket , inet_ntoa(address.sin_addr) , ntohs(address.sin_port));
            if(send(new_socket, message, strlen(message), 0) != strlen(message)) perror("send");
            for (i = 0; i < MAX_CL_NUM; i++)
            {
                if( client_socket[i] == 0 )
                {
                    client_socket[i] = new_socket;
                    printf("[Daemon] Adding to list of sockets as %d\n" , i);
                    break;
                }
            }
        }

        //Client socket is getting active

        for (i = 0; i < MAX_CL_NUM; i++)
        {
            sd = client_socket[i];
            if (FD_ISSET(sd , &readfds))
            {
                if ((valread = read(sd , buffer, 1024)) == 0)
                {
                    getpeername(sd, (struct sockaddr*)&address, \
						(socklen_t*)&addrlen);
                    printf("[Daemon] Host disconnected , ip %s , port %d \n", inet_ntoa(address.sin_addr) , ntohs(address.sin_port));
                    close(sd);
                    client_socket[i] = 0;
                }
                else
                {
                    buffer[valread] = '\0';
                    send(sd , buffer , strlen(buffer) , 0 );
                }
            }
        }
    }
}

int main(int argc , char *argv[])
{
    setup_socket_server();
    return 0;
}
