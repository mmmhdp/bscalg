#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#define TRUE 1
#define FALSE 0

#define SIEVE_CAP 10000000

typedef struct {
  int n;
  char *s;
} Sieve;

void _fill_sieve(Sieve *sv) {
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

int _sieve_bound(int num) {
  double dnum, dres;
  if (num <= 20)
    return 100;
  dnum = num;
  dres = dnum * (log(dnum) + log(log(dnum)));
  return (int)round(dres);
}

int _is_prime(int n, Sieve *sv) { return sv->s[n] == 0 ? TRUE : FALSE; }

int _get_sq_l(int a, int b, Sieve *sv) {
  int counter = 0, n = 0, _var;

  while (TRUE) {
    _var = n * n + n * a + b;
    _var = _var < 0 ? -_var : _var;

    if (!_is_prime(_var, sv)) {
      break;
    }
    n += 1;
  }

  counter = n;
  return counter;
}

void get_coefs(int *a, int *b, int N, int *sq_len) {

  int _a, _b, _val;
  Sieve *sv = (Sieve *)malloc(sizeof(Sieve));
  sv->n = _sieve_bound(SIEVE_CAP);
  sv->s = (char *)calloc(sv->n, sizeof(char));

  _fill_sieve(sv);

  *sq_len = 0;

  for (_a = -N + 1; _a < N; _a++) {
    for (_b = -N + 1; _b < N; _b++) {
      _val = _get_sq_l(_a, _b, sv);
      if (_val > *sq_len) {
        *sq_len = _val;
        *a = _a;
        *b = _b;
      }
    }
  }

  free(sv->s);
  free(sv);
}

int main() {

  int nitems, N, a, b, sq_len;

  nitems = scanf("%d", &N);
  if (nitems != 1) {
    abort();
  }

  get_coefs(&a, &b, N, &sq_len);
  printf("%d %d %d\n", a, b, sq_len);
}
