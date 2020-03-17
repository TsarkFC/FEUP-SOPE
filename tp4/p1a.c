// PROGRAMA p01a.c
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>

int time = 30;

void sigint_handler(int signo){
    printf("In handler ...\n");
}

int main(void){
    if (signal(SIGUSR1,sigint_handler) < 0){ 
        fprintf(stderr,"Unable to install SIGINT handler\n");
        exit(1);
    }

    if (signal(SIGUSR1,sigint_handler) < 0){
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