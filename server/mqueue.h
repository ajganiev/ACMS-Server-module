#ifndef ACMS_MQUEUE_H
#define ACMS_MQUEUE_H

#include "config.h"

typedef struct {
    int command;
    char sender[SENDER_MAXSIZE];
    char payload[DATA_MAXSIZE];
} g_msg;

typedef struct {
    int size;
    g_msg *data;
    int current;
} peer_mq;

int mq_create(int queue_size, peer_mq *queue)
{
    queue->data = calloc(queue_size, sizeof(g_msg));
    queue->size = queue_size;
    queue->current = 0;
    return 0;
}

void mq_remove(peer_mq *queue)
{
    free(queue->data);
    queue->data = NULL;
}

int mq_enqueue(peer_mq *queue, g_msg *message)
{
    if (queue->current == queue->size) return -1;
    queue->data[queue->current] = *message;
    queue->current++;
    return 0;
}

int mq_dequeue(peer_mq *queue, g_msg *message)
{
    if (queue->current == 0) return -1;
    *message = queue->data[queue->current-1];
    queue->current--;
    return 0;
}

int mq_flush(peer_mq *queue)
{
    queue->current = 0;
    return 0;
}

#endif //ACMS_MQUEUE_H
