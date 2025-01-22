#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

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
    if (sem_wait(empty) != 0 || pthread_mutex_lock(&lock) != 0) {
        printf("Error: sem_wait or mutex lock goes wrong!");
        return -1;  
    }

    buffer[tail] = item;
    tail = (tail + 1) % (BUFFER_SIZE);

    if (sem_post(full) != 0 || pthread_mutex_unlock(&lock) != 0) {
        printf("Error: sem_post or mutex unlock goes wrong!");
        return -1;  
    }

    return 0;
}

int remove_item(buffer_item *item) {
    if (sem_wait(full) != 0 || pthread_mutex_lock(&lock) != 0) {
        printf("Error: sem_wait or mutex lock goes wrong!");
        return -1;  
    }

    *item = buffer[head];
    head = (head + 1) % (BUFFER_SIZE);

    if (sem_post(empty) != 0 || pthread_mutex_unlock(&lock) != 0) {
        printf("Error: sem_post or mutex unlock goes wrong!");
        return -1;  
    }

    return 0;
}

void *producer(void *value) {
    buffer_item item;
    while (1) {
        sleep(rand() % MAX_SLEEP + 1);
        item = rand();
        if (insert_item(item)) {
            printf("Failed Inserting.\n");
        } 
        else {
            printf("Producer Number %i produced %d.\n", *(int *)value, item);
        }
    }
}


void *consumer(void *value) {
    buffer_item item;
    while (1) {
        sleep(rand() % MAX_SLEEP + 1);
        if (remove_item(&item)) {
            printf("Failed Consuming.\n");
        } 
        else {
            printf("Producer Number %i consumed %d.\n", *(int *)value, item);
        }
    }
}