#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <string.h>


int main(int argc, char* argv[]){
    int fd2;

    mkfifo("/tmp/fifo.s",0660);

    int fd=open("/tmp/fifo.s",O_RDONLY);
    int dest=open("dest.txt", O_WRONLY | O_CREAT,  S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);

    char* rec = malloc(200);
    while(read(fd, rec, 200)==0){}

    char* cmd[100];
    char* begin = strtok(rec, " ");
    int i = 0;
    cmd[i] = malloc(strlen(begin));
    cmd[i] = begin;
    while (1){
        i++;
        begin = strtok(NULL, " ");
        if (begin == NULL) break;
        cmd[i] = malloc(strlen(begin));
        cmd[i] = begin;
    }

    int status;
    if (fork() == 0) {
        dup2(dest, STDOUT_FILENO);
        execvp(cmd[0], cmd);
    }
    else  wait(&status);
    free(rec);

    do {
        fd2=open("/tmp/fifo.pid",O_WRONLY);
        if (fd2==-1) sleep(1);
    } while (fd2==-1);

    close(dest);
    int send = open("dest.txt", O_RDONLY);
    char* buffer = malloc(200);
    read(send, buffer, 200);
    write(fd2, buffer, strlen(buffer));

    close(fd);
    close(send);
    close(fd2);
    unlink("/tmp/fifo_s");
    exit(0);
}