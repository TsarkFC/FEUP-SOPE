#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>

int main(void){
    int req;
    int ans;
    int n1 = 0, n2 = 0;
    
    do {
        req=open("/tmp/fifo_req",O_WRONLY);
        if (req==-1) sleep(1);
    } while (req==-1);
    do {
        ans=open("/tmp/fifo_ans",O_RDONLY);
        if (ans==-1) sleep(1);
    } while (ans==-1);

    do{
        char* no1 = malloc(100);
        char* no2 = malloc(100);
        char* send = malloc(200);
        write(STDOUT_FILENO, "NUMBER1: ", strlen("NUMBER1: "));
        read(STDOUT_FILENO,no1,100);
        write(STDOUT_FILENO, "NUMBER2: ", strlen("NUMBER2: "));
        read(STDOUT_FILENO,no2,100);

        sprintf(send, "%s %s", no1, no2);
        n1 = atof(no1);free(no1);
        n2 = atof(no2);free(no2);

        if (n1 == 0 && n2 == 0){
            free(send);
            break;
        }

        write(req, send, strlen(send));
        free(send);

        char* res = malloc(100);
        read(ans, res, 100);
        printf("%s", res);
        free(res);
        
    }while(1);

    exit(0);
}