#include <stdio.h>
#include <stdlib.h>

int ipow(int base, int exp) {
  int power = 1;
  for (int i = 0; i < exp; i++) {
    power *= base;
  }
  return power;
}

unsigned int spowm(unsigned a, unsigned b, unsigned n) {
  unsigned exp, mult, prod;
  exp = 1;
  if (b > 1) {
    for (int i = 1; i < b; i++) {
      exp = ipow(a, exp);
    }
  }

  mult = a % n;
  prod = 1;

  while (exp > 0) {
    if (exp % 2 == 1) {
      prod = (prod * mult) % n;
      exp -= 1;
    }
    mult = (mult * mult) % n;
    exp /= 2;
  }

  return prod;
}

int main() {
  int nitems, a, b, n, res;
  nitems = scanf("%d%d%d", &a, &b, &n);
  if (nitems != 3)
    exit(1);
  res = spowm(a, b, n);
  printf("%d\n", res);
}
