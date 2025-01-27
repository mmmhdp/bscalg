#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdint.h>

typedef unsigned long long ull;

ull spowm(ull mult, ull exp, ull mod) {
  ull prod = 1;
  mult = mult % mod;
  while (exp > 0) {
    if (exp % 2 == 1) {
      __uint128_t mul = (__uint128_t) (prod % mod) * (mult % mod);
      prod = mul % mod;
      exp -= 1;
    }
    __uint128_t sub_mul = (__uint128_t) (mult % mod) * (mult % mod);
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
    x = y; y = q;
  }
  return y;
}


int ft_is_prime(ull P, int ntests){
  if (P == 1ULL) return 0;
  if (P == 2ULL) return 1;
  if (P == 3ULL) return 1;

  if ((P & 1ULL) == 0ULL) return 0;

  ull witness, tval, tmp_gcd;
  
  for (int i = 0; i < ntests; i++){
    witness = 2ULL + rand() % (P - 3ULL);

    tmp_gcd = gcd(witness, P);
    if (tmp_gcd == 1ULL){
      tval = spowm(witness, P - 1, P);
      if (tval != 1ULL){
        return 0;
      }
    } else if (tmp_gcd != P){
      return 0;
    } 
  }
  return 1;
}

int main(void) {
    srand((unsigned)time(NULL));
    int k, n;

    if (scanf("%d %d", &k, &n) != 2) {
        return 1;
    }

    const ull LIMIT = (1ULL << 60);
    ull seq0 = 0ULL;
    ull seq1 = 1ULL;

    ull best_prime = 0ULL;

    for(;;) {
        __uint128_t next_val = ( __uint128_t )k * seq1 + ( __uint128_t )n * seq0;
        
        if (next_val >= LIMIT)
            break;

        ull seq2 = (ull) next_val;

        if (seq2 >= 2ULL) {
            if (ft_is_prime(seq2, 20)) {
                if (seq2 > best_prime) {
                    best_prime = seq2;
                }
            }
        }
        seq0 = seq1;
        seq1 = seq2;
    }

    printf("%llu\n", best_prime);
    return 0;
}

