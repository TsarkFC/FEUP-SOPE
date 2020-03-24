#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

#define READ 0
#define WRITE 1

int main(int argc, char *argv[]){
    int fd[2];
    int send[2];
    int receive[2];
    pid_t pid;
    
    pipe(fd);

    pid = fork();

    //father
    if (pid > 0){
        printf("Parent reading from keyboard...\n");
        //read(STDIN_FILENO, send, sizeof(int)*2);
        scanf("%d %d", &send[0], &send[1]);

        printf("Value read: %d, %d\n", send[0], send[1]);

        write(fd[WRITE], send, sizeof(int)*2);
        close(fd[WRITE]);
    }
    //child
    else if (pid == 0){
        close(fd[WRITE]);
        read(fd[READ], receive, sizeof(int)*2);
        printf("Number1: %d\nNumber2: %d\n", receive[0], receive[1]);

        //calculations
        printf("sum: %d\n", receive[0] + receive[1]);
        printf("mult: %d\n", receive[0] * receive[1]);
        printf("div: %fd\n", (float)receive[0] / receive[1]);
    }

    return 0;
}