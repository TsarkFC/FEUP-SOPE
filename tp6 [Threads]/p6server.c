#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <string.h>
#include <pthread.h>

typedef struct{
    int n1;
    int n2;
}operation;

void *Plus(void *calculator){
    char *res = malloc(100);
    operation *op = (operation *) calculator; 
    sprintf(res, "Result: %d + %d = %d\n", (*op).n1, (*op).n2, (*op).n1 + (*op).n2);
    return res;
}

void *Minus(void *calculator){
    char *res = malloc(100);
    operation *op = (operation *) calculator; 
    sprintf(res, "Result: %d - %d = %d\n", (*op).n1, (*op).n2, (*op).n1 - (*op).n2);
    return res;
}

void *Cross(void *calculator){
    char *res = malloc(100);
    operation *op = (operation *) calculator; 
    sprintf(res, "Result: %d * %d = %d\n", (*op).n1, (*op).n2, (*op).n1 * (*op).n2);
    return res;
}


void *Bars(void *calculator){
    char *res = malloc(100);
    operation *op = (operation *) calculator;
    if ((*op).n2 == 0){
        sprintf(res, "Division by 0!\n");
        return res;
    }  
    sprintf(res, "Result: %d / %d = %d\n", (*op).n1, (*op).n2, (*op).n1 / (*op).n2);
    return res;
}

int main(int argc, char* argv[]){
    int fd2;

    mkfifo("/tmp/fifo_req",0660);

    pthread_t threads[4];
    while(1){
        int fd=open("/tmp/fifo_req",O_RDONLY);
        char* rec = malloc(200);
        while(read(fd, rec, 200)==0){}


        char* cmd[100];
        char* begin = strtok(rec, " ");
        int i = 0;
        cmd[i] = malloc(strlen(begin));
        cmd[i] = begin;
        while (1){
            i++;
            begin = strtok(NULL, " ");
            if (begin == NULL) break;
            cmd[i] = malloc(strlen(begin));
            cmd[i] = begin;
        }

        operation *op = (operation*) malloc(sizeof(operation));
        char *res;

        (*op).n1 = atoi(cmd[0]);
        (*op).n2 = atoi(cmd[1]);
        if ((*op).n1 == 0 && (*op).n2 == 0) {
            close(fd);
            break;
        }

        pthread_create(&threads[0], NULL, Plus, (void *)op);
        pthread_create(&threads[1], NULL, Minus, (void *)op);
        pthread_create(&threads[2], NULL, Cross, (void *)op);
        pthread_create(&threads[3], NULL, Bars, (void *)op);

        do {
            fd2=open(cmd[2],O_WRONLY);
            if (fd2==-1) sleep(1);
        } while (fd2==-1);

        int t;
        char result[1024] = "";
        for(t=0; t< 4; t++){
            pthread_join(threads[t], (void*)&res); 
            strcat(result, res);
            free(res);   
        }
        write(fd2, result, strlen(result));
        free(op); 

        close(fd);
        close(fd2);
    }
    unlink("/tmp/fifo.req");
    exit(0);
}