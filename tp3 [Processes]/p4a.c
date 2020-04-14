#include <stdio.h>
#include <unistd.h>
#include <string.h>

int main(void){
    //filho -> Hello
    //pai   -> world !

    if (fork() > 0){ //pai
        printf("world !\n");
    }
    else{ //filho
        write(STDOUT_FILENO, "Hello ", strlen("Hello "));
    }
    return 0;
}