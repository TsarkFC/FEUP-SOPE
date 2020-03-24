#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <string.h>

#define READ 0
#define WRITE 1

int main(int argc, char* argv[]){

    if (argc != 2){
        printf("usage: %s textfile\n", argv[0]);
        exit(1);
    }

    int fd[2];
    pipe(fd);

    pid_t pid;

    int fileno = open(argv[1], O_RDONLY);

    char content[20];
    close(fd[WRITE]);
    while (read(fileno, content, 20)){
        write(fd[WRITE], content, 20);
        close(fd[WRITE]);
    }

    pid = fork();
    if (pid == 0){
        dup2(fileno, STDOUT_FILENO);
        //dup2(fd[WRITE], STDOUT_FILENO);
        execlp("sort", "sort", argv[1], NULL);
    }
    else if (pid > 0){
        printf("GOT PARENT\n");
        close(fd[WRITE]);
        write(STDOUT_FILENO, "TESTING...\n", strlen("TESTING...\n"));
        while (read(fd[READ], content, 20)){
            write(STDOUT_FILENO, content, 20);
            printf("GOT HERE\n");
        }
    } 


    return 0;
}