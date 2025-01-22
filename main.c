#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

#include <pthread.h>

#include "buffer.h"

int main(int argc, char *argv[]) {
    
    int sleepSecond = atoi(argv[1]);
    size_t numProducer = atoi(argv[2]);
    size_t numConsumer = atoi(argv[3]);

    full = sem_open("/full_semaphore", O_CREAT, 0644, 0);
    empty = sem_open("/empty_semaphore", O_CREAT, 0644, BUFFER_SIZE);

    pthread_mutex_init(&lock, NULL);


    pthread_t *producers = (pthread_t *)malloc(numProducer * sizeof(pthread_t));
    int *producer_id = (int *)malloc(numProducer * sizeof(pthread_t));
    for(size_t i = 0 ; i != numProducer; ++i) {
        producer_id[i] = i + 1;
        pthread_create(&producers[i], NULL, producer, &producer_id[i]);
    }

    pthread_t *consumers = (pthread_t *)malloc(numConsumer * sizeof(pthread_t));
    int *consumer_id = (int *)malloc(numConsumer * sizeof(pthread_t));
    for(int i = 0; i != numConsumer; ++i) {
        consumer_id[i] = i + 1;
        pthread_create(&consumers[i], NULL, consumer, &consumer_id[i]);
    }

    sleep(sleepSecond);
    for(int i = 0; i != numProducer; ++i) {
        pthread_cancel(producers[i]);
        pthread_join(producers[i], NULL);
    }

    sleep(sleepSecond);   
    for(int i = 0; i != numConsumer; ++i) {
        pthread_cancel(consumers[i]);
        pthread_join(consumers[i], NULL);
    }

    sem_close(full);
    sem_unlink("/full_semaphore");

    sem_close(empty);
    sem_unlink("/empty_semaphore");
    
    pthread_mutex_destroy(&lock);

    return 0;
}