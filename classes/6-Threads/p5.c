// PROGRAMA p05.c
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#define NUM_THREADS 4

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
    sprintf(res, "Result: %d / %d = %d\n", (*op).n1, (*op).n2, (*op).n1 / (*op).n2);
    return res;
}




int main(){
    pthread_t threads[NUM_THREADS];
    char *res;
    char number1[50], number2[50];

    operation *op = (operation*) malloc(sizeof(operation)); 

    write(STDOUT_FILENO, "Number 1: ", strlen("Number 1: "));
    read(STDOUT_FILENO, number1, 50);
    (*op).n1 = atoi(number1);
    write(STDOUT_FILENO, "Number 2: ", strlen("Number 2: "));
    read(STDOUT_FILENO, number2, 50);
    (*op).n2 = atoi(number2);

    pthread_create(&threads[0], NULL, Plus, (void *)op);
    pthread_create(&threads[1], NULL, Minus, (void *)op);
    pthread_create(&threads[2], NULL, Cross, (void *)op);
    pthread_create(&threads[3], NULL, Bars, (void *)op);

    int t;
    for(t=0; t< NUM_THREADS; t++){
        pthread_join(threads[t], (void*)&res);
        printf("%s\n", res);     
        free(res);   
    }
    free(op);
    pthread_exit(0);
}