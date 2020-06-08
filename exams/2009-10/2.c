#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <pthread.h>

void *PrintHello(void *threadnum) {
    printf("Hello from thread no. %d!\n", *(int *) threadnum);
    pthread_exit(NULL);
}
int main() {
    int t;
    int num[7]; 
    pthread_t tid[7];
    for(t=0; t< 7; t++){
        num[t] = t;
        printf("Creating thread %d\n", t);
        pthread_create(&tid[t], NULL, PrintHello, (void *)&num[t]);
    }
    pthread_exit(0);
}