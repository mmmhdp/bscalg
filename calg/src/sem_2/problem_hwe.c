#include <assert.h>
#include <limits.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#define ENDL (printf("\n"))
#define TRUE 1
#define FALSE 0

struct sieve_t {
  int n;
  unsigned char *mod1;
  unsigned char *mod5;
};

void set_bit(struct sieve_t *sv, int pos) {
  int offset = (pos / 6) % CHAR_BIT, byte_offset = (pos / 6) / CHAR_BIT;

  if (pos % 6 != 1 && pos % 6 != 5)
    return;

  if (pos % 6 == 1) {
    sv->mod1[byte_offset] |= (1u << offset);
  } else {
    sv->mod5[byte_offset] |= (1u << offset);
  }
}

int check_bit(struct sieve_t *sv, int pos) {
  int offset = (pos / 6) % CHAR_BIT, byte_offset = (pos / 6) / CHAR_BIT;
  if (pos % 6 == 1)
    return sv->mod1[byte_offset] & (1u << offset);
  return sv->mod5[byte_offset] & (1u << offset);
}

int is_prime(struct sieve_t *sv, int num) {
  if (num == 0)
    return FALSE;
  if (num == 2 || num == 3)
    return TRUE;
  if (num % 6 != 1 && num % 6 != 5)
    return FALSE;
  return (check_bit(sv, num)) ? 0 : 1;
}

void fill_sieve(struct sieve_t *sv) {
  int el_mod1, el_mod5;
  int sieve_range = sv->n * 6 * CHAR_BIT;
  set_bit(sv, 1);

  for (int el = 6; (el - 1) * (el - 1) < sieve_range; el += 6) {
    el_mod1 = el - 1;
    if (check_bit(sv, el_mod1) == 0) {
      for (int sub_el = el_mod1 * el_mod1; sub_el < sieve_range;
           sub_el += el_mod1) {
        set_bit(sv, sub_el);
      }
    }

    el_mod5 = el + 1;
    if (el_mod5 * el_mod5 >= sieve_range)
      break;
    if (check_bit(sv, el + 1) == 0) {
      for (int sub_el = el_mod5 * el_mod5; sub_el < sieve_range;
           sub_el += el_mod5) {
        set_bit(sv, sub_el);
      }
    }
  }
}

int sieve_bound(int num) {
  double dnum, dres;
  if (num <= 20)
    return 100;
  dnum = num;
  dres = dnum * (log(dnum) + log(log(dnum)));
  return (int)round(dres);
}

int nth_prime(struct sieve_t *s, int N) {
  int cnt, p;
  if (N == 1)
    return 2;
  if (N == 2)
    return 3;

  cnt = 2;
  p = 5;

  for (;;) {
    if (is_prime(s, p)) {
      cnt += 1;
      if (cnt == N)
        break;
    }
    p += 2;
    if (is_prime(s, p)) {
      cnt += 1;
      if (cnt == N)
        break;
    }
    p += 4;
  }

  return p;
}

int main() {
  int n, outp;
  int res;
  struct sieve_t *s;

#if 1
  res = scanf("%d", &n);
  assert(res == 1);
#endif

  s = (struct sieve_t *)malloc(sizeof(struct sieve_t));
  s->n = ((sieve_bound(n) / CHAR_BIT) / 6) + 1;
  s->mod1 = (unsigned char *)calloc(s->n + 1, sizeof(char));
  s->mod5 = (unsigned char *)calloc(s->n + 1, sizeof(char));

  fill_sieve(s);

#if 1
  {
    int i = 0, k = 0;
    printf("primes:\n");
    while (k < n) {
      if (is_prime(s, i)) {
        printf("%d ", i);
        k += 1;
      }
      i += 1;
    }
    printf("\n");
  }

  outp = nth_prime(s, n);
  printf("%d\n", outp);
#endif

  free(s->mod1);
  free(s->mod5);
  free(s);
}

#line 10000
