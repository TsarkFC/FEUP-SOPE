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

struct Request {
  pid_t pid;
  size_t nbytes;
  char data[1000];
};

struct Reply {
  size_t nbytes;
  char data[1000];
};

void createRequest(const char *filename,  
                   struct Request *req)  {
    int file = open(filename, O_RDONLY);
    char buffer[BUF_SIZE]; // BUF_SIZE < 1000
    size_t n;
    req->pid = getpid(); req->nbytes = 0;
    while ((n = read(file, buffer, BUF_SIZE)) > 0) {
        memcpy(&req‐>data[req->nbytes], buffer, n);
        req->nbytes += n;
    }
    close(file);
}

int main(int argc, char *argv[]){
    mkfifo("requests", 0660);
    char fifoname[50];
    sprintf(fifoname, "ans%d", getpid());
    mkfifo(fifoname, 0600);

    int req = open("requests", O_WRONLY);
    int ans = open(fifoname, O_RDONLY);

    pid_t pid = fork();

    struct Request request;
    createRequest(argv[1], &request);
    write(req, &request, sizeof(request));

    if (pid > 0){
        int status;
        wait(&status);
        struct Reply reply;
        read(ans, &reply, sizeof(reply));
        write(STDOUT_FILENO, reply.data, reply.nbytes);
    }
    else if (pid == 0){
        execl("./compressF", "./compressF", NULL);
    }

    close(req);
    close(ans):
    unlink("requests");
    unlink(fifoname);

    return 0;
}