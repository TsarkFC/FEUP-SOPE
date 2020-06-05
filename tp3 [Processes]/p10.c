// PROGRAMA p10.c
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>

int main(int argc, char *argv[], char *envp[])
{
    pid_t pid;
    
    if (argc != 3) {
        printf("usage: %s dirname filename\n",argv[0]);
        exit(1);
    }

    pid=fork();

    if (pid > 0) printf("My child is going to execute command \"ls -laR %s\"\n", argv[1]);
    else if (pid == 0){
        
        char cmd[20];
        int file;

        file = open(argv[2], O_WRONLY | O_CREAT, 0666);

        /*Easy way of doing without redirecting
        sprintf(cmd, "ls -laR %s > %s", argv[1], argv[2]);
        system(cmd);*/

        dup2(file ,STDOUT_FILENO);

        execlp("ls", "ls", "-laR", argv[1], NULL);

        close(file);

    }
    exit(0);
} 