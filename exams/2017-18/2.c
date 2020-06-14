#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <time.h>
#include <fcntl.h>
#include <pthread.h>
#include <semaphore.h>
#include <pthread.h>
#include <sys/mman.h>

typedef struct{} PData;

PData pdata;
sem_t produce, consume;
void put(PData *x);
PData get();

void* t1(void* arg){
    sem_wait(&produce);
    put(&pdata);
    sem_post(&consume);
}

void* t2(void* arg){
    sem_wait(&consume);
    pdata = get();
    sem_post(&produce);
}

int main(void){
    sem_init(&produce, NULL, 1);
    sem_init(&consume, NULL, 0);
}