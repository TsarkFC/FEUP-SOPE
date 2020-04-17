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
        printf("Usage: %s comand\n", argv[0]); //Tested with ./test
        exit(3);
    }

    do {
        fd=open("/tmp/fifo.s",O_WRONLY);
        if (fd==-1) sleep(1);
    } while (fd==-1);
    write(fd, argv[1], strlen(argv[1]));
    
    mkfifo("/tmp/fifo.pid",0660);
    int fd2=open("/tmp/fifo.pid" ,O_RDONLY);

    char* buffer = malloc(200);
    while(read(fd2, buffer, 200) == 0){} //-> only receiving command results with size of 200 chars

    printf("Command result: %s\n", buffer);

    free(buffer);
    close(fd); close(fd2);
    unlink("/tmp/fifo.pid");
    exit(0);
}