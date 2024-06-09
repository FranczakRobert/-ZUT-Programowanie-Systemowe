#include "timeManager.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <signal.h>
#include <unistd.h>


volatile int keep_running = 1;

void* compute(void* arg) {
    ThreadData* data = (ThreadData*) malloc(sizeof(ThreadData));
    // pthread_setspecific(key, data);
    pthread_t id = pthread_self();

    start();
    int n = 2;

    while (1) {
        if (keep_running == 0) {
            keep_running = 1;
            long result = stop();
            printf("\t\tWatek %lu Czas wykonania: %ld ms\n", (long)id,result);
            pthread_exit(NULL);
        }
        n = n * n;
                                                                                                                                                                                                                                sleep(1);
    }

   return NULL;
}


void signal_handler(int sig) {
    if (sig == SIGINT) {
        keep_running = 0;
    }
}

int main(int argc, char** argv) {

    if(argc <= 1) {
        printf("Podaj: \n arg 1 = maxymalna długość zycia wątkow.\n arg 2 = ilość wątokw\n");
        return -1;
    }

    
    int max_thread_life  = atoi(argv[1]);
    int number_of_threads = atoi(argv[2]);
    int threads_life_times_array[number_of_threads];
    int cpu_time_used;
    pthread_t threads_IDs[number_of_threads];
    clock_t start, end;

    struct sigaction sa;
    sa.sa_handler = signal_handler;
    sa.sa_flags = 0;
    sigemptyset(&sa.sa_mask);
    sigaction(SIGINT, &sa, NULL);

    srand(time(NULL));
    start = clock() / CLOCKS_PER_SEC;

    for (int i = 0; i < number_of_threads; i++) {
        pthread_t thread_id;
        pthread_create(&thread_id, NULL,compute, NULL);
        int life_time = rand() % max_thread_life + 1;
        threads_life_times_array[i] = life_time;
        threads_IDs[i] = thread_id;
        printf("Wątek %d ID:[%lu] Czas życia:[%d]\n",i,(long)threads_IDs[i],life_time);
    }

    while (1)
    {
        end = clock();
        cpu_time_used = ((end - start)) / CLOCKS_PER_SEC;
        int x = 1;
        int all_threads_terminated = 1;

        for(int i = 0; i < number_of_threads; i++) {
            if(threads_life_times_array[i] <= cpu_time_used) {
                pthread_kill(threads_IDs[i], SIGINT);
                pthread_join(threads_IDs[i], NULL);
            }
            else {
                all_threads_terminated = 0;
            }
        }
        if (all_threads_terminated) {
            break;
        }
    }

    return 0;
}

