#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#define BUF_LENGTH 256

int main(int argc, char *argv[]){
  FILE *src, *dst;

  char buf[BUF_LENGTH];

  //argv[1] = "infile.txt"
  if ( ( src = fopen( argv[1], "r" ) ) == NULL ){
    printf("ERRNO: %d\n", errno);
    printf("usage: nome_do_executável file1 file2 \n");
    exit(1);
  }
  //argv[2] = "outfile.txt"
  if ( ( dst = fopen( argv[2], "w" ) ) == NULL ){
    printf("ERRNO: %d\n", errno);
    exit(2);
  }
  while( ( fgets( buf, BUF_LENGTH, src ) ) != NULL ){
    fputs( buf, dst );
  }

  fclose(src);
  fclose(dst);

  printf("ERRNO: %d\n", errno);
  exit(0); // zero é geralmente indicativo de "sucesso"
} 