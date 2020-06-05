#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <time.h>

int np = 5;
double mean = 0;
pid_t pid;

void signal_handler(int signo){
    write(STDOUT_FILENO, "Child signal received\n", strlen("Child signal received\n"));
    int status;
    wait(&status);
    printf("Received status: %d\n", WEXITSTATUS(status));
    if (WEXITSTATUS(status)) mean += WEXITSTATUS(status);
    np--;
}

int main(){
    srand(time(0));
    int safe = 5;
 
    struct sigaction action;
    action.sa_handler = signal_handler;
    sigemptyset(&action.sa_mask);
    action.sa_flags = 0;

    if (sigaction(SIGCHLD,&action,NULL) < 0){
        fprintf(stderr,"Unable to install SIGINT handler\n");
        exit(1);
    }

    //process creation
    while(np > 0){
        int num = rand() % 10;
        pid = fork();
        if (pid > 0) pause();
        else if (pid == 0){
            printf("New child created\n");
            int num = rand() % 10;
            printf("Exit code: %d\n", num);
            return num;
        }
    }

    mean /= safe;

    printf("Média: %f\n", mean);
}