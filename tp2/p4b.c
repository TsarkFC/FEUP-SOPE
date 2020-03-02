#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <ctype.h>

#define KEY 27
#define ENTER_KEY 10

struct student{
    char name[50];
    int grade;
};

int main(int argc, char **argv){
    char ch;
    int enter_press = 0; //odd to names, even to numbers
    int i = 0;

    int file = open(argv[1], O_WRONLY);

    struct student stu;
    memset(stu.name, ' ', 50);
    stu.grade = 0;

    while (read(STDIN_FILENO, &ch, 1) && ch != KEY) {
        if (ch != ENTER_KEY){
            if (enter_press % 2 == 1 && (!isdigit(ch))){
                printf("ERRNO: %d\n", errno);
                printf("Grade has to have numerical value! \n");
                exit(1);
            }
            if (enter_press % 2 == 0){
                stu.name[i] = ch;
                i++;
            }
            else {
                stu.grade = stu.grade * 10 + (ch - '0');
            }
        }
        else{
            enter_press++;
            if (enter_press % 2 == 0) {
                write(file, &stu, sizeof(struct student));
                write(file, "\n", 1);
                
                i = 0;
                stu.grade = 0;
                memset(stu.name, ' ', 50);
            }
        }
        
    }

    close(file);
}