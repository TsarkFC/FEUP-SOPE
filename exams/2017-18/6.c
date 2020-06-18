#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <errno.h>
#include <string.h>
#include <linux/limits.h>    
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>

char destination_dlr[50];
char filename[50];
char dirpath[50];
int status;

int main(int argc, char *argv[]){
    //1)
    if (argc != 4){
        printf("Invalid number of arguments\n");
        exit(1);
    }
    //2)
    strcpy(destination_dlr, argv[3]);
    strcpy(filename, argv[2]);
    strcpy(dirpath, argv[1]);

    //3)
    struct sigaction action;
    action.sa_handler = SIG_IGN;
    sigemptyset(&action.sa_mask);
    action.sa_flags = 0;

    if (sigaction(SIGINT,&action,NULL) < 0){
        fprintf(stderr,"Unable to install SIGINT handler\n");
        exit(1);
    }

    //4)
    process_dir(dirpath);

    return 0;
}

int process_dir(char *dirname)   {
    DIR *dir;
    struct dirent *entry;
    struct stat statbuf;

    if (!(dir = opendir(dirname))) return 1;
    while ((entry = readdir(dir)) != NULL) {
        char path[1024];
        // ‐‐‐ BLOCO A ‐‐‐
        sprintf(path, "%s/%s", dirname, entry->d_name);

        if (lstat(path, &statbuf)==-1){
            perror("lstat ERROR");
            exit(3);
        }
            
        if (S_ISDIR(statbuf.st_mode)) {
            if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
                continue;
            // cria um processo que invoca process_dir()
            if (fork() < 0){
                process_dir(path);
                exit(0);
            }
            else wait(&status);
        }  
        // ‐‐‐ FIM DO BLOCO A ‐‐‐
        // ‐‐‐ BLOCO B ‐‐‐
        else if (S_ISREG(statbuf.st_mode)) { // se 'entry' for um ficheiro regular
            if (strstr(entry->d_name, filename) != NULL) { // se o nome do ficheiro contiver filename
                // cria um processo que invoca o utilitário 'cp'
                if (fork() < 0){
                    execlp("cp", "cp", filename, destination_dlr, NULL);
                }
                else wait(&status);
            }
        }
        // ‐‐‐ FIM DO BLOCO B ‐‐‐
    }
    return 0;
}