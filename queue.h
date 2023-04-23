#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

typedef struct Queue_Item {
    uint64_t data;
    struct Queue_Item* next;
} Queue_Item;

typedef struct Queue{
    struct Queue_Item* first;
    struct Queue_Item* last;
    pthread_mutex_t* lock;
} Queue;

Queue* init_queue()
{

    Queue* queue = (Queue*) malloc(sizeof(Queue));
    queue->first = NULL;
    queue->last = NULL;
    queue->lock = (pthread_mutex_t*) malloc(sizeof(pthread_mutex_t));
    pthread_mutex_init(queue->lock, NULL);


    return queue;
}

void free_queue(Queue* queue)
{
    free((void*)queue->lock);
    free((void*)queue);
}

void queue_push(Queue* queue, uint64_t data)
{
    Queue_Item* item = (Queue_Item*) malloc(sizeof(Queue_Item));
    item->data = data;
    item->next = NULL;

    pthread_mutex_lock(queue->lock);
    if (queue->first == NULL && queue->last == NULL)
    {
        queue->first = item;
        queue->last = item;
    }
    else
    {
        queue->last->next = (struct Queue_Item*) item;
        queue->last = item;
    }
    pthread_mutex_unlock(queue->lock);
}

Queue_Item* queue_pop(Queue* queue)
{
    pthread_mutex_lock(queue->lock);

    Queue_Item* item = queue->first;

    // If there's 0 or 1 items in the queue set first/last to NULL
    if (queue->first == queue->last)
    {
        queue->first = NULL;
        queue->last = NULL;
    }
    else
        queue->first = item->next;
    pthread_mutex_unlock(queue->lock);

    return item;
}

void print_queue(Queue* queue)
{
    Queue_Item* item = queue->first;
    if (item == NULL)
        return;

    while(1)
    {
        printf("%llu\n", item->data);

        if (item == queue->last)
            break;
        else
            item = item->next;
    }
}
