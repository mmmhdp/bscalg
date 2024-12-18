#include <stdio.h>

unsigned long long fib(unsigned n, unsigned m) {
  unsigned long long tmp, first = 0ull, second = 1ull;
  int idx;

  if (n == 0) {
    return 0ull;
  }
  for (idx = 2; idx <= n; idx++) {
    tmp = second;
    second = (first + second) % m;
    first = tmp;
  }
  return second;
}

int main() {
  unsigned long long res;
  unsigned n, m;
  scanf("%u%u", &n, &m);
  res = fib(n, m);
  printf("%llu\n", res);
}
