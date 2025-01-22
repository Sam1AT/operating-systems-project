#ifndef BUFFER_H_INCLUDED
#define BUFFER_H_INCLUDED

typedef int buffer_item;
#define BUFFER_SIZE 5

#include <semaphore.h>
#include <pthread.h>

extern sem_t *full, *empty;
pthread_mutex_t lock;

int insert_item(buffer_item item);
int remove_item(buffer_item *item);
void *producer(void *value);
void *consumer(void *value);


#endif
