#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <errno.h>
#include <linux/limits.h>    
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <fcntl.h>
#define BUF_SIZE 999;
#define READ 0
#define WRITE 1

void full_copy(int from, int to) {
    char buffer[BUF_SIZE]; // BUF_SIZE < 1000
    size_t n;
    while ((n = read(from, buffer, BUF_SIZE)) > 0)
        write(to, buffer, n);
    close(from);
    close(to);
}

int main(int argc, char *argv[]){
    int fd[2];
    pipe(fd);

    pid_t pid = fork();

    if (pid > 0){
        close(fd[READ]);
        int file = open(argv[1], O_RDONLY);
        full_copy(file, fd[WRITE]);
    }
    else if (pid == 0){
        close(fd[WRITE]);
        dup2(fd[READ], STDIN_FILENO);
        execl("./compress", "./compress", NULL);
    }
    return 0;
}