#include "../server/message.h"

#ifndef ACMS_SERVER_APP_PROTO_H
#define ACMS_SERVER_APP_PROTO_H

#define APP_MSG_NUM 2

size_t pl_size[APP_MSG_NUM];

enum APP_CMD {
    P_AUTH = 0,
    P_AUTH_RESP = 1
};

typedef struct {
    char name[10];
    char login[10];
    char pass[10];
} p_auth;

typedef struct {
    int status;
} p_auth_resp;

void init_pl_size() {
    pl_size[0] = sizeof(p_auth);
    pl_size[1] = sizeof(p_auth_resp);
}

int prepare_packet(int cmd, char *sender, g_msg *msg, void* data) {
    msg->command = cmd;
    sprintf(msg->sender, "%s", sender);
    memcpy(&msg->payload, data, pl_size[cmd]);
    return 0;
}

int log_msg(g_msg *message)
{

    printf("[ACMS][MSG]: Type: %d: Sender %s \n", message->command, message->sender);
    if(message->command == 1) {
        p_auth_resp *acc = (p_auth_resp*) &message->payload;
        printf("[ACMS][MSG]: Payload: %d \\n", acc->status);
    }
    else {
        p_auth *plane = (p_auth*) &message->payload;
        printf("[ACMS][MSG]: Payload: %s \n", plane->name);
    }
    return 0;
}

#endif //ACMS_SERVER_APP_PROTO_H
