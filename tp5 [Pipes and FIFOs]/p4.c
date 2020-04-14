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


// Execute:
//     -ls dir –laR; 
//     -grep arg;
//     -sort
// in pipeline.

int main(int argc, char* argv[]){

    int fd1[2], fd2[2];
    pipe(fd1);
    pipe(fd2);
    int status;


    pid_t pid1 = fork();

    if (pid1 == 0){
        close(fd1[READ]);
        close(fd2[WRITE]);
        close(fd2[READ]);
        dup2(fd1[WRITE], STDOUT_FILENO);
        execlp("ls", "ls", argv[1], "-laR", NULL);
    }
    
    pid_t pid2 = fork();

    if (pid2 == 0){
        close(fd1[WRITE]);
        close(fd2[READ]);
        dup2(fd1[READ], STDIN_FILENO);
        dup2(fd2[WRITE], STDOUT_FILENO);
        execlp("grep", "grep", argv[2], NULL);
    }

    pid_t pid3 = fork();

    if (pid3 == 0){
        close(fd2[WRITE]);
        close(fd1[WRITE]);
        close(fd1[READ]);
        dup2(fd2[READ], STDIN_FILENO);
        execlp("sort", "sort", NULL);
    } 

    wait(&status);

    return 0;
}