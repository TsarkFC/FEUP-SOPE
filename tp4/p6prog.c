#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>

int main(int argc, char* argv[]){
    int times = 0;
    while (1){
        times++;
        sleep(5);
        write(STDOUT_FILENO, argv[1], strlen(argv[1]));
        write(STDOUT_FILENO, "\n", 1);
        if (times == 6) break;
    }
    write(STDOUT_FILENO, "Prog finished!\n", strlen("Prog finished!\n"));
    execlp("kill", "kill", "-SIGKILL", argv[2], NULL);
    return 0;
}