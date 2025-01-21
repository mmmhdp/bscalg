#include <stdio.h>

int int_sum(int *x, int *y) {
  int sum = *x + *y;
  return sum;
}

int main(int argc, char *argv[]) {
  int a, b, s;
  a = 3;
  b = 4;
  s = int_sum(&a, &b);
  printf("Sum of a:= %d and b:= %d is equal to %d\n", a, b, s);
  return 0;
}
