#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
int random_number;

void handle_alarm(int sig) {
    union sigval sv;
    sv.sival_int = random_number;
    sigqueue(getppid(), SIGRTMIN + 10, sv);

    exit(random_number);
}

void handle_CTRLC(int sig) {
    
}

int main(int argc, char** argv) {
    
    struct sigaction action;
    sigset_t sigset;

    srand(time(NULL));

    sigemptyset(&sigset);

    action.sa_handler = &handle_alarm;
    action.sa_mask = sigset;
    action.sa_flags = 0;
    sigaction(SIGALRM, &action,NULL);

    action.sa_handler = &handle_CTRLC;
    action.sa_flags = 0;
    sigaction(SIGINT, &action, NULL);

    double task_counter;
    clock_t begin_task;
    
    random_number = rand() % atoi(argv[1]) + 1;

    time_t timestamp;
    char buffer[128];
    struct tm *tm;
    time(&timestamp);
    tm = localtime(&timestamp);
    strftime(buffer, 128, "%a %b %d %H:%M:%S %Y", tm);

    printf("[%d] [%d] [%s]\n",getpid(), random_number,buffer);

    alarm(random_number); 
    long zaawansowane_obliczenia = 1;

    task_counter = clock();
    task_counter = 0;
    while(1) {
        zaawansowane_obliczenia *= zaawansowane_obliczenia + 1;
        task_counter = (double)(clock() - begin_task) / CLOCKS_PER_SEC;
    }
}