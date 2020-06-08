# 2009-10

## Parte  A1
### 1
1. **Multiprogramação** -> Execução intercalada de processos.
2. **Modo utilizador e Modo supervisor** -> impede que os processos dos utilizadores possam executar directamente certas instruções-máquina, as
quais só podem ser executas em modo supervisor, através de "chamadas ao sistema" feitas por esses
processos.
3. **Processo** está bloqueado se estiver à espera de um evento ou recurso.
4. O **tempo de comutação** entre threads do mesmo processo é inferior ao temmpo correspoondente entre processos tradicionais.
5. **Round robin** é problemático com processos com elevados I/O Bursts, favorecendo os processos que fazem uso intensivo do processador.
6. **Secção crítica** é um bloco de código que acede a variáveis ou recursos comuns a mais do que um processo/thread, variáveis e recursos esses que têm de ser usados em exclusão mútua.
7. O contador do semáforo não pode ser inicializado com **valores negativos**.
8. Poderá ocorrer situação de deadlock.
9. Linux **processos em execução** -> comando ps
10. **mkfifo()** não devolve um descritor de ficheiro.
11. **wait()** -> processo-pai espera que o processo-filho termine.
12. **signal()** instala o handler de um sinal.
13. **Comunicação entre processos pai e filho** pipe tem que ser criado antes do processo filho.
14. **Vantagem dos FIFOS em relação a pipes**: permitem a transferência de informação entre quaisquer dois processos que estejam em execução num mesmo computador.

## Parte A2

### 1 (Problema consumidor-produtor)
```c
PROGRAMA CONSUMIDOR
do{
    sem_wait(mayConsume);
    sem_wait(m);
    item = buffer[I]; //extrai item
    I = (I + 1) % N;
    sem_signal(m);
    sem_signal(mayProduce);
    ... // processa item
}  while();
```
- a) **Valores iniciais dos semáforos**
    - mayConsume -> 1
    - mayProduce -> 0

- b) 

```c
PROGRAMA PRODUTOR
do{
    sem_wait(mayProduce);
    sem_wait(m);
    I = (I - 1) % N;
    buffer[I] = item;
    sem_signal(m);
    sem_signal(mayConsume);
    ... // processa item
}  while();
```

### 2
- a) Ao ser passada uma referência da variável 't', futuras alterações à variável serão tidas em conta.

- b) 
```c
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
```

## Parte B

### 1


