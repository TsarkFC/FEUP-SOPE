#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>

const int NUM_WORKERS = 4;
pthread_t workers[NUM_WORKERS];
pthread_t loadBalancer;
void* dequeueJob();
void* lbEntry(void* p);
void* workerEntry(void* p);
int setupLB();
int setupFifos();
int setupWorkers();
int teardownWorkers();
int teardownLB();
int teardownFifos();
void listenHTTPrequests();

pthread_t tid;
pthread_t tids[NUM_WORKERS];

int main(int argc, const char * argv[]){
    if (setupFifos()) {
        printf("FIFOs setup failed\n");
        return 1;
    }
    if (setupWorkers()) {
        printf("Worker setup failed\n");
        return 2;
    }
    if (setupLB()) {
        printf("LB setup failed\n");
        return 3;
    }
    listenHTTPrequests();
    teardownWorkers();
    teardownLB();
    teardownFifos();
    return 0;
}

//a)
int setupFifos(){
    for (int i = 0; i < NUM_WORKERS; i++){
        char str[20];
        sprintf(str, "/tmp/myfifo%d", i+1);
        mkfifo(str, 0660);
    }
    return 0;
}

//b)
int setupLB(){
    return pthread_create(&loadBalancer, NULL, lbEntry, NULL);
}

//c)
int setupWorkers(){
    int ids[NUM_WORKERS];
    for (int i = 1; i < NUM_WORKERS+1; i++){
        ids[i] = i;
        pthread_create(&workers[i], NULL, workerEntry, (void*)&ids[i]);
    }
    return 0;
}

//d)
void* workerEntry(void* p){
    //1
    int *id = (int*) p;
    char fifoname[20];
    sprintf(fifoname, "/tmp/myfifo%d", *id);
    int fd = open(fifoname, O_RDONLY);

    //2
    char buffer[100];
    char filename[20];
    sprintf(filename, "worker%d.log", *id);
    int write_file = open(filename, O_WRONLY | O_APPEND | O_CREAT, 0600);

    //3
    while (readline(fd, buffer)){
        write(write_file, buffer, strlen(buffer)+1);
    }
}

int readline(int fd, char *str){
    int n;
    do {
        n = read(fd,str,1);
    } while (n>0 && *str++ != '\0');
    return (n>0);
}

//e)
int teardownWorkers(){
    for (int i = 1; i < NUM_WORKERS+1; i++){
        pthread_join(workers[i], NULL);
    }
    return 0;
}
int teardownLB(){
    return pthread_join(loadBalancer, NULL);
}

