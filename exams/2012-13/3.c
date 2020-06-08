#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>

int redirectStdin(char* filename){
    int fd = open(filename, O_RDWR);
    if (fd == -1){
        perror("Openning file\n");
        return -1;
    }

    int dup_fd = dup2(STDIN_FILENO, fd);

    if (dup_fd == -1){
        perror("Redirect stdin\n");
        return -1;
    }

    return 0;
}

int main(){
    int numValuesRead, value;

    if (redirectStdin("infile.txt") < 0){
        perror("redirectStdin");
        exit(1);
    }
    numValuesRead = scanf("%d", &value);
    if (numValuesRead == 1){
        printf("'value' = %d was read from 'infile.txt'\n", value);
        exit(0);
    }
    else{
        printf("Error reading 'value' from 'infile.txt'\n");
        exit(1);
    }
}