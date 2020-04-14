#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>

void handler(int signo){
    if (signo == SIGALRM){
        write(STDOUT_FILENO, "Suiciding...\n", strlen("Suiciding...\n"));
        sleep(10);
    }
}

int main(int argc, char* argv[]){
    struct sigaction action;
    action.sa_handler = handler;
    sigemptyset(&action.sa_mask);
    action.sa_flags = 0;
    
    if (sigaction(SIGALRM,&action,NULL) < 0){
        fprintf(stderr,"Unable to install SIGALRM handler\n");
        exit(1);
    }

    alarm(atoi(argv[1]));

    char pid[20];
    sprintf(pid, "%d", getpid());

    execlp("./prog", "./prog", argv[3], pid, NULL);

    write(STDOUT_FILENO, "Suiciding...\n", strlen("Suiciding...\n"));

    return 0;
}