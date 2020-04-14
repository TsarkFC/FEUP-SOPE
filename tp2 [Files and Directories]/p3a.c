#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

#define BUFFER_SIZE 512

int main(int argc, char *argv[])
{
  int fd1, fd2, nr, nw;
  unsigned char buffer[BUFFER_SIZE];

  if (argc != 2) {
    printf("Usage: %s <filename>\n", argv[0]);
    return 1;
  }

  //OPEN SOURCE FILE
  fd1 = open(argv[1], O_WRONLY | O_APPEND | O_CREAT);
  if (fd1 == -1) {
    perror(argv[1]);
    return 2;
  }

  //READ SOURCE AND SEND TO DESTINATION
  while ((nr = read(fd1, buffer, BUFFER_SIZE)) > 0)
    if ((nw = write(STDOUT_FILENO, buffer, nr)) <= 0 || nw != nr) {
      perror(argv[1]);
      close(fd1);
      return 4;
  }
    
  close(fd1);

  return 0;
}

