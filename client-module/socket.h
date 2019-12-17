#ifndef ACMS_SOCKET_H
#define ACMS_SOCKET_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "msg.h"
#include <mqueue.h>

typedef struct {
    int socket;
    struct sockaddr_in address;
    peer_mq send_buffer;
    mqd_t *qd_client;
    g_msg sending_buffer;
    size_t current_sending_byte;
    g_msg receiving_buffer;
    size_t current_receiving_byte;
} socket_peer;

int sp_delete(socket_peer *peer)
{
    close(peer->socket);
    mq_remove(&peer->send_buffer);
}

int sp_create(socket_peer *peer, mqd_t *qd_client)
{
    mq_create(MAX_MESSAGES_BUFFER_SIZE, &peer->send_buffer);
    peer->qd_client = qd_client;
    peer->current_sending_byte   = -1;
    peer->current_receiving_byte = 0;
    return 0;
}

char *get_ip_str(socket_peer *peer)
{
    static char ret[INET_ADDRSTRLEN + 10];
    char peer_ipv4_str[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &peer->address.sin_addr, peer_ipv4_str, INET_ADDRSTRLEN);
    sprintf(ret, "%s:%d", peer_ipv4_str, peer->address.sin_port);
    return ret;
}

int sp_recv(socket_peer *peer, int (*message_handler)(socket_peer*, g_msg *))
{
    printf("[ACMS] Got message from %s.\n", get_ip_str(peer));
    size_t len_to_receive;
    ssize_t received_count;
    size_t received_total = 0;
    do {
        if (peer->current_receiving_byte >= sizeof(peer->receiving_buffer)) {
            message_handler(peer, &peer->receiving_buffer);
            peer->current_receiving_byte = 0;
        }
        len_to_receive = sizeof(peer->receiving_buffer) - peer->current_receiving_byte;
        if (len_to_receive > MAX_SEND_SIZE)
            len_to_receive = MAX_SEND_SIZE;
        received_count = recv(peer->socket, (char *)&peer->receiving_buffer + peer->current_receiving_byte, len_to_receive, MSG_DONTWAIT);
        if (received_count < 0) {
            if (errno == EAGAIN || errno == EWOULDBLOCK) printf("[ACMS] Peer is not ready.\n");
            else {
                perror("recv error");
                return -1;
            }
        }
        else if (received_count < 0 && (errno == EAGAIN || errno == EWOULDBLOCK)) break;
        else if (received_count == 0) {
            printf("[ACMS] Got disconnect from %s.\n", get_ip_str(peer));
            return -1;
        }
        else if (received_count > 0) {
            peer->current_receiving_byte += received_count;
            received_total += received_count;
        }
    } while (received_count > 0);
    printf("[ACMS] received %zu bytes in total.\n", received_total);
    return 0;
}

int sp_send(socket_peer *peer)
{
    printf("[ACMS] Sending to %s.\n", get_ip_str(peer));
    size_t len_to_send;
    ssize_t send_count;
    size_t send_total = 0;
    do {
        if (peer->current_sending_byte < 0 || peer->current_sending_byte >= sizeof(peer->sending_buffer)) {
            printf("[ACMS] Polling queue");
            if (mq_dequeue(&peer->send_buffer, &peer->sending_buffer) != 0) {
                peer->current_sending_byte = -1;
                printf("[ACMS] Queue is empty.\n");
                break;
            }
            printf("[ACMS] Got msg from queue.\n");
            peer->current_sending_byte = 0;
        }
        len_to_send = sizeof(peer->sending_buffer) - peer->current_sending_byte;
        if (len_to_send > MAX_SEND_SIZE)
            len_to_send = MAX_SEND_SIZE;
        send_count = send(peer->socket, (char *)&peer->sending_buffer + peer->current_sending_byte, len_to_send, 0);
        if (send_count < 0) {
            if (errno == EAGAIN || errno == EWOULDBLOCK) printf("[ACMS] Peer is not ready.\n");
            else {
                perror("send error");
                return -1;
            }
        }
        else if (send_count < 0 && (errno == EAGAIN || errno == EWOULDBLOCK)) break;
        else if (send_count == 0) {
            printf("[ACMS] Sent 0 bytes. Something's wrong...\n");
            break;
        }
        else if (send_count > 0) {
            peer->current_sending_byte += send_count;
            send_total += send_count;
        }
    } while (send_count > 0);
    printf("[ACMS] sent %zu bytes in total.\n", send_total);
    return 0;
}

void server_mq_send(socket_peer *peer, g_msg *msg) {
    mq_enqueue(&peer->send_buffer, msg);
}

#endif //ACMS_SOCKET_H
