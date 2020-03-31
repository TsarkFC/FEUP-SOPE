#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <errno.h>
#include <string.h>
#include <linux/limits.h>

int main(int argc, char *argv[])
{
    DIR *dirp;
    struct dirent *direntp;
    struct stat stat_buf;
    char *str;
    char directoryname[150] = { '\0' };

    if (argc != 2)
    {
        fprintf( stderr, "Usage: %s dir_name\n", argv[0]);
        exit(1);
    }

    if ((dirp = opendir(argv[1])) == NULL)
    {
        perror(argv[1]);
        exit(2);
    }

    while ((direntp = readdir(dirp)) != NULL){
        char fp[PATH_MAX];
        snprintf(fp, sizeof(fp), "%s/%s", argv[1], direntp->d_name);
        if (lstat(fp, &stat_buf)==-1)
        {
            perror("lstat ERROR");
            exit(3);
        }
        if (S_ISREG(stat_buf.st_mode)) {
            long num = stat_buf.st_size;
            str = "regular";

            printf("%-25s - %s  size: %ld\n", direntp->d_name, str, num);
        }
        
        else if (S_ISDIR(stat_buf.st_mode)) {
            strcpy(directoryname, direntp->d_name);
            if(directoryname[1] != '\0'){
                if(directoryname[2] != '\0'){

                    str = "directory";
                    printf("%-25s - %s\n", direntp->d_name, str);
                }
            }
            else if(directoryname[0] != '.' || directoryname[1] != '\0'){
                if(directoryname[1] != '.' || directoryname[2] != '\0'){
                    str = "directory";
                    printf("%-25s - %s\n", direntp->d_name, str);
                }
            }
            memset(directoryname, 0, sizeof(directoryname));
            //directoryname[0] = '\0';
        }
        else {
            str = "other";
            printf("%-25s - %s\n", direntp->d_name, str);
        }
        
    }
    closedir(dirp);
    
    exit(0);
}

