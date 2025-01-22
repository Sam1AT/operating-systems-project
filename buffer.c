#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include <semaphore.h>
#include <pthread.h>

#include "buffer.h"

#define MAX_SLEEP 2

buffer_item buffer[BUFFER_SIZE];
size_t head=0, tail=0;

sem_t *full = NULL;
sem_t *empty = NULL;
pthread_mutex_t lock;

int insert_item(buffer_item item) {
    sem_wait(empty);
    pthread_mutex_lock(&lock);

    buffer[tail] = item;
    tail = (tail + 1) % (BUFFER_SIZE);

    pthread_mutex_unlock(&lock);
    sem_post(full);

    return 0;
}

int remove_item(buffer_item *item) {
    sem_wait(full);
    pthread_mutex_lock(&lock);

    *item = buffer[head];
    head = (head + 1) % (BUFFER_SIZE);

    pthread_mutex_unlock(&lock);
    sem_post(empty);

    return 0;
}

void *producer(void *value) {
    buffer_item item;
    while (1) {
        sleep(rand() % MAX_SLEEP + 1);
        item = rand();
        if (insert_item(item)) {
            fprintf(stderr, "Failed Inserting.\n");
        } else {
            printf("Producer Number %i produced %d.\n", *(int *)value, item);
        }
    }
}


void *consumer(void *value) {
    buffer_item item;
    while (1) {
        sleep(rand() % MAX_SLEEP + 1);
        if (remove_item(&item)) {
            fprintf(stderr, "Failed Removing.\n");
        } else {
            printf("Producer Number %i consumed %d.\n", *(int *)value, item);
        }
    }
}