#include "timeManager.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <signal.h>
#include <unistd.h>

pthread_once_t PTHREAD = PTHREAD_ONCE_INIT;
pthread_key_t key;

static void free_memory(void *buffer) {
    if (buffer) {
        free(buffer);
    }
}

static void create_key(void) {
    pthread_key_create(&key, free_memory);
}


long stop()
{
    ThreadData* data = (ThreadData*) pthread_getspecific(key);
    clock_gettime(CLOCK_REALTIME, &(data->stop_time));
    long duration = (data->stop_time.tv_sec - data->start_time.tv_sec) * 1000;
    duration += (data->stop_time.tv_nsec - data->start_time.tv_nsec) / 1000000;

    return duration;
}

void start() {
    pthread_once(&PTHREAD,create_key);
    ThreadData* data = (ThreadData*) pthread_getspecific(key);
    if(data == NULL) {
        data = malloc(sizeof(ThreadData));
        pthread_setspecific(key,data);
    }
    clock_gettime(CLOCK_REALTIME, &(data->start_time));
}