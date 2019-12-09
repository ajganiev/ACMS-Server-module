#ifndef UNTITLED_MESSAGE_H
#define UNTITLED_MESSAGE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "config.h"

//Change to protocol
typedef struct {
    char sender[SENDER_MAXSIZE];
    char data[DATA_MAXSIZE];
}  message_t;

int prepare_message(char *sender, char *data, message_t *message)
{
    sprintf(message->sender, "%s", sender);
    sprintf(message->data, "%s", data);
    return 0;
}

int print_message(message_t *message)
{
    printf("Message: \"%s: %s\"\n", message->sender, message->data);
    return 0;
}

#endif //UNTITLED_MESSAGE_H
