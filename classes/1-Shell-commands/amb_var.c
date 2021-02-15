#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[], char **envp)
{
  for (char **env = envp; *env != 0; env++){
    //a)
    printf("%s \n", *env);
    
    //b)
    if (strncmp("USER=", *env, 5) == 0) printf("%s \n", *env);
  }

  //c)
  printf("USER=%s\n", getenv("USER"));

  //d)
  printf("USER=%s\n", getenv("USER_NAME"));
 
  return 0;
} 