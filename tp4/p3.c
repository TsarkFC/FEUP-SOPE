#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

void sigint_handler(int signo){
    printf("Handler!\n");
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

}