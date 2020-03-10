#include <stdio.h>
#include <unistd.h>
#include <string.h>

int main(){

    if (fork() > 0){ //grandfather
        write(STDOUT_FILENO, "Hello ", strlen("Hello "));
    }
    else{
        if (fork() > 0){ //father
            write(STDOUT_FILENO, "my ", strlen("my "));
        }
        else{ //son
            write(STDOUT_FILENO, "friends !\n", strlen("friends !\n"));
        }
    }


    return 0;
}