#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <string.h>
#include <time.h>
#include <fcntl.h>
#include <pthread.h>
#include <semaphore.h>

int aposta, aposta_feita = 0;
int face, numero_sorteado = 0;
int numero_jogadas = 0;
int apostas_ganhas = 0;
int apostas_perdidas = 0;

//used in exercise solutions(?)
pthread_cond_t cond1 = PTHREAD_COND_INITIALIZER;
pthread_cond_t cond2 = PTHREAD_COND_INITIALIZER;
pthread_cond_t cond3 = PTHREAD_COND_INITIALIZER;

pthread_mutex_t m1 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t m2 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t m3 = PTHREAD_MUTEX_INITIALIZER;

sem_t sem1;
sem_t sem2;

//a1
void *t_aposta(void *arg){
    while (1){
        while (aposta_feita);
            //pthread_cond_wait(&cond1, &m1);

        do{
            printf("Inserir numero (1-6): \n");
            scanf("%d", &aposta);
        }
        while (aposta < 1 || aposta > 6);

        aposta_feita = 1;
    }
    pthread_exit(0);
}

//a2
void *t_roda(void *arg){
    srand(time(NULL));
    while (1){
        while (numero_sorteado);
            //pthread_cond_wait(&cond2, &m2);

        face = rand() % 6 + 1;

        numero_sorteado = 1;
    }
    pthread_exit(0);
}

//a3
void *t_compara(void *arg){
    while(1){
        while(numero_sorteado != 1 || aposta_feita != 1);
            //pthread_cond_wait(&cond3, &m3);

        sem_wait(&sem1);
        
        if (face == aposta) 
            apostas_ganhas += 1;
        else 
            apostas_perdidas += 1;
        numero_jogadas += 1;

        aposta_feita = 0;
        numero_sorteado = 0;

        sem_post(&sem2);
    }
    pthread_exit(0);
}

//a4
void *t_mostra(void *arg){
    while (1){
        sem_wait(&sem2);
        printf("Numero de jogadas: %d\nApostas ganhas: %d\nApostas perdidas: %d\n\n", numero_jogadas, apostas_ganhas, apostas_perdidas);
        sleep(1);
        sem_post(&sem1);
    }
    pthread_exit(0);
}

//a5
int main(){
    pthread_t threads[4];

    sem_init(&sem1, 0, 1);
    sem_init(&sem2, 0, 0);

    pthread_create(&threads[0], NULL, t_aposta, NULL);
    pthread_create(&threads[1], NULL, t_roda, NULL);
    pthread_create(&threads[2], NULL, t_compara, NULL);
    pthread_create(&threads[3], NULL, t_mostra, NULL);

    for(int t=0; t<4; t++)
        pthread_join(threads[t], NULL);

    pthread_exit(0);
}
