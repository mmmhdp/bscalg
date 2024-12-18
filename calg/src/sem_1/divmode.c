#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

void read_input(int *pa, int *pb) {
  printf("Input a and b: ");
  int nitems = scanf("%d%d", pa, pb);
  if (nitems != 2 || pb == 0) {
    printf("Error: input invalid, expected any a and b != 0\n");
    abort();
  }
}
int main() {
  int a, b, p, q;

  read_input(&a, &b);

  assert(b != 0);

  p = a / b;
  q = a % b;
  printf("p = %d, q = %d\n", p, q);

  return 0;
}
