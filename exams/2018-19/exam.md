# 2018-19

## 1 (V/F)

- V - **Espera circular** é a melhor forma que um programador tem para prevenir a espera circular.
- V - for (int i=1; i<=3; i++) { fork(); } printf("Hello "); escreve 8 vezes.
- F - chamadas exec() **não criam um novo processo**.
- V - Processos com **maior prioridade** acabrão mais rapidamente pelo que se justifica a atribuição de um **menor quantum**.
- V - **Preempção** utiliza o **timer** para controlar o tempo de execução de um processo.
- F - **Race condition** acontece quando dois processos utilizam os mesmos dados praticamente simultaneamente.
- V - **Diferentes processos** poderão ter o **mesmo endereço lógico** mapeado para **diferentes regiões de memória física**.
- V (?) - **Princípio de localidade de referência** é a base técnica de gestão de memória virtual.
- F - **Multiprogramação**: execução intercalada de processos. **Multiprocessamento**: execução simultânea de vários processos.
- F - **readdir()** retorna NULL após ler a última entrada de um diretório.
- F - **execlp()** não cria um processo.
- F - **ps** permite saber quais os processos em execução.
- F - **inodes** não contêm o nome do ficheiro associado.
- V - Não é necessário reabrir um ficheiro que já se encontre aberto após a criação de um novo processo.
- V - **Process Control Block** de um **processo filho** só é destruído quando o **processo pai** tomar conhecimento da sua termiação.
- V - 16KBytes -> 2<sup>14</sup>; 2<sup>32</sup> / 2<sup>14</sup> = 2<sup>18</sup>
- V - Múltiplos sinais enviados ao mesmo tempo, poderá resultar em apenas um sinal interpretado.
- V - Protótipo de thread.
- F - **Mutex** pode ser utilizado por diversos processos, tem de ser inicializado de diferente maneira para tal.
- F - **Secção crítica**: pedaço de código que executa recursos partilhados.
Uma **secção atómica** é um pedaço de código que tem de ser executado rapidamente e sem interrupções.

## 2

- a) Um **semáforo ao ser inicializado com um número negativo** será interpretado como um número inteiro sem sinal muito grande, uma vez que irá ocorrer overflow a interpretar o número negativo como unsigned int.

- b)
```c
P0
init(sem1, 1);
init(sem2, 0);
```

```c
P1
while(goOn){
    wait(sem1);
    doWork1();
    signal(sem2);
}
```

```c
P1
while(goOn){
    wait(sem2);
    doWork1();
    signal(sem1);
}

```

## 3

- a) ![](3a.png)
14 faltas de página (4 iniciais mais 10).

- b) 
    - **Thrashing**: o processo passa mais tempo em atividades de paginação do que a executar. Neste caso nas últimas 7 páginas que foram acedidas houve necessidade de carregamento consecutivo.
    - 1, 2, 3, 4, 1, 5, 4, 3, 6, 5, 1, 3, <ins>2, 4, 6, 1, 5, 2, 4</ins> 

- c)
    - 1) Necessidade recorrente de carregar uma página que não se encontrava no espaço de endereçamento físico. Número de páginas ativamente utilizadas (1,2,4,5,6) é superior ao número de **frames** alocados (4).

    - 2) **Estratégia de frequência de falta de página**:
        - monitoriza a frequência de falta de páginas de um processo.
        - estabelece uma gama de frequências aceitáveis.
        - acima de uma certa frequência, atribuir mais um frame ao processo; caso não hajam frames disponíveis, suspender o processo.

## 4
- 1) **Inode** contém apenas a **metadata** relativa a um ficheiro. Essa metadata contém a localização dos blocos na sua estruturas e não os próprios blocos, facilitando o acesso. É combinada uma estratégia de **acesso direto** (para ficheiros mais pequenos) e **acesso indexado** (para ficheiros maiores, onde cada referência de localização do inode do ficheiro aponta para uma tabela de indíces que posteriormente apontará para os blocos de informação).

- 2) Uma vez que cada página é referenciada, após cada acesso é necssário **recolocar essa referência no seu lugar inicial**. Para ficheiros maiores que impliquem utilização de **várias tabelas de índices**, para além do espaço ocupado por cada tabela, para cada acesso, será necessário recolar todos as referências para cada tabela na sua posição original.

## 5

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <errno.h>  
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>

#define READ 0
#define WRITE 1

char filename[50];

int readline(int fd, char *str){
    int n;
    do {
        n = read(fd,str,1);
    } while (n>0 && *str++ != '\0');
    return (n>0);
}

int main(int argc, char *argv[]){
    if (argc != 2){
        printf("./5 filename\n");
        exit(1);
    }

    strcpy(filename, argv[1]);

    int fd[2], fd2[2], status;
    pipe(fd);
    pipe(fd2);

    int file = open(filename, O_CREAT | O_APPEND | O_WRONLY, 0660);
        pid_t pid = fork();
        if (pid > 0){
            char* line = NULL;
            size_t n = 0;

            close(fd[READ]);
            close(fd2[WRITE]);
            FILE* f1 = fdopen(fd[WRITE], "w");
            FILE* f2 = fdopen(fd2[READ], "r");

            while(getline(&line, &n, stdin) != -1){
                fprintf(f1, "%s", line);
                fflush(f1);

                getline(&line, &n, f2);
                line[strlen(line)-1] = '\0';
                write(file, line, strlen(line));
                write(file, " = ", 3);
                getline(&line, &n, f2);
                write(file, line, strlen(line));
            }
            
        }
        else if (pid == 0){
            close(fd[WRITE]);
            close(fd2[READ]);
            printf("Executing...\n");
            dup2(fd[READ], STDIN_FILENO);
            dup2(fd2[WRITE], STDOUT_FILENO);
            execlp("bc", "bc", "-qi", NULL);
            perror("Exec failled");
            exit(1);
        }
    close(file);
    return 0;
}
```

## 6

- a) Processo principal deve criar o FIFO, de modo a estar disponível para qualquer escritor. O FIFO deve existir enquanto o programa leitor está a correr.

- b)
```c
mkfifo("/users/tmp/srvfifo", 0000);
```

- c)
Na escrita é impossível acontecer, uma vez que escritas com número de bytes inferior a **PIPE_BUF** são instruções atómicas.
Na leitura no entanto poderão ocorrer sobreposição de instruções.
Pode-se evitar isto recorrendo a mecanismos de sincronização como um mutex ou um semáforo.

- d)
    - d1) **Região de memória partilhada**
        ```c
        //reader
        int main(){
            mkfifo("/users/tmp/srvfifo", 0000);
            int fd = open("/users/tmp/srvfifo", O_RDONLY);

            int mem = shm_open("/tmp/is_open", O_CREAT | O_RDWR, 0660);
            int* isopen = mmap(NULL, sizeof(int), 
                                    PROT READ|PROT WRITE,MAP SHARED, mem, 0);
            *isopen = 1;
            char buf[200];

            while(readline(fd, buf)){
                if (closing){
                    *isopen = 0;
                }
            }

        }
        ```
        ```c
        //writer
        int main(){
            int fd;
            do {
                fd=open("/users/tmp/srvfifo",O_WRONLY);
                if (fd==-1) sleep(1);
            } while (fd==-1);
            int sending = 1; //change when not writing any longer

            int mem = shm_open("/tmp/is_open", O_RDONLY, 0660);
            int* isopen = mmap(NULL, sizeof(int), 
                                    PROT READ,MAP SHARED, mem, 0);

            while(sending){
                if (*isopen) write(fd, msg, strlen(msg));
                else break;
            }
            close(fd);
        }
        ```
    - d2)
        ```c
        //reader
        int main(){
            mkfifo("/users/tmp/srvfifo", 0000);
            int fd = open("/users/tmp/srvfifo", O_RDONLY);

            int mem = shm_open("/tmp/is_open", O_CREAT | O_RDWR, 0660);
            int* isopen = mmap(NULL, sizeof(int), 
                                    PROT READ|PROT WRITE,MAP SHARED, mem, 0);
            *isopen = 1;
            char buf[200];

            while(1){
                if (readline(fd, buf) == 0 && !*isopen){
                    break;
                }
                if (closing){
                    *isopen = 0;
                }
            }

        }
        ```

    - d3) Verificação feita em d1)

## 7

- a) Caso se passasse i como parametro da thread este valor iria estar sempre a ser alterado, uma vez que era sempre o mesmo endereço que era passado como parametro. Assim temos um endereço de memória para cada valor passado como parametro.

- b) O programa encerraria sem que as restantes threads terminassem.

- c) Bloqueio. A verificação da condição de igualdade apenas se verifica quando o valor é alterado, isto pois é garantido que cada thread que execute a secção crítica irá alterar o valor.

- d)
```c
// NOTE: for simplicity, error treatment was ommited
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
int numThreads;
int turn = 0; // The first thread to run must have thrIndex=turn=0
void * thr(void *arg)
{
    int thrIndex = *(int*)arg; // The effective indexes are 0,1,2,...
    while(1){
        pthread_mutex_lock(&mutex);
        while (turn != index){
            pthread_cond_wait(&cond, &mutex);
        }

        printf("%d ", thrIndex + 1); // The numbers shown are 1,2,3,...
        turn = (turn + 1) % numThreads;
        pthread_cond_signal(&conds[turn]); // Signals next
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}
int main()
{
    printf("Number of threads ? "); scanf("%d", &numThreads);
    int *arg = (int *) malloc(sizeof(int)*numThreads);
    pthread_t *tid = (pthread_t *) malloc(sizeof(pthread_t)*numThreads);
    conds = calloc(numThreads, sizeof(pthread_cond_t));
    pthread_cond_signal(&conds[turn]);
    for (int i = 0; i < numThreads; i++){
        arg[i] = i;
        pthread_create(&tid[i], NULL, thr, (void*)&arg[i]);
    }
    pthread_exit(NULL);
}
```