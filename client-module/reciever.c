#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

#include <fcntl.h>
#include <sys/stat.h>
#include <mqueue.h>

#define SERVER_QUEUE_NAME   "/olo3"
#define QUEUE_PERMISSIONS 0777
#define MAX_MESSAGES 10
#define MAX_MSG_SIZE 1024
#define MSG_BUFFER_SIZE MAX_MSG_SIZE + 10


#define SENDER_MAXSIZE 128
#define DATA_MAXSIZE 1024
#include "../protocol/ap_data.h"


int main (int argc, char **argv)
{
    mqd_t qd_server;   // queue descriptors
    long token_number = 1; // next token to be given to client

    printf ("Reciever: Hello, World!\n");

    struct mq_attr attr;

    attr.mq_flags = 0;
    attr.mq_maxmsg = MAX_MESSAGES;
    attr.mq_msgsize = sizeof(g_msg);
    attr.mq_curmsgs = 0;

    if ((qd_server = mq_open (SERVER_QUEUE_NAME, O_RDONLY | O_CREAT, QUEUE_PERMISSIONS, &attr)) == -1) {
        perror ("Reciever: mq_open (reciever)");
        exit (1);
    }
    char in_buffer [MSG_BUFFER_SIZE];
    g_msg msg;

    while (1) {
        // get the oldest message with highest priority
        unsigned int size = mq_receive (qd_server, (char*) &msg, sizeof(g_msg), NULL);
        if (size == -1) {
            perror (" ERROR ! Reciever: mq_receive");
            exit (1);
        } else {
            in_buffer[size+1]='\0';
            printf ("Reciever: message received: %s \n", msg.sender);
        }

    }
}