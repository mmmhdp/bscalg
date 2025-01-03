#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

long long gcd(long long a, long long b) { return b == 0 ? a : gcd(b, a % b); }

long long lcm(long long a, long long b) { return (a * b) / gcd(a, b); }

long long llm(long long n) {
  long long running_lcm = 2;

  for (long long i = 3; i <= n; i++) {
    running_lcm = lcm(running_lcm, i);
  }

  return running_lcm;
}

int main() {

  int N, nitems;

  nitems = scanf("%d", &N);

  if (nitems != 1)
    abort();

  printf("%lld\n", llm(N));
  return 0;
}
