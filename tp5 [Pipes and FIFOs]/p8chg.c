#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>

int main(int argc, char* argv[]){
    int fd;

    if (argc != 2){
        printf("Usage: %s name\n", argv[0]);
        exit(3);
    }

    do {
        fd=open("/tmp/fifo_chg",O_WRONLY);
        if (fd==-1) sleep(1);
    } while (fd==-1);

    write(fd, argv[1], strlen(argv[1]));

    exit(0);
}