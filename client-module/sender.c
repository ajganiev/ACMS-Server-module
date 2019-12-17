#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include "config.h"
#include <fcntl.h>
#include <sys/stat.h>
#include <mqueue.h>

int main (int argc, char **argv)
{
    mqd_t qd_server, qd_client;   // queue descriptors
    long token_number = 1; // next token to be given to client

    printf ("Sender: Hello, World!\n");

    struct mq_attr attr;

    attr.mq_flags = 0;
    attr.mq_maxmsg = MAX_MESSAGES;
    attr.mq_msgsize = MAX_MQ_MSG_SIZE;
    attr.mq_curmsgs = 0;

    char out_buffer [MSG_BUFFER_SIZE] = "Suck";


    if ((qd_client = mq_open (CLIENT_QUEUE_NAME, O_WRONLY)) == 1) {
        perror ("Sender: Not able to open client queue");
    }


    if (mq_send (qd_client, out_buffer, strlen (out_buffer) + 1, 0) == -1) {
        perror ("Sender: Not able to send message to client");
    }else{
        printf("Sended: %s", out_buffer);
    }

}