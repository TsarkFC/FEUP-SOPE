// PROGRAMA p02.c
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>
#define STDERR 2
#define NUMITER 10000

int n = 30;

void * thrfunc(void * arg){
    int *i = (int *)malloc(sizeof(int));
    *i = 0;
    fprintf(stderr, "Starting thread %s\n", (char *) arg);
    while (n>1){
        write(STDERR,arg,1);
        n--;
        (*i)++;
    }
    return i;
}

int main(){
    pthread_t ta, tb;
    char one = '1';
    char two = '2';

    int *res1;
    int *res2;
    pthread_create(&ta, NULL, thrfunc, &one);
    pthread_create(&tb, NULL, thrfunc, &two);
    pthread_join(ta, (void*)&res1);
    pthread_join(tb, (void*)&res2);

    printf("\nResult 1: %d\n", *res1);
    printf("Result 2: %d\n", *res2);
    return 0;
}