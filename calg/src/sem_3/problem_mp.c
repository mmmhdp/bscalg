#include <assert.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct
{
  int n;
  int *box;
} Poly;

void
poly_init (Poly **p, int n)
{
  *p = calloc (1, sizeof (Poly));
  if (*p == NULL)
    {
      printf ("Allocation for poly failed\n");
      return;
    }

  (*p)->n = n;
  (*p)->box = calloc (n, sizeof (int));
  if ((*p)->box == NULL)
    {
      printf ("Allocation for poly box failed\n");
      free (*p);
    }
}

void
poly_free (Poly *p)
{
  free (p->box);
  free (p);
}

void
polys_free (int nump, ...)
{
  int i;
  Poly *ptmppoly;

  va_list ppolys;
  va_start (ppolys, nump);

  for (i = 0; i < nump; i++)
    {
      ptmppoly = va_arg (ppolys, Poly *);
      poly_free (ptmppoly);
    }

  va_end (ppolys);
}

int
poly_trim_dim (Poly const *const r)
{
  int i, n = r->n;

  for (i = r->n - 1; i >= 0; i--)
    {
      if (r->box[i] == 0)
        n--;
      else
        break;
    }

  return n;
}

void
poly_print (Poly const *const p)
{
  int i, with_trimmed_zeros_n;

  with_trimmed_zeros_n = poly_trim_dim (p);

  if (p->n <= 0 || p->box == NULL)
    {
      return;
    }

  for (i = 0; i < with_trimmed_zeros_n; i++)
    printf ("%d ", p->box[i]);
  printf ("\n");
}

void
poly_sum (Poly const *const p1, Poly const *const p2, Poly *const r)
{
  int i, idx, *buff, bfsz;

  bfsz = (p1->n > p2->n) ? p1->n : p2->n;
  buff = calloc (bfsz, sizeof (int));

  free (r->box);
  r->box = buff;
  r->n = bfsz;

  idx = (p1->n < p2->n) ? p1->n : p2->n;
  for (i = 0; i < idx; i++)
    {
      r->box[i] = p1->box[i] + p2->box[i];
    }

  if (idx == p1->n)
    for (i = idx; i < p2->n; i++)
      r->box[i] = p2->box[i];
  else
    for (i = idx; i < p1->n; i++)
      r->box[i] = p1->box[i];
}

void
poly_subs (Poly const *const p1, Poly const *const p2, Poly *const r)
{
  int i, idx, *buff, bfsz;

  bfsz = (p1->n > p2->n) ? p1->n : p2->n;
  buff = calloc (bfsz, sizeof (int));

  free (r->box);
  r->box = buff;
  r->n = bfsz;

  idx = (p1->n < p2->n) ? p1->n : p2->n;
  for (i = 0; i < idx; i++)
    {
      r->box[i] = p1->box[i] - p2->box[i];
    }

  if (idx == p1->n)
    for (i = idx; i < p2->n; i++)
      r->box[i] = p2->box[i];
  else
    for (i = idx; i < p1->n; i++)
      r->box[i] = p1->box[i];
}

void
poly_mult (Poly const *const p1, Poly const *const p2, Poly *const r)
{
  int i, j;

  // assert(r->n >= p2->n + p1->n);

  for (i = 0; i < p1->n; i++)
    {
      for (j = 0; j < p2->n; j++)
        {
          r->box[i + j] += p1->box[i] * p2->box[j];
        }
    }
}

/* Karastuba alg descriprion
 * (A_1x^{n/2} + A_0)(B_1x^{n/2} + B_0) =
 * = A_1*B_1x^{n} + ((A_1 + A_0)(B_1 + B_0) - A_1*B_1 - A_0*B_0)x^{n/2}  +
 * A_0*B_1 =
 *
 * thus only 3 mult needed
 *
 * C_1 = A_1 * B_1
 * C_0 = A_0 * B_0
 * A_s = A_1 + A_0
 * B_s = B_1 + B_0
 * C   = A_s * B_s
 * C_s = C - C_1
 * C_2 = C_s - C_0
 *
 *
 *
 * and
 *
 * p1 * p2 = C_1x^{n} + C_2x^{n/2} + C_0
 *
 * */

void
poly_right_shift (Poly *p, int shift_steps)
{
  int i, *buff, bfsz;

  bfsz = (shift_steps + p->n);
  buff = calloc (bfsz, sizeof (int));

  for (i = 0; i < p->n; i++)
    buff[i + shift_steps] = p->box[i];

  free (p->box);
  p->box = buff;
  p->n += shift_steps;
}

void
poly_karatsuba_mult (Poly const *const p1, Poly const *const p2, Poly *const r)
{
  // assert(r->n >= p2->n + p1->n);
  // assert(p2->n == p1->n);
  int trim_dim_p1 = poly_trim_dim (p1);
  int trim_dim_p2 = poly_trim_dim (p2);

  int n = (trim_dim_p1 < trim_dim_p2) ? trim_dim_p1 : trim_dim_p2;

  // naive works fine for small scale
  if (n <= 1)
    {
      poly_mult (p1, p2, r);
      return;
    }

  // use Karatsub's alg for larger polynoms

  Poly *A_0 = NULL, *B_0 = NULL, *A_1 = NULL, *B_1 = NULL, *C_1 = NULL,
       *C_0 = NULL, *A_s = NULL, *B_s = NULL, *C = NULL, *C_s = NULL,
       *C_2 = NULL, *S = NULL;

  // init and allocate
  poly_init (&A_1, n);
  poly_init (&B_1, n);
  poly_init (&C_1, n);

  poly_init (&A_0, n - 1);
  poly_init (&B_0, n - 1);
  poly_init (&C_0, 2 * (n - 1) - 1);

  poly_init (&A_s, n);
  poly_init (&B_s, n);
  poly_init (&C, 2 * n - 1);

  poly_init (&C_s, 2 * n);
  poly_init (&C_2, 2 * n);
  poly_init (&S, 2 * n);

  // fill

  A_1->box[0] = p1->box[poly_trim_dim (p1) - 1];
  B_1->box[0] = p2->box[poly_trim_dim (p1) - 1];

  for (int i = 0; i < poly_trim_dim (p1) - 1; i++)
    A_0->box[i] = p1->box[i];

  for (int i = 0; i < poly_trim_dim (p2) - 1; i++)
    B_0->box[i] = p2->box[i];

  // main part

  poly_karatsuba_mult (A_1, B_1, C_1);
  poly_karatsuba_mult (A_0, B_0, C_0);
  poly_sum (A_1, A_0, A_s);
  poly_sum (B_1, B_0, B_s);
  poly_karatsuba_mult (A_s, B_s, C);
  poly_subs (C, C_1, C_s);
  poly_subs (C_s, C_0, C_2);

  poly_right_shift (C_1, 2 * (n - 1));
  poly_right_shift (C_2, n - 1);
  poly_sum (C_1, C_2, S);
  poly_sum (S, C_0, r);

#if 0
  printf("C_1\n");
  poly_print(C_1);
  poly_right_shift(C_1, 2*(n - 1));
  poly_print(C_1);

  printf("C_2\n");
  poly_print(C_2);
  poly_right_shift(C_2, n - 1);
  poly_print(C_2);

  printf("S\n");
  poly_print(S);
  poly_sum(C_1, C_2, S);
  poly_print(S);

  printf("r\n");
  poly_print(r);
  poly_sum(S, C_0, r);
  poly_print(r);
#endif

  polys_free (12, A_0, B_0, A_1, B_1, C_1, C_0, A_s, B_s, C, C_s, C_2, S);
}

int
main (void)
{

  int nitems, tmp, i;

  Poly *p1 = calloc (1, sizeof (Poly));
  Poly *p2 = calloc (1, sizeof (Poly));
  Poly *p3 = calloc (1, sizeof (Poly));

  nitems = scanf ("%d", &tmp);
  p1->n = tmp;
  if (nitems != 1)
    abort ();

  p1->box = calloc (p1->n, sizeof (int));

  nitems = scanf ("%d", &tmp);
  p2->n = tmp;
  if (nitems != 1)
    abort ();

  p2->box = calloc (p2->n, sizeof (int));

  p3->n = p1->n + p2->n;
  p3->box = calloc (p3->n, sizeof (int));

  for (i = 0; i < p1->n; i++)
    {
      nitems = scanf ("%d", &tmp);
      if (nitems != 1)
        abort ();

      p1->box[i] = tmp;
    }

  for (i = 0; i < p2->n; i++)
    {
      nitems = scanf ("%d", &tmp);
      if (nitems != 1)
        abort ();

      p2->box[i] = tmp;
    }

#if 1
  poly_mult (p1, p2, p3);
#elif 0
  poly_subs (p1, p2, p3);
#elif 0
  poly_right_shift (p1, 2);
#else
#endif

#if 0
  printf("Polynoms\n");
  poly_print(p1);
  poly_print(p2);
#endif
  poly_print (p3);

  poly_free (p1);
  poly_free (p2);
  poly_free (p3);

  return 0;
}
