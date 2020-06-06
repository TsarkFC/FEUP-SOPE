#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <string.h>
#include <time.h>
#include <fcntl.h>

struct message {
    char str[9];
    pid_t pid;
    char carater;
}

int main(){
    //a)
    struct message msg;
    msg.pid = getpid();
    msg.str = "mkhead";
    msg.carater = 'B';

    int fda = open("/tmp/fifo.serv", O_WRONLY);

    write(fda, &msg, sizeof(msg));
    close(fda);

    //b)
    pid_t pid = getpid();
    char str[9] = "mkhead";
    char carater = 'B';

    int fdb = open("/tmp/fifo.serv", O_WRONLY);

    write(fdb, &pid, sizeof(pid_t));
    write(fdb, str, strlen(str));
    write(fdb, &carater, 1);
    close(fdb);
}