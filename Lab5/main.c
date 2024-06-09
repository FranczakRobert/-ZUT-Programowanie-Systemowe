#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <stdbool.h>

void handle(int sig, siginfo_t *si, void *unused);
void handle_CTRLC(int sig);
void create_child_process(int max);
int maxLengthLiveSec;
bool canCreateChildren = true;

static int childRunning = 0;


int main(int argc, char** argv) {
    struct sigaction action;
    sigset_t sigset;

    sigemptyset(&sigset);

    action.sa_sigaction = &handle;
    action.sa_mask = sigset;
    action.sa_flags = SA_SIGINFO;
    sigaction(SIGRTMIN + 10, &action,NULL);

    action.sa_handler = &handle_CTRLC;
    action.sa_flags = 0;
    sigaction(SIGINT, &action, NULL);

     //maxLengthLiveSec ;//= atoi(argv[1]);
    int creationBreakSec ;//= atoi(argv[2]);

    clock_t begin_creation;
    double time_spent;

    begin_creation = clock();
    int retriveOption = 0;
     while((retriveOption = getopt(argc,argv,"w:t:")) != -1) {
        switch (retriveOption) {
            case 'w':
                if(atoi(optarg) > 0)
                    maxLengthLiveSec = atoi(optarg);
                else
                    printf("Value can't be less then 1...\n");
                break;

            case 't':
                if(atoi(optarg) > 0)
                    creationBreakSec = atoi(optarg);
                else
                    printf("Value can't be less then 1...\n");
                break;

            case '?':
                printf("Cos nie tak...\n");
                break;
            
            default:
                exit(1);
        }
    }
    

    while(canCreateChildren) {
        time_spent = (double)(clock() - begin_creation) / CLOCKS_PER_SEC;
        if(time_spent >= creationBreakSec){
            begin_creation = clock();
            create_child_process(maxLengthLiveSec);
        }
    }
    while (childRunning > 0) {}
    
    return 0;
}

void create_child_process(int max) {
    childRunning++;
    int process = fork();

    if(process == 0) {
        char maxLengthLiveSecStr[10]; 
        sprintf(maxLengthLiveSecStr, "%d", maxLengthLiveSec);
        execlp("./process", "process", maxLengthLiveSecStr, NULL);
    }
}


void handle(int sig, siginfo_t *si, void *unused) {
    time_t timestamp;
    char buffer[128];
    struct tm *tm;
    time(&timestamp);
    tm = localtime(&timestamp);
    strftime(buffer, 128, "%a %b %d %H:%M:%S %Y", tm);
    printf("\t");
    printf("[%d] [%d] [%s]\n",si->si_pid, si->si_status,buffer);
    childRunning--;
}

void handle_CTRLC(int sig) {
    canCreateChildren = false;
    printf("\n");
}
