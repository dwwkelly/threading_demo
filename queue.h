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
    pthread_cond_t* cond;
} Queue;

Queue* init_queue()
{

    Queue* queue = (Queue*) malloc(sizeof(Queue));
    queue->first = NULL;
    queue->last = NULL;
    queue->lock = (pthread_mutex_t*) malloc(sizeof(pthread_mutex_t));
    pthread_mutex_init(queue->lock, NULL);
    queue->cond = (pthread_cond_t*) malloc(sizeof(pthread_cond_t));
    pthread_cond_init(queue->cond, NULL);

    return queue;
}

void free_queue(Queue* queue)
{

    // TODO loop over queue items and free everything in the queue

    free((void*)queue->lock);
    queue->lock = NULL;
    free((void*)queue->cond);
    queue->cond = NULL;
    free((void*)queue);
    queue = NULL;
}

void queue_push(Queue* queue, uint64_t data)
{
    Queue_Item* item = (Queue_Item*) malloc(sizeof(Queue_Item));
    item->data = data;
    item->next = NULL;

    pthread_mutex_lock(queue->lock);
    if (queue->first == NULL && queue->last == NULL)
    {
        // This is the first item being added to the queue
        queue->first = item;
        queue->last = item;
    }
    else
    {
        queue->last->next = (struct Queue_Item*) item;
        queue->last = item;
    }
    pthread_cond_signal(queue->cond);
    pthread_mutex_unlock(queue->lock);
}

Queue_Item* queue_pop(Queue* queue)
{
    pthread_mutex_lock(queue->lock);
     while (queue->first == NULL)
         pthread_cond_wait(queue->cond, queue->lock);

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
