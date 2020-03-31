#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <string.h>

#define READ 0
#define WRITE 1
#define MAXLINE 100


// Execute:
//     -ls dir –laR; 
//     -grep arg;
//     -sort
//     -others
// in pipeline.

//Usage ./p5 dir string

int string_slasher(char* args, char** final){

    char* buffer = strtok(args, "|");

    int i = 0;
    while (buffer != NULL){
        final[i] = malloc(sizeof(buffer));
        strcpy(final[i], buffer);
        i++;
        buffer = strtok(NULL, "|");
    }
    return i;
}

void cmd_builder(char* cmd, char**final){
    char* buffer = strtok(cmd, " ");

    int i = 0;
    while (buffer != NULL){
        final[i] = malloc(sizeof(buffer));
        strcpy(final[i], buffer);
        i++;
        buffer = strtok(NULL, " ");
    }
    final[i] = NULL;
}


int main(int argc, char* argv[]){

    char* str[MAXLINE];
    int size = string_slasher(argv[1], str);
    printf("Size: %d\n", size);
    char* cmd[MAXLINE];

    int fd1[2], fd2[2];
    pipe(fd1);
    pipe(fd2);
    int status;

    pid_t pid1 = fork();

    if (pid1 == 0){
        cmd_builder(str[0], cmd);
        close(fd1[READ]);
        close(fd2[WRITE]);
        close(fd2[READ]);
        dup2(fd1[WRITE], STDOUT_FILENO);
        execvp(cmd[0], cmd);
    }

    for (int i = 1; i < size-1; i++){
        if (fork() == 0){
            if (i%2 == 0){
                close(fd2[WRITE]);
                close(fd1[READ]);
                dup2(fd2[READ], STDIN_FILENO);
                dup2(fd1[WRITE], STDOUT_FILENO);
                cmd_builder(str[i], cmd);
                execvp(cmd[0], cmd);
            }
            else{
                close(fd1[WRITE]);
                close(fd2[READ]);
                dup2(fd1[READ], STDIN_FILENO);
                dup2(fd2[WRITE], STDOUT_FILENO);
                cmd_builder(str[i], cmd);
                execvp(cmd[0], cmd);
            }
        }
    }

    pid_t pid3 = fork();

    if (pid3 == 0){
        if (size%2 != 0){
            cmd_builder(str[size-1], cmd);
            close(fd2[WRITE]);
            close(fd1[WRITE]);
            close(fd1[READ]);
            dup2(fd2[READ], STDIN_FILENO);
            execvp(cmd[0], cmd);
        }
        else{
            cmd_builder(str[size-1], cmd);
            close(fd1[WRITE]);
            close(fd2[WRITE]);
            close(fd2[READ]);
            dup2(fd1[READ], STDIN_FILENO);
            execvp(cmd[0], cmd);
        }
    } 

    wait(&status);

    return 0;
}