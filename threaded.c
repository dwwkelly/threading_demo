#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <pthread.h>

#include "prime_factor.h"
#include "queue.h"

#define BUFLEN 64
#define PORT 5000

typedef struct Thread_Input {
    struct Queue* queue;
    int socket;
    pthread_mutex_t* socket_lock;
} Thread_Input;


void* Process(void* _thread_input)
{
    if (_thread_input == NULL)
        return NULL;

    Thread_Input* thread_input = (Thread_Input*) _thread_input;
    struct Queue* queue = thread_input->queue;

    while(1)
    {
        Queue_Item* item = queue_pop(queue);
        if (item == NULL)
            continue;

        uint64_t input = item->data;

        prime_factor(input);
    }

    return NULL;
}


int main()
{
    // Setup Queue
    Queue* queue = init_queue();

    // Setup UDP sockets
    int socket_fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    socklen_t server_struct_length = sizeof(server_addr);
    size_t msg_len = 64;
    char server_message[msg_len];

    bind(socket_fd,
         (const struct sockaddr *) &server_addr,
         sizeof(server_addr));

    // Setup queue inputs
    pthread_mutex_t* socket_lock = (pthread_mutex_t*) malloc(sizeof(pthread_mutex_t));
    pthread_mutex_init(socket_lock, NULL);

    // Setup thread input
    Thread_Input* thread_input = (Thread_Input*) malloc(sizeof(Thread_Input));
    thread_input->queue = queue;
    thread_input->socket = socket_fd;
    thread_input->socket_lock = socket_lock;

    // Create threads
    size_t n_threads = 4;
    pthread_t thread_array[n_threads];
    int counter = 0;
    for (counter=0; counter<n_threads; counter++)
    {
        pthread_create(&(thread_array[counter]), NULL, Process, thread_input);
    }

    // Main loop, waits for messages to show then puts them on the queue
    while(1)
    {
        memset(server_message, 0x00, msg_len);
        // Wait for a message to show up
        recvfrom(socket_fd,
                 server_message,
                 msg_len,
                 0,
                 (struct sockaddr*)&server_addr,
                 &server_struct_length);

        uint64_t input = (uint64_t) server_message;

        queue_push(queue, input);
    }

    for (counter=0; counter<n_threads; counter++)
    {
        pthread_join(thread_array[counter], NULL);
    }

    free_queue(queue);

    return 0;
}
