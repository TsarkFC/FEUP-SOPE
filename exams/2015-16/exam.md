# 2015-16

## 1

- a) **Hardware usado em:**

    - **Proteção do SO:** MMU - Memory Management Unit. Impede certos processos de acederem regiões de memória às quais não têm acesso.

    - **Escalonamento do processador:** Timer. Controlo do tempo utilizado por cada processo.

    - **Semáforos:** Instruções test_and_test que bloqueiem a execução de um processo.

- b) 
    - Comparação da dureação de CPU bursts. Caso estes sejam longos estamos perante um processo CPU-bound, caso contrário I/O bound.

    - Algoritmos de escalonamento tendem a **penalizar CPU bound**. São mais "pesados". Prioridade de um processo pode estar associada ao tempo de CPU burst. Processos com longos CPU bursts serão mais penalizados.

- c) 
```c
sem_t palces;
sem_t picking;

void viatura(){
    wait(places);
    wait(picking);
    pick_place();
    signal(picking);

    //Quando quiser sair
    signal(places):
}

int main(){
    init(places, N);
    init(picking, 1);
    //Criação de processos/threads
}
```

- d)

    - O **endereço lógico de x é o mesmo em ambos os casos**. No entanto quando o processo filho é criado, é criada uma cópia de x, que apesar de possuir o mesmo endereço lógico, possui endereço físico diferente, justificando-se assim os diferentes valores que a variável apresenta.

    - A ordem dos processos a executar a seguir a uma chamada fork poderá não ser sempre a mesma.

- e)

    - Em muitos casos é preferível que ocorra um deadlock, de vez em quando, do que estar sujeito ao overhead necessário para os evitar/detetar.

    - **Prevenção de deadlocks:** assegurar que pelo menos 1 não se verifica:
        
        - exclusão mútua;
        - retém e espera;
        - não preempção dos recursos;
        - espera circular. 

- f) 

    - **Trashing:** acontece quando um processo passa mais tempo em atividades de paginação do que a executar. O que se verifica em P2.

    - Já existem processadores com a utilização baixa, pelo que **aumentar o grau de multiprogramação** não seria aconselhável.

- g)

    - /home/user1/SOPE

    - 
        - a - Diretório
        - b - Ficheiro regular
        - g - Fifo
        - permissões de acesso de b: **‐rw‐rw‐r‐‐**
            
            - owner: read and write;
            - group: read and write;
            - others: read.

    - chmod +x b ou chmod 0764 b

    - C e E, possuem o mesmo i-node.

## 2

```c
//a)
int setupFifos(){
    for (int i = 0; i < NUM_WORKERS; i++){
        char str[20];
        sprintf(str, "/tmp/myfifo%d", i+1);
        mkfifo(str, 0660);
    }
    return 0;
}

//b)
int setupLB(){
    return pthread_create(&loadBalancer, NULL, lbEntry, NULL);
}

//c)
int setupWorkers(){
    int ids[NUM_WORKERS];
    for (int i = 1; i < NUM_WORKERS+1; i++){
        ids[i] = i;
        pthread_create(&workers[i], NULL, workerEntry, (void*)&ids[i]);
    }
    return 0;
}

//d)
void* workerEntry(void* p){
    //1
    int *id = (int*) p;
    char fifoname[20];
    sprintf(fifoname, "/tmp/myfifo%d", *id);
    int fd = open(fifoname, O_RDONLY);

    //2
    char buffer[100];
    char filename[20];
    sprintf(filename, "worker%d.log", *id);
    int write_file = open(filename, O_WRONLY | O_APPEND | O_CREAT, 0600);

    //3
    while (readline(fd, buffer)){
        write(write_file, buffer, strlen(buffer)+1);
    }
}

int readline(int fd, char *str){
    int n;
    do {
        n = read(fd,str,1);
    } while (n>0 && *str++ != '\0');
    return (n>0);
}

//e)
int teardownWorkers(){
    for (int i = 1; i < NUM_WORKERS+1; i++){
        pthread_join(workers[i], NULL);
    }
    return 0;
}
int teardownLB(){
    return pthread_join(loadBalancer, NULL);
}
```

## 3

- a) - e)

```c
info_t get_info();
int C = 0;

//b) / c)
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

void* t2(void* arg){
    //d)
    pthread_mutex_lock(&mutex);

    while(!C)
        pthread_cond_wait(&cond, &mutex);

    //B

    pthread_mutex_unlock(&mutex);

}

void* t1(void* arg){
    //a)
    pthread_t tid;
    info_t* info = malloc(sizeof(info_t));
    *info = get_info();
    pthread_create(&tid, NULL, t2, (void*)info);

    //d)
    pthread_mutex_lock(&mutex);
    //change conditional A
    pthread_cond_signal(&cond); //e) -> pthread_cond_broadcast(&cond);
    pthread_mutex_unlock(&mutex);
}
```

- f)
    - Dois processos distintos não partilhariam as variáveis globais que as threads partilham, sendo assim necessário recorrer a outros mecanismos como regiões de memória partilhadas ou pipes para a comunicação entre processos seja possível.

    - O mutex e a variável de condição necessitariam assim de ser inicializados dinamicamente recorrendo a **pthread_mutex_init** e **pthread_cond_init** incluindo **PTHREAD_PROCESS_SHARED**.

    - Exemplo:
     ```c
        pthread_mutexattr_t mutex;
        pthread_mutex_attr_init(&mutex);
        pthread_mutexattr_setpshared(&mutex,PTHREAD_PROCESS_SHARED);

        pthread_condattr_t cond;
        pthread_condattr_init(&cond);
        pthread_condattr_setpshared(&cond, PTHREAD_PROCESS_SHARED);
        ```

