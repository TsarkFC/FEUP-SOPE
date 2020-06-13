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

sem_t sem;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void* receiver(void* arg){
    // MAX_NUM_PHONE_CALLS: constante global que indica o no máx. total de chamadas
    while (numPhoneCalls < MAX_NUM_PHONE_CALLS) {
        // espera até que chegue uma chamada telefónica e "anota" o número de telefone
        int phoneNumber = receiveCall();
        pthread_mutex_lock(&mutex);
        numPhoneCalls++;
        pthread_mutex_unlock(&mutex);
        if (numPhoneCalls % 1000 == 0) {
            // o concorrente cujo número de telefone é phoneNumber é um dos vencedores
            // informa o thread caller que deve fazer uma chamada para phoneNumber
            sem_post(&sem);
        }
    }
}

void* caller(void* arg){}

int main(void){
    sem_init(&sem, NULL, 0);
    // a)
    pthread_t cal;
    pthread_t rec[10];
    int num[10];
    for (int i = 0; i<10; i++){
        num[i] = i+1;
        pthread_create(&rec[i], NULL, receiver, (void*)&num[i]);
    }
    pthread_create(&cal, NULL, caller, NULL);

    for (int i = 0; i<10; i++)
        pthread_join(rec[i], NULL);
    pthread_join(cal, NULL);
}
