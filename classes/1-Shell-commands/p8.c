#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

int main(int argc, char *argv[]){
  

  clock_t begin = clock();

  time_t t;
  srand((unsigned) time(&t));

  int n = rand() % atoi(argv[1]);
  int it = 0;
  printf("Iteration: %d;  Random number: %d; Number: %d\n", it, n, atoi(argv[2]));

  while (n != atoi(argv[2])){
    n = rand() % atoi(argv[1]);
    it++;
    printf("Iteration: %d;  Random number: %d; Number: %d\n", it, n, atoi(argv[2]));
  }

  clock_t end = clock();
  double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;

  printf("CPU time: %lf\n", time_spent);
  printf("Real time: %lf\n", (double)(end - begin));

  return 0;
}