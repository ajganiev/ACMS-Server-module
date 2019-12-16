#ifndef ACMS_SERVER_PROTO_COMMON_H
#define ACMS_SERVER_PROTO_COMMON_H
#define APP_MSG_NUM 2

#include "ap_data.h"
#include "app_sv_handlers.h"
#include "app_cl_handlers.h"
#include "../server/socket.h"
size_t pl_size[APP_MSG_NUM];
//int (*handlers[APP_MSG_NUM]) (g_msg* msg, void*);

void init_handlers() {
    pl_size[0] = sizeof(p_auth);
    pl_size[1] = sizeof(p_auth_resp);
}

int prepare_packet(int cmd, char *sender, g_msg *msg, void* data, size_t size) {
    static int pck_count;
    msg->command = cmd;
    msg->id = pck_count++;
    sprintf(msg->sender, "%s", sender);
    memcpy(&msg->payload, data, size);
    return 0;
}


int log_msg(g_msg *message)
{
    printf("[ACMS][MSG]: Type: %d: Sender %s \n", message->command, message->sender);
    if(message->command == 1) {
        p_auth_resp *acc = (p_auth_resp*) &message->payload;
        printf("[ACMS][MSG]: Payload: %d \n", acc->status);
    }
    else {
        p_auth *plane = (p_auth*) &message->payload;
        printf("[ACMS][MSG]: Payload: %s %s \n", plane->login, plane->pass);
    }
    return 0;
}

int server_message_handler(socket_peer * peer, g_msg* msg) {
    switch (msg->command) {
        case P_AUTH: {
            g_msg answer;
            p_auth_resp data = handle_p_auth(msg);
            prepare_packet(P_AUTH_RESP, "noone", &answer, &data, sizeof(p_auth_resp));
            mq_send(peer, &answer);
        }
    }
};

int client_message_handler(socket_peer * peer, g_msg* msg) {
    switch (msg->command) {
        case P_AUTH_RESP: {
            handle_p_auth_resp(msg);
            break;
        }
    }
};






#endif //ACMS_SERVER_PROTO_COMMON_H
