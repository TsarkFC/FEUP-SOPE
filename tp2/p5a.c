#include <fcntl.h>
#include <unistd.h>

int main(void)
{
  int fd;
  char *text1="AAAAA";
  char *text2="BBBBB";

  //O_EXCL -> fails if file already exists

  fd = open("f1.txt",O_CREAT/*|O_EXCL*/|O_TRUNC|O_WRONLY|O_SYNC,0600);
  write(fd,text1,5);
  write(fd,text2,5);
  close(fd);
  return 0;
}

