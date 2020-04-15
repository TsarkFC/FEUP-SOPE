#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>


int main(void){

    mkfifo("/tmp/fifo_req",0660);
    mkfifo("/tmp/fifo_ans",0660);

    int req;
    int ans;

    req=open("/tmp/fifo_req",O_RDONLY);
    ans=open("/tmp/fifo_ans",O_WRONLY);

    printf("Server openned FIFO's\n");

    float n1, n2;
    do{
        char* rec = malloc(200);
        read(req, rec, 200);
        sscanf(rec, "%f %f", &n1, &n2);
        free(rec);

        if (n1 == 0 && n2 == 0) break;

        char* res = malloc(256);
        sprintf(res, "%f x %f = %f\n%f / %f = %f\n%f + %f = %f\n", n1, n2, n1*n2, n1, n2, n1/n2, n1, n2, n1+n2);
        write(ans, res, strlen(res));
        free(res); 
 
    } while(1);

    close(req);
    close(ans);
    unlink("/tmp/fifo_req");
    unlink("/tmp/fifo_ans");
    exit(0);
}