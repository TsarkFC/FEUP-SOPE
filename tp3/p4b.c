#include <stdio.h>
#include <unistd.h>
#include <string.h>

int main(void){
    //pai   -> Hello
    //filho -> world !

    if (fork() > 0){ //pai
        write(STDOUT_FILENO, "Hello ", strlen("Hello "));
    }
    else{ //filho
        write(STDOUT_FILENO, "world !\n", strlen("world !\n"));
    }
    return 0;
}