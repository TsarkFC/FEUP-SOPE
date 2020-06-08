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



int readline(int fd, char *str){
    int n;
    do {
    n = read(fd,str,1);
    } while (n>0 && *str++ != '\0');
    return (n>0);
}

void recursive(char* dirpath){
    DIR *dirp;
    struct dirent *direntp;
    struct stat stat_buf;
    char *str;
    char directoryname[150] = { '\0' };

    if ((dirp = opendir(dirpath)) == NULL){
        perror(dirpath);
        exit(2);
    }

    printf("%s:\n", dirpath);
    pid_t pid = fork();

    if (pid == 0){
        execlp("ls", "ls", "-la", dirpath, NULL);
    }
    else if (pid > 0){
        wait(NULL);
        while ((direntp = readdir(dirp)) != NULL){
            char fp[PATH_MAX];
            snprintf(fp, sizeof(fp), "%s/%s", dirpath, direntp->d_name);

            if (lstat(fp, &stat_buf)==-1){
                perror("lstat ERROR");
                exit(3);
            }
            
            if (S_ISDIR(stat_buf.st_mode)) {
                strcpy(directoryname, direntp->d_name);
                if (directoryname[0] != '.' && directoryname[1] != '\0'
                    && directoryname[0] != '.' && directoryname[1] != '.' && directoryname[2] != '\0'){
                    char dir[50] = "/";
                    strcat(dir, directoryname);
                    char directory[50];
                    strcpy(directory, dirpath);
                    strcat(directory, dir);
                    recursive(directory);
                }
                memset(directoryname, 0, sizeof(directoryname));
            } 
        }
        closedir(dirp);
    }
}

int main(int argc, char *argv[])
{

    if (argc != 2){
        fprintf( stderr, "Usage: %s dir_name\n", argv[0]);
        exit(1);
    }

    recursive(argv[1]);
    
    exit(0);
}



