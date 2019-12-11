#ifndef ACMS_MESSAGE_H
#define ACMS_MESSAGE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "config.h"
#include "../airplane/plane.h"

enum CMD {
    AUTH = 0
};

typedef struct {
    enum CMD command;
    char sender[SENDER_MAXSIZE];
    char payload[DATA_MAXSIZE];
}  g_msg;

typedef struct {
    enum CMD command;
    char sender[SENDER_MAXSIZE];
    plane_data payload;
} data_msg;

typedef struct {
    char name[10];
    char login[10];
    char password[10];
} account;



int prep_data_msg_plane(enum CMD cmd, char *sender, char *data, g_msg *message)
{
    plane_data plane;
    sprintf(plane.uuid, "%s", "ice-235");
    plane.speed = 434.5;
    plane.x = 23.45;
    plane.y = 45.43;
    plane.height = 1235.5;
    message->command = cmd;
    sprintf(message->sender, "%s", sender);
    memcpy(&message->payload, &plane, sizeof(plane));
    return 0;
}

int prep_data_msg_acc(enum CMD cmd, char *sender, char *data, g_msg *message)
{
    account acc;
    sprintf(acc.name, "%s", "Aziz");
    sprintf(acc.login, "%s", "aj");
    sprintf(acc.password, "%s", "123qwerty");
    message->command = cmd;
    sprintf(message->sender, "%s", sender);
    memcpy(&message->payload, &acc, sizeof(acc));
    return 0;
}

int prep_cmd(enum CMD cmd, char *sender, char *data, g_msg *message)
{
    message->command = cmd;
    sprintf(message->sender, "%s", sender);
    sprintf(message->payload, "%s", data);
    return 0;
}

//int log_msg(g_msg *message)
//{
//
//    printf("[ACMS][MSG]: Type: %d: Sender %s \n", message->command, message->sender);
//    if(message->command == 1) {
//        account *acc = (account*) &message->payload;
//        printf("[ACMS][MSG]: Payload: %s \\n", acc->name);
//    }
//    else {
//        plane_data *plane = (plane_data*) &message->payload;
//        printf("[ACMS][MSG]: Payload: %lf \n", plane->height);
//    }
////    printf("[ACMS][MSG]: Payload: %lf \n", message->payload.height);
//    return 0;
//}

#endif //ACMS_MESSAGE_H
