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

info_t get_info();
int C = 0;

//b) / c)
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
void* t2(void* arg){
    //d)
    pthread_mutex_lock(&mutex);

    while(!C)
        pthread_cond_wait(&cond, &mutex);

    //B

    pthread_mutex_unlock(&mutex);

}

void* t1(void* arg){
    //a)
    pthread_t tid;
    info_t* info = malloc(sizeof(info_t));
    *info = get_info();
    pthread_create(&tid, NULL, t2, (void*)info);

    //d)
    pthread_mutex_lock(&mutex);
    //change conditional A
    pthread_cond_signal(&cond); //e) -> pthread_cond_broadcast(&cond);
    pthread_mutex_unlock(&mutex);
}

int main(void){

}