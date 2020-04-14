#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

#define READ 0
#define WRITE 1

struct pp{
    int number1;
    int number2;
};

int main(int argc, char *argv[]){
    int fd[2];
    struct pp packet;
    int receive[2];
    pid_t pid;
    
    pipe(fd);

    pid = fork();

    //father
    if (pid > 0){
        printf("Parent reading from keyboard...\n");
        //read(STDIN_FILENO, send, sizeof(int)*2);
        scanf("%d %d", &packet.number1, &packet.number2);

        printf("Value read: %d, %d\n", packet.number1, packet.number2);

        write(fd[WRITE], &packet, sizeof(struct pp));
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