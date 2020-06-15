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
        if (*str == '\n')*str = '\0';
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