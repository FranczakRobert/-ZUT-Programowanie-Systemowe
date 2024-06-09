#ifndef TIMEMANAGER_H
#define TIMEMANAGER_H

#include <time.h>

typedef struct {
    struct timespec start_time;
    struct timespec stop_time;
} ThreadData;

void destructor(void* value);
void start();
long stop();

#endif // TIMEMANAGER_H
