#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>

int time = 30;

void sigint_handler(int signo){
    printf("In handler ...\n");
}

int main(void){
    struct sigaction action;
    
    action.sa_handler = sigint_handler;
    sigemptyset(&action.sa_mask);
    action.sa_flags = 0;

    if (sigaction(SIGINT,&action,NULL) < 0){
        fprintf(stderr,"Unable to install SIGINT handler\n");
        exit(1);
    }

    if (sigaction(SIGUSR1,&action,NULL) < 0){
        fprintf(stderr,"Unable to install SIGUSR1 handler\n");
        exit(1);
    }

    printf("Sleeping for 30 seconds ...\n");

    while(time != 0){
        time = sleep(time);
    }
    
    printf("Waking up ...\n");
    exit(0);
} 
