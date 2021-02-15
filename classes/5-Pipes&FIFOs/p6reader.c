// PROGRAMA p06_reader.c
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>

int readline(int fd, char *str);
int main(void){
    int fd;
    char str[100];
    mkfifo("/tmp/myfifo",0660);
    fd=open("/tmp/myfifo",O_RDONLY); //With O_RDWR p6reader does not end after p6writer has closed file; (you can still write to file) 
    while(readline(fd,str)) printf("%s",str);
    close(fd);
    return 0;
}
int readline(int fd, char *str){
    int n;
    do {
    n = read(fd,str,1);
    } while (n>0 && *str++ != '\0');
    return (n>0);
}