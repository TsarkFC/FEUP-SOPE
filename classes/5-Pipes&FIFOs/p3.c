#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <string.h>

#define READ 0
#define WRITE 1
#define MAXLINE 42

int main(int argc, char* argv[]){

    if (argc != 2){
        printf("usage: %s textfile\n", argv[0]);
        exit(1);
    }

    int fd[2];
    pipe(fd);

    pid_t pid;
    int status, n;

    char content[MAXLINE];

    int fileno = open(argv[1], O_RDWR);

    pid = fork();
    if (pid == 0){
        close(fd[READ]);
        dup2(fd[WRITE], STDOUT_FILENO);
        execlp("sort", "sort", argv[1], NULL);
    }
    else if (pid > 0){
        waitpid(-1, &status, WUNTRACED);
        close(fd[WRITE]);
        while (n = read(fd[READ], content, MAXLINE)){
            write(STDOUT_FILENO, content, MAXLINE);
        }
    }

    close(fileno); 

    return 0;
}