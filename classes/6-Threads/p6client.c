#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>

int main(int argc, char* argv[]){
    int fd;

    if (argc != 3){
        printf("Usage: %s number1 number2\n", argv[0]); //Tested with ./test
        exit(3);
    }
    
    char fifo[100];
    sprintf(fifo, "/tmp/fifo_ans_%d", getpid());
    char numbers[100];
    sprintf(numbers, "%s %s %s", argv[1], argv[2], fifo);

    do {
        fd=open("/tmp/fifo_req", O_WRONLY);
        if (fd==-1) sleep(1);
    } while (fd==-1);

    write(fd, numbers, strlen(numbers));
    if (atoi(argv[1]) == 0 && atoi(argv[2]) == 0){
        close(fd); 
        unlink(fifo);
        exit(0);
    } 
    
    mkfifo(fifo,0660);
    int fd2=open(fifo ,O_RDONLY);

    char* buffer = malloc(200);
    while(read(fd2, buffer, 200) == 0){} //-> only receiving command results with size of 200 chars

    printf("%s\n", buffer);

    free(buffer);
    close(fd); close(fd2);
    unlink(fifo);
    exit(0);
}