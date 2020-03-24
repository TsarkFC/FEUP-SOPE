#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

#define READ 0
#define WRITE 1

int main(int argc, char *argv[]){
    int fd[2];
    int cd[2];
    int send[2];
    int receive[2];
    pid_t pid;
    
    pipe(fd);
    pipe(cd);

    pid = fork();

    //father
    if (pid > 0){
        printf("Parent reading from keyboard...\n");
        //read(STDIN_FILENO, send, sizeof(int)*2);
        scanf("%d %d", &send[0], &send[1]);

        printf("Value read: %d, %d\n", send[0], send[1]);

        write(fd[WRITE], send, sizeof(int)*2);
        close(fd[WRITE]);

        int s, m; float dF;

        close(cd[WRITE]);
        read(cd[READ], &s, sizeof(int));
        read(cd[READ], &m, sizeof(int));
        read(cd[READ], &dF, sizeof(float));

        printf("sum: %d\n", s);
        printf("mult: %d\n", m);
        printf("div: %fd\n", dF);
    }
    //child
    else if (pid == 0){
        close(fd[WRITE]);
        read(fd[READ], receive, sizeof(int)*2);
        printf("Number1: %d\nNumber2: %d\n", receive[0], receive[1]);

        //calculations
        int sum = receive[0] + receive[1];
        int mult = receive[0] * receive[1];
        float divFloat = (float)receive[0] / receive[1];

        write(cd[WRITE], &sum, sizeof(int));
        write(cd[WRITE], &mult, sizeof(int));
        write(cd[WRITE], &divFloat, sizeof(float));
        close(cd[WRITE]);
    }

    return 0;
}