#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <time.h>


int main(int argc, char* argv[]){

    if (argc != 2){
        printf("Usage: %s time\n", argv[0]);
        exit(3);
    }

    clock_t begin = clock();
    int final = atoi(argv[1]);

    mkfifo("/tmp/fifo_chg",0660);

    int fd;
    int msgno = 0;

    fd=open("/tmp/fifo_chg",O_RDONLY);

    do{
        char* rec = malloc(200);
        int time_passed = 0;
        while(read(fd, rec, 200)==0){
            if ((double)(clock() - begin)/CLOCKS_PER_SEC >= final){
                time_passed = 1;
                break;
            }
        }
        if (time_passed) break;
        msgno++;
        printf("CHEGOU: %s\n", rec);
        free(rec);
    } while((double)(clock() - begin)/CLOCKS_PER_SEC < final);

    printf("TOTAL: %d\n", msgno);

    close(fd);
    unlink("/tmp/fifo_chg");
    exit(0);
}