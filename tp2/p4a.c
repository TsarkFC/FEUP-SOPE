#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <ctype.h>

#define KEY 27
#define ENTER_KEY 10

int main(int argc, char **argv){
    char ch;
    int enter_press = 0; //odd to names, even to numbers

    int file = open(argv[1], O_WRONLY);

    char *init = "Write name of the student and grade intercalated with ENTER key press!\n\n";

    write(STDOUT_FILENO, init, strlen(init));

    while (read(STDIN_FILENO, &ch, 1) && ch != KEY) {
        if (ch != ENTER_KEY){
            if (enter_press % 2 == 1 && (!isdigit(ch))){
                printf("ERRNO: %d\n", errno);
                printf("Grade has to have numerical value! \n");
                exit(1);
            }

            write(file, &ch, 1);
        }
        else{
            enter_press++;
            if (enter_press % 2 != 0) write(file, ": ", 2);
            else write(file, "\n", 1);
        }
        
    }

    close(file);
}