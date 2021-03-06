// PROGRAMA p02a.c
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void sigint_handler(int signo){
    if (signo == SIGTERM) printf("SIGTERM HANDLER!\n");
    else{
        printf("Entering SIGINT handler ...\n");
        sleep(10);
        printf("Exiting SIGINT handler ...\n");
    }
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

    if (sigaction(SIGTERM, &action, NULL < 0)){
        fprintf(stderr,"Unable to install SIGTERM handler\n");
        exit(1);
    }

    printf("Try me with CTRL-C ...\n");
    while(1) pause();
    exit(0);
} 