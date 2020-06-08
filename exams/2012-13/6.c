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

#define N 100 // buffer size
char buffer[N];
int ri = 0; // read index
int wi = 0; // write index
pthread_mutex_t mut = PTHREAD_MUTEX_INITIALIZER;
sem_t canIread, canIwrite;

int open_in_fifo(char* fifoName) {
    int fd = open(fifoName, O_RDONLY | O_NONBLOCK);
    if (fd < 0)
        pthread_exit(0);
    return fd;
}
void* receive(void* p) {
    int f = open_in_fifo(p);
    char c;
    while(read(f,&c,1)){
        printf("Read from FIFO %s the value %d\n", (char*)p, c);

        //c) Permite que não hajam threads a alterar o buffer ao mesmo tempo
        pthread_mutex_lock(&mut);
        buffer[wi++] = c;
        wi %= N;
        pthread_mutex_unlock(&mut);
        //--

        if (wi == 0){
            sem_post(&canIwrite);
            sem_wait(&canIread);
        }
    }
    close(f);
}

int open_out_fifo(char* fifoName) {
    int fd = open(fifoName, O_WRONLY | O_NONBLOCK);
    if (fd < 0)
        pthread_exit(0);
    return fd;
}

void* send(void* p) {
    int f = open_out_fifo(p);
    for(;;){
        if (ri == 0){
            sem_wait(&canIwrite);
        }
        printf("Write to FIFO %s the value %d\n", (char*)p, buffer[ri]);
        write(f,&buffer[ri++],1);
        ri %= N;

        if (ri == 0){
            sem_post(&canIread);
        }
        
    }
    close(f);
}

void init_semaphores() {
    sem_init(&canIread, 0, 0);
    sem_init(&canIwrite, 0, 0);
}

void destroy_semaphores() {
    sem_destroy(&canIread);
    sem_destroy(&canIwrite);
}
int main(int argc, char** argv) {
    pthread_t st; // send thread
    pthread_t* rt; // receive threads
    if (argc < 3){
        perror("Usage ./6 f1 [f2 [..[fn]]] fout\n");
        exit(1);
    }

    init_semaphores();

    //a)
    rt = malloc(sizeof(pthread_t)*(argc-2));
    pthread_t* safe = rt;
    pthread_t* safe2 = rt;
    for (int i = 1; i < argc-1; i++)
        pthread_create(rt++, NULL, receive, (void*)argv[i]);
    pthread_create(&st, NULL, send, (void*)argv[argc-1]);

    for (int i = 0; i < argc-2; i++){
        pthread_join(*safe, NULL);
        safe++;
    }
    pthread_join(st, NULL);
    //--

    free(safe2);
    destroy_semaphores();

    return 0;
}