#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#define ENDL (printf("\n"))

struct sieve_t {
  int n;
  char *s;
};

void fill_sieve(struct sieve_t *sv) {
  sv->s[0] = 1;
  sv->s[1] = 1;

  for (int i = 2; i * i < sv->n; i++) {
    if (sv->s[i] == 0) {
      for (int gap = i + i; gap < sv->n; gap += i) {
        sv->s[gap] = 1;
      }
    }
  }
}

int nth_prime(struct sieve_t *sv, int N) {
  int counter = 0, target = -1;

  for (int i = 2; i < sv->n; i++) {
    if ((sv->s)[i] == 0)
      counter += 1;
    if (counter == N) {
      target = i;
      break;
    }
  }

  assert(target > 0);
  return target;
}

void print_bitwise(int n, char *name) {
  while (*name != '\0')
    printf("%c", *name++);
  ENDL;
  for (int i = 31; i >= 0; i--) {
    printf("%c", (n & (1 << i)) ? '1' : '0');
  }
  ENDL;
}

int sieve_bound(int num) {
  double dnum, dres;
  if (num <= 20)
    return 100;
  dnum = num;
  dres = dnum * (log(dnum) + log(log(dnum)));
  return (int)round(dres);
}

int main() {

#if 0
  int mask = 0b1010101010, bitstr = 0b0010101111;

  print_bitwise(mask, "mask");
  print_bitwise(bitstr, "bitstr");
  print_bitwise(mask & bitstr, "mask & bitstr");

  for (int i = 0; i < 20; i++){
    printf("number %d is ", i);
    is_prime(i) ? printf("prime") : printf("NOT prime");
    ENDL;
  }
#endif

  int n, outp;
  struct sieve_t *s;

  n = 1000000;
  s = (struct sieve_t *)malloc(sizeof(struct sieve_t));
  s->n = sieve_bound(n);
  s->s = (char *)calloc(s->n, sizeof(char));

  fill_sieve(s);
  outp = nth_prime(s, n);
  printf("%d\n", outp);

  free(s->s);
  free(s);
}
