
#include "queue.h"

int main()
{
    // Setup Queue
    Queue* queue = init_queue();

    queue_push(queue, 128);
    queue_push(queue, 129);
    queue_push(queue, 130);

    printf("printing list\n");
    print_queue(queue);

    printf("poping list\n");
    Queue_Item* item;
    item = queue_pop(queue);
    printf("%llu\n", item->data);
    free((void*)item);
    item = queue_pop(queue);
    printf("%llu\n", item->data);
    free((void*)item);
    item = queue_pop(queue);
    printf("%llu\n", item->data);
    free((void*)item);

    printf("printing list\n");
    print_queue(queue);

    queue_push(queue, 1280);
    queue_push(queue, 1290);
    queue_push(queue, 1300);

    printf("printing list\n");
    print_queue(queue);

    item = queue_pop(queue);
    printf("%llu\n", item->data);
    free((void*)item);

    free_queue(queue);

    return 0;

}
