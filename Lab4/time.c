#include <getopt.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <time.h>

struct Option {
    unsigned int stdOUT : 1;
    unsigned int numbersOfCalls : 8;
};

// Funkcja do pomiaru czasu w mikrosekundach
double getElapsedTime(struct timespec start, struct timespec end) {
    return (double)(end.tv_sec - start.tv_sec) * 1e6 + (double)(end.tv_nsec - start.tv_nsec) / 1e3;
}

int main(int argc, char **argv) {
    int retriveOption;
    struct Option option;
    option.numbersOfCalls = 1;
    double real_total = 0.0, user_total = 0.0, sys_total = 0.0;
    
    if(argc < 2) {
        printf("Podaj nazwę programu oraz jego argumenty\n");
        return -1;
    }

    while((retriveOption = getopt(argc,argv,"+vt:")) != -1) {
        switch (retriveOption) {
            case 'v':
                option.stdOUT = 1;
                break;

            case 't':
                if(atoi(optarg) > 0)
                    option.numbersOfCalls = atoi(optarg);
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

    for(int i = 0; i < option.numbersOfCalls; i++) {
        pid_t pid = fork();
        
        if(pid == 0) {
            if(option.stdOUT == 0) {
                int devNull = open("/dev/null", O_WRONLY);
                dup2(devNull, STDOUT_FILENO);
                dup2(devNull, STDERR_FILENO);
                close(devNull);
            }

            execvp(argv[optind], &argv[optind]);
            perror("execvp");
            exit(EXIT_FAILURE);
        }
    }

    for(int i = 0; i < option.numbersOfCalls; i++) {
        int status;
        struct rusage rusage; 
        struct timespec start, end;

        clock_gettime(CLOCK_REALTIME, &start); 
        
        pid_t a = wait3(&status, 0, &rusage); 

        clock_gettime(CLOCK_REALTIME, &end); 

        double real_time = getElapsedTime(start, end); 
        double user_time = (double) rusage.ru_utime.tv_sec * 1000000 + rusage.ru_utime.tv_usec;
        double sys_time = (double) rusage.ru_stime.tv_sec * 1000000 + rusage.ru_stime.tv_usec;

        real_total += real_time;
        user_total += user_time;
        sys_total += sys_time;


        printf("Real: %.2f microseconds\n", real_time);
        printf("User: %.2f microseconds\n", user_time);
        printf("System: %.2f microseconds\n-------------------------\n\n", sys_time);
    }

    if(option.numbersOfCalls > 1) {
        printf("\nAverage times:\n");
        printf("Real: %.2f microseconds\n", real_total / option.numbersOfCalls);
        printf("User: %.2f microseconds\n", user_total / option.numbersOfCalls);
        printf("System: %.2f microseconds\n", sys_total / option.numbersOfCalls);
    }

    return 0;
}
