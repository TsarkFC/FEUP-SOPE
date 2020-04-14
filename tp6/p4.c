// PROGRAMA p04.c
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#define NUM_THREADS 10

void *PrintHello(void *threadnum){
    sleep(1);
    pthread_t tid = pthread_self();
    printf("TID: %lu\n", (long unsigned)tid);
    return threadnum;
}

int main(){
    pthread_t threads[NUM_THREADS];
    int t;
    int* res;
    for(t=0; t< NUM_THREADS; t++){
        pthread_create(&threads[t], NULL, PrintHello, (void *)&t);       
    }
    for(t=0; t< NUM_THREADS; t++){
        pthread_join(threads[t], (void*)&res);
        printf("Got thread number: %d\n", *res);        
    }
    pthread_exit(0);
}