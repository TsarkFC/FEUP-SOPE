#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

#define BUFFER_SIZE 512

int main(int argc, char *argv[])
{
    int fd1, fd2, nr, nw;
    unsigned char buffer[BUFFER_SIZE];

    if (argc != 3) {
        printf("Usage: %s <file1> <file2>\n", argv[0]);
        return 1;
    }

    //OPEN FIRST FILE
    fd1 = open(argv[1], O_RDONLY);
    if (fd1 == -1) {
        perror(argv[1]);
        return 2;
    }

    //OPEN SECOND FILE
    fd2 = open(argv[2], O_RDONLY);
    if (fd2 == -1) {
        perror(argv[2]);
        return 2;
    }

    //DUP2
    dup2(fd2, fd1);

    //READ SOURCE AND SEND TO DESTINATION
    while ((nr = read(fd1, buffer, BUFFER_SIZE)) > 0)
        if ((nw = write(STDOUT_FILENO, buffer, nr)) <= 0 || nw != nr) {
        perror(argv[1]);
        close(fd1);
        return 4;
    }
        
    close(fd1);
    
    return 0;
}

