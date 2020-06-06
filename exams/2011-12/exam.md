# Exame 2011-2012

## 1
1. Falso
2. Falso
3. Verdade
4. Falso 
5. Verdade
6. Verdade
7. Falso
8. Falso
9. Falso
10. Verdade

## 2
- a)
Não é bidirecional. Variáveis globais não são alteráveis por diferentes processos.
- b)
exec() não é bidirecional.
- c)
pipes -> comunicação unidirecional, sendo que um processo apenas poderá abrir o pipe num sentido.
- d)
memória partilhada -> comunicação bidirecional, sendo possível ler e alterar memória partindo de processos distintos.

## 3
- a) 
    - m1 -> 1, m2 -> 0, bridge -> 0
    - segundo veículo bloqueado até que o primeiro acabe de passar a ponte.
- b) Circulação de múltiplos veículos no mesmo sentido permitida, não havendo limite para o seu número.
- c) Sim, no entanto alterava o funcionamento do programa, não permitindo a entrada de 2 veículos em sentidos distintos.
- d) Não condições para que ocorram deadlocks, no entanto poderão ocorrer casos de inanição caso haja, por exemplo, trânsito permanente num sentido, ficando o outro bloqueado.

## 4

- a)

Tradução de endereço lógico para físico:

- Extrair, do endereço lógico, o número do segmento (bits mais significativos).

- Aceder à tabela de segmentos, usando este número, para obter o endereço físico do início do segmento

- Comparar o deslocamento (bits menos significativos do endereço lógico) com o comprimento do segmento; se aquele for maior do que este o endereço é inválido.

- b) "Segmation fault" -> deslocamento + posição inicial é superior ao tamanho do segmento.

## 5
- a)
```c
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <time.h>

int np = 5;
double mean = 0;
pid_t pid;

void signal_handler(int signo){
    write(STDOUT_FILENO, "Child signal received\n", strlen("Child signal received\n"));
    int status;
    wait(&status);
    printf("Received status: %d\n", WEXITSTATUS(status));
    if (WEXITSTATUS(status)) mean += WEXITSTATUS(status);
    np--;
}

int main(){
    srand(time(0));
    int safe = 5;
 
    struct sigaction action;
    action.sa_handler = signal_handler;
    sigemptyset(&action.sa_mask);
    action.sa_flags = 0;

    if (sigaction(SIGCHLD,&action,NULL) < 0){
        fprintf(stderr,"Unable to install SIGINT handler\n");
        exit(1);
    }

    //process creation
    while(np > 0){
        int num = rand() % 10;
        pid = fork();
        if (pid > 0) pause();
        else if (pid == 0){
            printf("New child created\n");
            int num = rand() % 10;
            printf("Exit code: %d\n", num);
            return num;
        }
    }

    mean /= safe;

    printf("Média: %f\n", mean);
}
```

- b) Caso sejam recebidos vários sinais ao mesmo tempo apenas 1 será tido em conta.

## 6

- a) / b)
```c
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

struct message {
    char str[9];
    pid_t pid;
    char carater;
}

int main(){
    //a)
    struct message msg;
    msg.pid = getpid();
    msg.str = "mkhead";
    msg.carater = 'B';

    int fda = open("/tmp/fifo.serv", O_WRONLY);

    write(fda, &msg, sizeof(msg));
    close(fda);

    //b)
    pid_t pid = getpid();
    char str[9] = "mkhead";
    char carater = 'B';

    int fdb = open("/tmp/fifo.serv", O_WRONLY);

    write(fdb, &pid, sizeof(pid_t));
    write(fdb, str, strlen(str));
    write(fdb, &carater, 1);
    close(fdb);
}
```

- c)
Na alínea b) os argumentos poderão não ser recebidos pela ordem apresentada, o que é resolvido ao enviar uma struct.

- d)
Em ambos os casos deve-se fechar o fifo, podendo haver perdas de informação em caso contrário.

## 7

```c
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
        printf("Numero de jogadas: %d\nApostas ganhas: %d\nApostas perdidas: %d\n\n",  
         numero_jogadas, apostas_ganhas, apostas_perdidas);
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
```