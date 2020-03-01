#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <ctype.h>

#define ENTER_ASCII 10

int main(int argc, char **argv){
    char ch;
    int enter_press = 0; //odd to names, even to numbers
    char array[50];
    int i = 0;

    int file = open(argv[1], O_WRONLY | O_CREAT | O_EXCL, 0644);

    while (read(STDIN_FILENO, &ch, 1) && ch != '\n') {
        if (ch != 10){
            array[i] = ch;
            i++;
        }
        else{
            enter_press++;
            if (enter_press % 2 == 0 && (!isdigit(array[0]) || !isdigit(array[1]))){
                printf("ERRNO: %d\n", errno);
                printf("Grade has to have numerical value! \n");
                exit(1);
            }
            write(file, array, sizeof(array));
            memset(array, 50, sizeof(char));
        }
        
    }
}