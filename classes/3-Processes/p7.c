// PROGRAMA p7.c
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char *argv[]){
    char prog[20];
    sprintf(prog,"%s.c",argv[1]);
    printf("Program %s has been compiled\n", argv[1]);
    execlp("gcc","gcc",prog,"-Wall","-o",argv[1],NULL); // Compiles code from argv[1].c
    printf("Comand not executed!");

    /***EXTRA -> executes code from argv[1].c
    char p[20] = "./";
    strcat(p, argv[1]);
    execlp(p, p, NULL);
    */
    exit(0);
} 