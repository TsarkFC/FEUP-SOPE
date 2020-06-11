#include <stdio.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <stdlib.h>

#define READ 0
#define WRITE 1

struct info{
    int filetype; //1, 2 or 100
    int inodenr;
    int blksize;
    char name[50];
};

int main(int argc, char *argv[]){
    DIR *dirp;
    struct dirent *direntp;
    struct stat stat_buf;
    int status;
    
    int fd[2];
    pipe(fd);

    if ((dirp = opendir(argv[1])) == NULL){
        perror(argv[1]);
        exit(2);
    }

    while ((direntp = readdir(dirp)) != NULL){
        if (strcmp(direntp->d_name, ".") == 0 
        || strcmp(direntp->d_name, "..") == 0) continue;
        struct info info;

        char fp[PATH_MAX];
        snprintf(fp, sizeof(fp), "%s/%s", argv[1], direntp->d_name);
        strcpy(info.name, direntp->d_name);
        

        if (lstat(fp, &stat_buf)==-1){
            perror("lstat ERROR");
            exit(3);
        }
        
        if (S_ISREG(stat_buf.st_mode)) {
            if (strcmp(direntp->d_name, argv[2]) == 0)
                info.filetype = 100;
            else
                info.filetype = 1;
        }

        else if (S_ISDIR(stat_buf.st_mode))
            info.filetype = 2;
        
        info.inodenr = stat_buf.st_ino;
        info.blksize = stat_buf.st_blksize;
        write(fd[WRITE], &info, sizeof(info));

        int pid = fork();
        if (pid == 0){
            close(fd[WRITE]);
            dup2(fd[READ], STDIN_FILENO);
            execlp("./formatter", "./formatter", NULL);
        }
        else if (pid > 0){
            waitpid(-1, &status, WNOHANG);
        }
    }
    closedir(dirp);
}
