#include <assert.h>
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

Poly *shift_poly(Poly *p, int shift) {
  Poly *tmp = init_poly(p->deg + shift);
  for (int i = 0; i < p->deg; i++) {
    tmp->coef[i + shift] = p->coef[i];
  }
  return tmp;
}

Poly *copy_poly(Poly *src) {
  Poly *tmp = init_poly(src->deg);
  for (int i = 0; i < src->deg; i++)
    tmp->coef[i] = src->coef[i];

  return tmp;
}

void cut_trailing_zeros_for_poly(Poly *p) {
  int largest_coef_idx = 0;

  for (int i = 0; i < p->deg; i++)
    if (p->coef[i] != 0)
      largest_coef_idx = i;

  p->deg = largest_coef_idx + 1;
  int *buffer = malloc(sizeof(int) * p->deg);
  for (int i = 0; i < p->deg; i++)
    buffer[i] = p->coef[i];

  free(p->coef);
  p->coef = buffer;
}

Poly *sum_poly(Poly *p1, Poly *p2) {
  Poly *lowest_deg_p = (p1->deg < p2->deg) ? p1 : p2;
  int deg = (p1->deg > p2->deg) ? p1->deg : p2->deg;

  Poly *p = init_poly(deg);

  int i;
  for (i = 0; i < lowest_deg_p->deg; i++)
    p->coef[i] = p1->coef[i] + p2->coef[i];

  if (p1->deg == p2->deg)
    return p;
  else if (p1->deg < p2->deg) {
    for (; i < p2->deg; i++) {
      p->coef[i] = p2->coef[i];
    }
  } else {
    for (; i < p1->deg; i++) {
      p->coef[i] = p1->coef[i];
    }
  }

  return p;
}

Poly *sub_poly(Poly *p1, Poly *p2) {
  Poly *lowest_deg_p = (p1->deg < p2->deg) ? p1 : p2;
  int deg = (p1->deg > p2->deg) ? p1->deg : p2->deg;

  Poly *p = init_poly(deg);

  int i;
  for (i = 0; i < lowest_deg_p->deg; i++)
    p->coef[i] = p1->coef[i] - p2->coef[i];

  if (p1->deg == p2->deg)
    return p;
  else if (p1->deg < p2->deg) {
    for (; i < p2->deg; i++) {
      p->coef[i] = (-1) * p2->coef[i];
    }
  } else {
    for (; i < p1->deg; i++) {
      p->coef[i] = p1->coef[i];
    }
  }

  return p;
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

  cut_trailing_zeros_for_poly(p);

  return p;
}

/*
4 4
1 + 2x + 3x^2 + 0x^3 + 1x^4
1 + 0x + 2x^2 + 0x^3 + 2x^4

***********************************************************************************
O(n^2) naive alg

(1 + 2x + 3x^2 + 1x^4)(1 + 0x + 2x^2 + 2x^4) =
1    + 0 + 2x^2 + 2x^4 +
2x   + 0 + 4x^3 + 4x^5 +
3x^2 + 0 + 6x^4 + 6x^6 +
x^4  + 0 + 2x^6 + 2x^8 =
1 + 2x + 5x^2 + 4x^3 + 9x^4 + 4x^5 + 8x^6 + 2x^8
1 2 5 4 9 4 8 0 2

***********************************************************************************
O(n^{log_2{3}} karatsuba mult alg

A * B = (t^{n/2}*P + Q)(t^{n/2}*R + T)  = t^{n}*PR  + t^{n/2}*(PT + QR) + QT
x = PR
y = QT
z = PT + QR = (P + Q)(R + T) - x - y

(P + Q)(R + T) = PR + PT + QR + QT = x + z + y

A * B  = t^{n} * x + t^{n/2} * ((P + Q)(R + T) -x -y) + y

x - 1 mult
y - 1 mult
z - 4 sum and 1 mult

=> 3 sub ops with n/2 work on each
T(n) = 3T(n/2) + O(n)

3/2 > 1 => by Master Theorem : T(n) = 3^{log_{2}n} = n^{log_2{3}} < n^2!!!

example


A = 1 + 2x + 3x^2 + 1x^4 = (1 + 2x) + x^2(3 + 1x^2)
B = 1 + 0x + 2x^2 + 2x^4 = (1 + 0x) + x^2(2 + 2x^2)

Q = 1 + 2x
P = 3 + x^2

T = 1 + 0x
R = 2 + 2x^2
 * */

Poly *karatsuba_mult_poly(Poly *p1, Poly *p2) {
  Poly *p, *Q, *P, *T, *R, *PR, *QT, *s_1, *s_2, *prod, *sub_1, *z, *shifted_PR,
      *shifted_z, *p_1;
  int n_1 = p1->deg, n_2 = p2->deg;

  assert(n_1 % 2 == 0);
  assert(n_2 % 2 == 0);
  assert(n_1 == n_2);

#if 1
  int DEG_BOUND = 512;
  if (p1->deg < DEG_BOUND + 1 && p2->deg < DEG_BOUND + 1) {
    p = mult_poly(p1, p2);
    return p;
  }
#endif

  /*
  A = p1
  B = p2
  A * B = (Q + t^{n/2}*P)(T + t^{n/2}*R)  = t^{n}*PR  + t^{n/2}*(PT + QR) + QT
  x = PR
  y = QT
  s_1 = P + Q
  s_2 = R + T
  prod = s_1 * s_2
  sub_1 = prod - x = prod - PR
  z = PT + QR = (P + Q)(R + T) - x - y = prod - x - y = sub_1 - y = sub_1 - QT
  */
  Q = init_poly(n_1 / 2);
  for (int i = 0; i < Q->deg; i++)
    Q->coef[i] = p1->coef[i];

  if (n_1 % 2 == 0) {
    P = init_poly(n_1 / 2);
    for (int i = Q->deg; i < p1->deg; i++)
      P->coef[i - Q->deg] = p1->coef[i];
  } else {
    P = init_poly(n_1 / 2 + 1);
    for (int i = Q->deg; i < p1->deg; i++)
      P->coef[i - Q->deg] = p1->coef[i];
  }

  T = init_poly(n_2 / 2);
  for (int i = 0; i < T->deg; i++)
    T->coef[i] = p2->coef[i];

  if (n_2 % 2 == 0) {
    R = init_poly(n_2 / 2);
    for (int i = T->deg; i < p2->deg; i++)
      R->coef[i - T->deg] = p2->coef[i];
  } else {
    R = init_poly(n_2 / 2 + 1);
    for (int i = T->deg; i < p2->deg; i++)
      R->coef[i - T->deg] = p2->coef[i];
  }

  PR = karatsuba_mult_poly(P, R);
  QT = karatsuba_mult_poly(Q, T);

  s_1 = sum_poly(P, Q);
  s_2 = sum_poly(R, T);

  prod = karatsuba_mult_poly(s_1, s_2);

  sub_1 = sub_poly(prod, PR);
  z = sub_poly(sub_1, QT);

  shifted_PR = shift_poly(PR, P->deg + R->deg);
  shifted_z = shift_poly(z, (P->deg + R->deg) / 2);

  p_1 = sum_poly(shifted_PR, shifted_z);
  p = sum_poly(p_1, QT);

  free_poly(PR);
  free_poly(QT);
  free_poly(s_1);
  free_poly(s_2);
  free_poly(prod);
  free_poly(sub_1);
  free_poly(shifted_PR);
  free_poly(shifted_z);
  free_poly(p_1);
  free_poly(z);
  free_poly(Q);
  free_poly(P);
  free_poly(T);
  free_poly(R);

  cut_trailing_zeros_for_poly(p);

  return p;
}

int main() {
  int nitems, n_1, n_2;
  Poly *p1, *p2, *p1kmp2;

#if 0
  Poly *p1mp2, *p1sp2, *p1subp2, *p1shift;
#endif

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

  p1shift = shift_poly(p1, 10);
  print_restricted_poly(p1shift);

  p1sp2 = sum_poly(p1, p2);
  print_restricted_poly(p1sp2);

  p1subp2 = sub_poly(p1, p2);
  print_restricted_poly(p1subp2);

  p1mp2 = mult_poly(p1, p2);
  print_restricted_poly(p1mp2);
#endif

  p1kmp2 = karatsuba_mult_poly(p1, p2);
  print_restricted_poly(p1kmp2);

#if 0
  printf("BASIC MULT DEG RES IS %d\n", p1mp2->deg);
  printf("KARA MULT DEG RES IS %d\n", p1kmp2->deg);
  for (int i = 0; i < p1mp2->deg; i++) {
    if (p1mp2->coef[i] != p1kmp2->coef[i]) {
      printf("WRONG COEF ON POS %d\n", i);
      printf("%d FOR BASIC AND %d for KARA\n", p1mp2->coef[i], p1kmp2->coef[i]);
      break;
    }
  }
#endif

  free_poly(p1);
  free_poly(p2);

#if 0
  free_poly(p1shift);
  free_poly(p1sp2);
  free_poly(p1subp2);
  free_poly(p1mp2);
#endif

  free_poly(p1kmp2);
}
