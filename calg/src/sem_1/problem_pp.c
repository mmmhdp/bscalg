#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

long long fibm(long long n, long long m) {
  long long old, prev, next;

  if (n == 0)
    return 0;
  if (n == 1)
    return 1 % m;
  if (n == 2)
    return 1 % m;

  old = 1 % m;
  prev = 1 % m;
  next = -100;

  for (long long i = 2; i < n; i++) {
    next = (old + prev) % m;
    old = prev;
    prev = next;

    assert(next >= 0);
  }
  return next;
}

int piz(int m) {
  int prd = 0, prev = 0, mid = 1 % m, next = -100;

  if (m == 0)
    return 0;
  if (m == 1)
    return 1;

  assert(m > 1);

  for (;; ++prd) {
    next = (prev + mid) % m;
    prev = mid;
    mid = next;

    if ((prev == 0 && mid == 1) && prd != 0) {
      break;
    }
  }

  prd += 1;

  return prd;
}

void fib_mod(long long n, long long m) {
  int prd = piz(m);
  long long fib_mod_m = fibm(n % prd, m);

  printf("%lld %d\n", fib_mod_m, prd);
}

int main() {
  char nitems;
  long long x, m;

  nitems = scanf("%lld%lld", &x, &m);

  if (nitems != 2) {
    printf("Incorrect number of arguments. Needed 2 but have %c instead",
           nitems);
    abort();
  }

  fib_mod(x, m);
}
