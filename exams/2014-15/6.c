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

#define SEM_INI 0

struct Data{};

struct Data buffer[10];

sem_t sem;

void* thread1(void *arg){
    for (int k=0; k<10; k++) {
        fill(&buffer[k]);
        //c)
        sem_post(&sem):
    }
}

void* thread2(void *arg){
    for (int k=0; k<10; k++) {
        //c)
        sem_wait(&sem);
        process(&buffer[k]);
    }
}

int main(void){
    //a) Second option (shared memmory region) - need to pass as parameter
    // and cast in thread functions:
    //
    // strcut Data *buffer = mmap(NULL, 10*sizeof(struct Data), 
    //                     PROT_READ, PROT_WRITE, 
    //                     MAP_SHARED | MAP_ANONYMOUS, -1, 0);

    //b)
    sem_init(&sem, 0, SEM_INI);

    pthread_t tid[2];
    pthread_create(&tid[0], NULL, thread1, NULL);
    pthread_create(&tid[1], NULL, thread2, NULL);
    pthread_join(tid[0], NULL);
    pthread_join(tid[1], NULL);
}