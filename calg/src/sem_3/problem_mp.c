#include <stdio.h>
#include <stdlib.h>

typedef struct {
  int deg;
  int *coef;
} Poly;

void free_poly(Poly *p) {
  free(p->coef);
  free(p);
}

Poly *init_poly(int deg) {
  Poly *p = (Poly *)malloc(sizeof(Poly));
  p->deg = deg;
  p->coef = (int *)calloc(p->deg, sizeof(int));
  return p;
}

void print_poly(Poly *p) {
  for (int i = 0; i < p->deg; i++)
    printf("%d ", p->coef[i]);
  printf("\n");
}

void print_restricted_poly(Poly *p) {
  int largest_coef_idx = 0;

  for (int i = 0; i < p->deg; i++)
    if (p->coef[i] != 0)
      largest_coef_idx = i;

  for (int i = 0; i < largest_coef_idx + 1; i++)
    printf("%d ", p->coef[i]);
  printf("\n");
}

/*
 2 3
 1 + 2x
 1 + 2x + 3x^2

 (1 + 2x)(1 + 2x + 3x^2) = (1 + 2x + 3x^2) + (2x + 4x^2 + 6x^3) =
 1 + 4x + 7x^2 + 6x^3

 1 4 7 6
 * */

Poly *mult_poly(Poly *p1, Poly *p2) {
  int deg = p1->deg * p2->deg;
  Poly *p = init_poly(deg);

  for (int i = 0; i < p1->deg; i++) {
    for (int j = 0; j < p2->deg; j++) {
      if ((i != 0 && j != 0) || (i == 0 && j == 0))
        p->coef[i + j] += p1->coef[i] * p2->coef[j];
      else if (i == 0 && j != 0)
        p->coef[j] += p1->coef[i] * p2->coef[j];
      else if (i != 0 && j == 0)
        p->coef[i] += p1->coef[i] * p2->coef[j];
    }
  }

  return p;
}

int main() {
  int nitems, n_1, n_2;
  Poly *p1, *p2, *p1mp2;

  nitems = scanf("%d %d", &n_1, &n_2);
  if (nitems != 2)
    abort();
  p1 = init_poly(n_1);
  p2 = init_poly(n_2);

  for (int i = 0; i < n_1; i++) {
    nitems = scanf("%d", &(p1->coef[i]));
    if (nitems != 1)
      abort();
  }

  for (int i = 0; i < n_2; i++) {
    nitems = scanf("%d", &(p2->coef[i]));
    if (nitems != 1)
      abort();
  }

#if 0
  print_poly(p1);
  print_poly(p2);
#endif

  p1mp2 = mult_poly(p1, p2);
  print_restricted_poly(p1mp2);

  free_poly(p1);
  free_poly(p2);
  free_poly(p1mp2);
}
