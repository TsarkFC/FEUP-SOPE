# 2016-17

## 1

- **DMA** - é um requisito de hardware para multiprogramação, permitindo conciliar I/O burst de um processo com CPU burst de outro.

- **Test&Set** - instrução utilizada pelos semáforos. Responsável por selecionar po próximo processo a executar a secção crítica.

- **bit de página válida/inválida** - utilizado na paginação a pedido. Cada entrada da tabela de páginas tem um bit que indica se a página está ou não em memória.

## 2

- a) 
    ```c
    sem_t sem1;
    sem_t sem2;
    sem_t sem3;

    init(sem1, 0);
    init(sem2, 0);
    init(sem3, 0);

    int p1(){
        signal(sem1);
        signal(sem1);
        wait(sem2);
        wait(sem3);
    }

    int p2(){
        signal(sem2);
        signal(sem2);
        wait(sem1);
        wait(sem3);
    }

    int p3(){
        signal(sem3);
        signal(sem3);
        wait(sem1);
        wait(sem2);
    }
    ```

- b) 
    - Não há exclusão mútua, apenas são usados recursos partilháveis.
    - Não há espera circular.

## 3

**"Um escalonamento do processador do tipo preemptivo favorece os processos CPU-bound."**
- Preentivo - recursos podem ser retirados a um processo. Processos com CPU bursts muito pequenos não esgotam o seu "quantum" podendo ser-lhes retirado o processador.
- CPU-bound - passa a maior parte do tempo a usar CPU, podendo ter CPU bursts muito longos.
- A afirmação assim **verifica-se**. Um processo CPU-bound irá gastar o seu "quantum" sendo imediatamente colocado na fila de prioridade.

## 4

- a) 
    - 0000010 001111101 = 1024 + 125
    - 512 Bytes/página  ->  página 2, offset 125

    - Página carregada no frame 15;
    - End físico: 15*512 + 125 = 7805 = 0001111 001111101

- b)
    - **Thrashing** acontece quando um processo passa mais tempi em atividades de paginação do que a executar.
    - **Soluções**:
        - Fazer swap out de um ou mais processos.
        - Retomar a execução quando houver mais memória livre.
    - **Evitar o thrashing**:
        - É possível recorrendo, por exemplo, a uma das duas estratégias referidas:
            
            - Estratégia dos conjuntos de trabalho. 
            - Estratégia da frequência de falta de página.

## 5

- a) Ao abrir em modo O_APPEND, a informações que cada processo pretende escrever serão acrescentadas àquilo que o ficheiro já contém. Em modo O_WRONLY o ficheiro é escrito "do inicio" cada vez que é aberto.

- b) Pode-se deslocar-se a posição do cursor do ficheiro de texto para a posição pretendida sempre que o ficheiro é aberto em modo O_WRONLY. 

## 6

- a) 
    ```c
    // CÓDIGO DE gen_seq
    int main(int argc, char *argv[]) {
        int numbits = atoi(argv[1]);
        // aloca memória e inicializa-a a zero
        char *bits = calloc(numbits + 1, sizeof(char));
        for (int i = 1; i <= numbits; ++i) {
            if (fork() == 0)
                execl("/home/sope/gen_bit", "/home/sope/gen_bit", NULL);
            else {
                int ret;
                wait(&ret);
                strcat(bits, ret == 0 ? "0" : "1");
            }
        }
        printf("%s\n", bits);
        return 0;
    }
    ```

- b) 
    - ```c
        // CÓDIGO DE gen_bit
        int main() {
            return (getpid() % 2);
        }
        ```
    - Com o código apresentado as sequências geradas terão sempre cadeias em que uns e zeros se encontram alternados, começando com 0 caso o primeiro processo tenha PID par, com 1 caso contrário.

- c)
    -  ```c
        // CÓDIGO DE gen_seq
        int main(int argc, char *argv[]) {
            int numbits = atoi(argv[1]);
            // aloca memória e inicializa-a a zero
            char *bits = calloc(numbits + 1, sizeof(char));
            for (int i = 1; i <= numbits; ++i) {
                if (fork() == 0)
                    execl("/home/sope/gen_bit", "/home/sope/gen_bit", NULL);
                else {
                    int ret;
                    wait(&ret);
                    ret = rand() % 2;
                    strcat(bits, ret == 0 ? "0" : "1");
                }
            }
            printf("%s\n", bits);
            return 0;
        }
        ```
    - Apesar de aparentemente aleatória a solução continua determinista, sendo que se fica dependente de rand(), que não é 100% aleatória.

- d) **Signal Handling**
    
    - ```c
        // CÓDIGO DE gen_bit
        int main() {
            pid_t ppid = getppid();
            int r = generateRandomBit();
            if (r == 0) kill(ppid, SIGUSR1);
            else kill(ppid, SIGUSR2);
            return 0;
        }
      ``` 

    - ```c
        static int bit = 0;

        void handler(int signo){
            if (signo == SIGUSR1) bit = 0;
            else if (signo == SIGUSR2) bit = 1;
        }

        // CÓDIGO DE gen_seq
        int main(int argc, char *argv[]) {
            struct sigaction action;
    
            action.sa_handler = handler;
            sigemptyset(&action.sa_mask);
            action.sa_flags = 0;

            if (sigaction(SIGUSR1,&action,NULL) < 0){
                fprintf(stderr,"Unable to install SIGUSR1 handler\n");
                exit(1);
            }
            if (sigaction(SIGUSR2,&action,NULL) < 0){
                fprintf(stderr,"Unable to install SIGUSR2 handler\n");
                exit(1);
            }

            int numbits = atoi(argv[1]);
            // aloca memória e inicializa-a a zero
            char *bits = calloc(numbits + 1, sizeof(char));
            for (int i = 1; i <= numbits; ++i) {
                if (fork() == 0)
                    execl("/home/sope/gen_bit", "/home/sope/gen_bit", NULL);
                else {
                    int ret;
                    wait(&ret);
                    strcat(bits, bit == 0 ? "0" : "1");
                }
            }
            printf("%s\n", bits);
            return 0;
        }
        ```

## 7

- a) FIFO.

    ```mermaid
    graph LR
     p1 --> fifo1;
     fifo1 --> p2;
     p2 --> fifo2;
     fifo2 --> p1;
    ```

- b) **Create and open bidirectional fifo**

```c
if (mkfifo("fifo1",0660) < 0){
    printf("Error creating first fifo\n");
    unlink("fifo1");
    exit(1);
}
if mkfifo("fifo2",0660){
    printf("Error creating second fifo\n");
    unlink("fifo2");
    exit(2);
}

int fd1=open("fifo1",O_WRONLY);
if (fd1 < 0){
    printf("Error opening first fifo\n");
    unlink("fifo1");
    unlink("fifo2");
    exit(3);
}
int fd2=open("fifo2",O_RDONLY);
if (fd2 < 0){
    printf("Error opening second fifo\n");
    unlink("fifo1");
    unlink("fifo2");
    close(fd1);
    exit(4);
}
```

- c) **Write struct to FIFO**
```c
struct data{
    pid_t pid;
    pthread_t tid;
}

struct data data;
data.pid = getpid();
data.tid = pthread_self();
if (write(fd1, &data, sizeof(data))){
    perror("Error writing to fifo\n");
    exit(1);
}
```

- d) Recebendo uma estrutura com valores negativos, poderia indicar que a outra thread iria encerrar em breve.

- e) 
```c
if (close(fd1) < 0){
    perror("Error closing\n");
}
if (unlink("fifo1") < 0){
    perror("Error destroying fifo\n");
}
```

## 8

- b)
    - Criaria um mutex que controlasse numPhoneCalls.
    - Semáforo inicializado a 0 para o controlo de realização de chamadas.

- a) / c) / d)
```c
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

void* caller(void* arg){
    while (numPhoneCalls < MAX_NUM_PHONE_CALLS) {
        sem_wait(&sem);
        //call
    }
}

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
```

