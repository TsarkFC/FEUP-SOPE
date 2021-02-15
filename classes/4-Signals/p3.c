#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>


static int v = 0;
static int growth = 1;

void sigint_handler(int signo){
    printf("Handler!\n");
    if (signo == SIGTERM) {
        write(STDOUT_FILENO, "Shutting down...\n", strlen("Shutting down...\n"));
        exit(0);
    }
    else if (signo == SIGUSR1) growth = 1;
    else if (signo == SIGUSR2) growth = 0;
}

int main(void){
    struct sigaction action;
    action.sa_handler = sigint_handler;
    sigemptyset(&action.sa_mask);
    action.sa_flags = 0;

    if (sigaction(SIGUSR1,&action,NULL) < 0){
        fprintf(stderr,"Unable to install SIGUSR1 handler\n");
        exit(1);
    }
    if (sigaction(SIGUSR2,&action,NULL) < 0){
        fprintf(stderr,"Unable to install SIGUSR2 handler\n");
        exit(1);
    }
    if (sigaction(SIGTERM,&action,NULL) < 0){
        fprintf(stderr,"Unable to install SIGTERM handler\n");
        exit(1);
    }

    while (1){
        //printf("WHY?\n");
        sleep(1);
        if (growth) v++;
        else v--;
        printf("Number: %d\n", v);
    }
    return 0;
}