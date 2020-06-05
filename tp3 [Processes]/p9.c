// PROGRAMA p9.c
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>

int main(int argc, char *argv[], char *envp[])
{
    pid_t pid, pid_child;
    int status;
    
    if (argc != 2) {
        printf("usage: %s dirname\n",argv[0]);
        exit(1);
    }

    pid=fork();

    if (pid > 0) {
        pid_child = wait(&status);

        int end = WIFEXITED(status);
        if (end) printf("Child terminated normally!\n");
        else printf("Child did not terminate normally!\n");

        int signal = WIFSIGNALED(status);
        if (signal) printf("Child was terminated by a signal!\n");
        else printf("Child was not terminated by a signal!\n");

        printf("PARENT: son %d terminated with exit code %d\n", pid_child, WEXITSTATUS(status));
        printf("My child executed command \"ls -laR %s\"\n", argv[1]);
    }
    else if (pid == 0){
        char exercise[1];
        //Does the same as bellow but appears before 'father' as it is a system call
        //write(STDOUT_FILENO, "Insert exercise letter: ", strlen("Insert exercise letter: "));
        //read(STDIN_FILENO, exercise, sizeof(char));

        printf("Insert exercise letter: ");
        scanf("%s", exercise);
        
        char *cmd[4];

        switch(exercise[0]){
            case 'a':
                execlp("ls", "ls", "-laR", argv[1], NULL);
                break;
            case 'b':
                execl("/bin/ls", "/bin/ls", "-laR", argv[1], NULL);
                break;
            case 'c':
                cmd[0] = "ls";
                cmd[1] = "-laR";
                cmd[2] = argv[1];
                cmd[3] = NULL;
                execvp("ls", cmd);
                break;
            case 'd':
                cmd[0] = "/bin/ls";
                cmd[1] = "-laR";
                cmd[2] = argv[1];
                cmd[4] = NULL;
                execv("/bin/ls", cmd);
                break;
            case 'e':
                cmd[0] = "/bin/ls";
                cmd[1] = "-laR";
                cmd[2] = argv[1];
                cmd[4] = NULL;
                execve("/bin/ls", cmd, envp);
                break;
        }

        printf("Command not executed !\n");
        exit(1);
    }
    exit(0);
} 