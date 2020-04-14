#include <stdio.h>
#include <stdlib.h>

void function1(){
  printf("Executing exit handler 1\n");
}
void function2(){
  printf("Executing exit handler 2\n");
}
void function3(){
  printf("Executing exit handler 3\n");
}
void main_done(){
  printf("Main done!\n");
}

int main(void){
  //Call is reversed!
  atexit(function1);
  atexit(function2);
  atexit(function3);
  //abort();
  atexit(main_done);
  return 0;
}