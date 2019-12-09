#ifndef UNTITLED_MQUEUE_H
#define UNTITLED_MQUEUE_H

#include "message.h"

/**
 * Message queue
 */
typedef struct {
    int size;
    message_t *data;
    int current;
} message_queue_t;

int create_message_queue(int queue_size, message_queue_t *queue)
{
    queue->data = calloc(queue_size, sizeof(message_t));
    queue->size = queue_size;
    queue->current = 0;
    return 0;
}

void delete_message_queue(message_queue_t *queue)
{
    free(queue->data);
    queue->data = NULL;
}

int enqueue(message_queue_t *queue, message_t *message)
{
    if (queue->current == queue->size)
        return -1;

    //memcpy(&queue->data[queue->current], message, sizeof(message_t));
    queue->data[queue->current] = *message;
    queue->current++;

    return 0;
}

int dequeue(message_queue_t *queue, message_t *message)
{
    if (queue->current == 0)
        return -1;
    *message = queue->data[queue->current-1];
    //memcpy(message, &queue->data[queue->current - 1], sizeof(message_t));
    queue->current--;

    return 0;
}

int dequeue_all(message_queue_t *queue)
{
    queue->current = 0;

    return 0;
}
#endif //UNTITLED_MQUEUE_H
