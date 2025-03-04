#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
  int x_00;
  int x_01;
  int x_10;
  int x_11;
  char is_fib;
} QuadMatrix;

long long fibModulo(long long n, long long m) {
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

char isFib(QuadMatrix *qm) {
  if (((qm->x_00 == qm->x_01) && (qm->x_00 == qm->x_10)) && (qm->x_11 == 0))
    return 1;
  return 0;
}

void fibMPow(QuadMatrix *qm, int n, int m) {
  qm->x_00 = fibModulo(n + 1, m);
  qm->x_01 = fibModulo(n, m);
  qm->x_10 = fibModulo(n, m);
  qm->x_11 = fibModulo(n - 1, m);
}

void mModuloMulty(QuadMatrix *qm, QuadMatrix *qm1, int m) {
  int curr_x_00 = (qm->x_00 * qm1->x_00) % m + (qm->x_01 * qm1->x_10) % m;
  int curr_x_01 = (qm->x_00 * qm1->x_01) % m + (qm->x_01 * qm1->x_11) % m;
  int curr_x_10 = (qm->x_10 * qm1->x_00) % m + (qm->x_11 * qm1->x_10) % m;
  int curr_x_11 = (qm->x_10 * qm1->x_01) % m + (qm->x_11 * qm1->x_11) % m;

  qm->x_00 = curr_x_00 % m;
  qm->x_01 = curr_x_01 % m;
  qm->x_10 = curr_x_10 % m;
  qm->x_11 = curr_x_11 % m;
}

void copyQuadMatrix(QuadMatrix *source, QuadMatrix *dist) {
  dist->x_00 = source->x_00;
  dist->x_01 = source->x_01;
  dist->x_10 = source->x_10;
  dist->x_11 = source->x_11;
  dist->is_fib = source->is_fib;
}

void IInitQuadMatrix(QuadMatrix *source) {
  source->x_00 = 1;
  source->x_01 = 0;
  source->x_10 = 0;
  source->x_11 = 1;
}
void fastBasicMPow(QuadMatrix *qm, int n, int m) {
  QuadMatrix *tmp = malloc(sizeof(QuadMatrix));
  IInitQuadMatrix(tmp);

#if 0
  assert (n > 1);
  for (int i = 1; i < n; i++){
     mModuloMulty(tmp, qm, m);
  }
#endif

  while (n > 0) {
    if (n % 2 == 1) {
      mModuloMulty(tmp, qm, m);
      n -= 1;
    } else {
      mModuloMulty(qm, qm, m);
      n /= 2;
    }
  }

  copyQuadMatrix(tmp, qm);
}

void matrixModuloPow(QuadMatrix *qm, int n, int m) {
  if (qm->is_fib) {
    fibMPow(qm, n, m);
  } else {
    fastBasicMPow(qm, n, m);
  }
}

void showMatrix(QuadMatrix *qm) {
  printf("%d ", qm->x_00);
  printf("%d ", qm->x_01);
  printf("%d ", qm->x_10);
  printf("%d", qm->x_11);
  printf("\n");
}

int main(void) {

  int nitems, x_00, x_01, x_10, x_11, n, m;

  nitems = scanf("%d %d %d %d %d %d", &x_00, &x_01, &x_10, &x_11, &n, &m);

  if (nitems != 6) {
    printf("Invalid number of args. Needed 6 but get %d", nitems);
    abort();
  }

  QuadMatrix *qm = malloc(sizeof(QuadMatrix));
  qm->x_00 = x_00;
  qm->x_01 = x_01;
  qm->x_10 = x_10;
  qm->x_11 = x_11;

  qm->is_fib = isFib(qm);
  matrixModuloPow(qm, n, m);
  showMatrix(qm);
}
