#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

#define READ 0
#define WRITE 1

int main(int argc, char *argv[]){
    int fd[2];
    char send[2][20];
    char receive[2][20];
    pid_t pid;
    
    pipe(fd);

    pid = fork();

    //father
    if (pid > 0){
        printf("Parent reading from keyboard...\n");
        //read(STDIN_FILENO, send, sizeof(int)*2);
        scanf("%s", send[0]);
        scanf("%s", send[1]);

        printf("Value read: %s, %s\n", send[0], send[1]);

        write(fd[WRITE], send, 40);
        close(fd[WRITE]);
    }
    //child
    else if (pid == 0){
        close(fd[WRITE]);
        read(fd[READ], receive, 40);
        int n1 = atoi(receive[0]), n2 = atoi(receive[1]);
        printf("Number1: %d\nNumber2: %d\n", n1, n2);

        //calculations
        printf("sum: %d\n", n1 + n2);
        printf("mult: %d\n", n1 * n2);
        printf("div: %fd\n", (float)n1 / n2);
    }

    return 0;
}