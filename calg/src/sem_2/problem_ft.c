#include <assert.h>
#include <limits.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define ENDL (printf("\n"))
#define TRUE 1
#define FALSE 0

typedef unsigned long long ull;

ull spowm(ull mult, ull exp, ull mod) {
  ull prod = 1;
  mult = mult % mod;

  while (exp > 0) {
    if (exp % 2 == 1) {
      __uint128_t mul = (__uint128_t)prod * mult;
      prod = mul % mod;
      exp -= 1;
    }

    __uint128_t sub_mul = (__uint128_t)mult * mult;
    mult = sub_mul % mod;
    exp /= 2;
  }

  return prod;
}

ull gcd(ull x, ull y) {
  ull q;
  q = x % y;

  if (q == 0)
    return y;

  for (;;) {
    q = x % y;
    if (q == 0)
      break;
    x = y;
    y = q;
  }
  return y;
}

int ft_is_prime(ull P) {
  if (P == 1ULL)
    return 0;
  if (P == 2ULL)
    return 1;
  if (P == 3ULL)
    return 1;

  if ((P & 1ULL) == 0ULL)
    return 0;

  ull witness, tval, tmp_gcd;
  int ntests = 64;

  for (int i = 0; i < ntests; i++) {
    witness = 2ULL + rand() % (P - 3ULL);

    tmp_gcd = gcd(witness, P);
    if (tmp_gcd == 1ULL) {
      tval = spowm(witness, P - 1, P);
      if (tval != 1ULL) {
        return 0;
      }
    } else if (tmp_gcd != P) {
      return 0;
    }
  }
  return 1;
}

int main() {
  int nitems, p_prime_flag;
  ull P;
  srand((unsigned)time(NULL));

#if 0
  for (int t = 0; t < 5; t++){
    nitems = scanf("%llu", &P);
    if (nitems != 1)
      abort();
   
    p_prime_flag = ft_is_prime(P);
    printf("%d\n", p_prime_flag);
  }
#endif

#if 1
  nitems = scanf("%llu", &P);
  if (nitems != 1)
    abort();

  p_prime_flag = ft_is_prime(P);
  printf("%d\n", p_prime_flag);
#endif
}
