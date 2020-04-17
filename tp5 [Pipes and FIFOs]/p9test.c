#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

int main(void){
    write(STDOUT_FILENO, "Hello world!\n", strlen("Hello world!\n"));
}