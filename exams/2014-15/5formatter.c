#include <stdio.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>

struct info{
    int filetype; //1, 2 or 100
    int inodenr;
    int blksize;
    char name[50];
};

int main(void){
    struct info info;
    char letter;
    while(read(STDIN_FILENO, &info, sizeof(info))){
        if (info.filetype == 100)
            printf("f %s %d %d\n", info.name, info.inodenr, info.blksize);
        else if (info.filetype == 1)
            printf("f %s\n", info.name);
        else if (info.filetype == 2)
            printf("d %s\n", info.name);
    }
    return 0;
}