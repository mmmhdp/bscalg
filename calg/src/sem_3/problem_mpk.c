#include <assert.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#define TRUE 1
#define FALSE 0
#define SHOW_DEBUG FALSE

typedef struct
{
  int n;
  int *box;
} Poly;

void poly_print (Poly const *const p);
void poly_free (Poly *p);
void polys_free (int nump, ...);
void poly_sum (Poly const *const p1, Poly const *const p2, Poly *const r);
void poly_substraction (Poly const *const p1, Poly const *const p2,
                        Poly *const r);
void poly_extended_right_shift (Poly *const p, int nsteps, Poly *const r);
void poly_copy (Poly const *const p, Poly *r);
void poly_karatsuba_mult (Poly const *const p1, Poly const *const p2,
                          Poly *const r);
void poly_set_box (Poly *const p, int *new_box, int new_dim);

void poly_trim_dim (Poly const *const p, Poly *r);

void
poly_print (Poly const *const p)
{
  int i;
  Poly *pbuff = calloc (1, sizeof (Poly));
  poly_trim_dim (p, pbuff);

  if (p->n <= 0 || p->box == NULL)
    {
      return;
    }

  for (i = 0; i < pbuff->n; i++)
    printf ("%d ", pbuff->box[i]);
  printf ("\n");

  poly_free (pbuff);
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

  assert (i == nump);

  va_end (ppolys);
}

void
poly_set_box (Poly *const p, int *new_box, int new_dim)
{
  free (p->box);
  p->box = new_box;
  p->n = new_dim;
}

void
poly_trim_dim (Poly const *const p, Poly *r)
{
  int idx, real_dim = p->n;
  int *buff;
  for (idx = p->n - 1; idx >= 0; idx--)
    {
      if (p->box[idx] != 0)
        break;
      real_dim--;
    }

  if (real_dim == p->n)
    {
      poly_copy (p, r);
      return;
    }

  if (real_dim == 0)
    {
      buff = calloc (1, sizeof (int));
      real_dim = 0;
      poly_set_box (r, buff, real_dim);
      return;
    }

  buff = calloc (real_dim, sizeof (int));
  for (idx = 0; idx < real_dim; idx++)
    buff[idx] = p->box[idx];
  poly_set_box (r, buff, real_dim);
}

void
poly_sum (Poly const *const p1, Poly const *const p2, Poly *const r)
{
  int idx;
  int dim = p1->n > p2->n ? p1->n : p2->n;

  int major_dim = p1->n > p2->n ? p1->n : p2->n;
  int minor_dim = p1->n < p2->n ? p1->n : p2->n;

  char p1_is_major = 0;
  if (major_dim == p1->n)
    p1_is_major = 1;

  int *buff = calloc (dim, sizeof (int));

  for (idx = 0; idx < minor_dim; idx++)
    buff[idx] = p1->box[idx] + p2->box[idx];

  for (idx = minor_dim; idx < major_dim; idx++)
    if (p1_is_major)
      buff[idx] = p1->box[idx];
    else
      buff[idx] = p2->box[idx];

  poly_set_box (r, buff, major_dim);
}

void
poly_substraction (Poly const *const p1, Poly const *const p2, Poly *const r)
{
  int idx;
  int dim = p1->n > p2->n ? p1->n : p2->n;

  int major_dim = p1->n > p2->n ? p1->n : p2->n;
  int minor_dim = p1->n < p2->n ? p1->n : p2->n;

  char p1_is_major = 0;
  if (major_dim == p1->n)
    p1_is_major = 1;

  int *buff = calloc (dim, sizeof (int));

  for (idx = 0; idx < minor_dim; idx++)
    buff[idx] = p1->box[idx] - p2->box[idx];

  for (idx = minor_dim; idx < major_dim; idx++)
    if (p1_is_major)
      buff[idx] = p1->box[idx];
    else
      buff[idx] = p2->box[idx];

  poly_set_box (r, buff, major_dim);
}

void
poly_copy (Poly const *const sp, Poly *dp)
{
  int *buff, idx, dim;
  dim = sp->n;

  buff = calloc (dim, sizeof (int));

  for (idx = 0; idx < sp->n; idx++)
    buff[idx] = sp->box[idx];

  poly_set_box (dp, buff, dim);
}

void
poly_extended_right_shift (Poly *const p, int nsteps, Poly *const r)
{
  int idx, *buff;
  buff = calloc (p->n + nsteps, sizeof (int));

  for (idx = 0; idx < p->n; idx++)
    buff[idx + nsteps] = p->box[idx];

  poly_set_box (r, buff, p->n + nsteps);
}

void
poly_naive_mult (Poly const *const p1, Poly const *const p2, Poly *r)
{
  int i, j, *buff, dim;
  dim = p1->n + p2->n;
  buff = calloc (dim, sizeof (int));

  for (i = 0; i < p1->n; i++)
    {
      for (j = 0; j < p2->n; j++)
        {
          buff[i + j] += p1->box[i] * p2->box[j];
        }
    }

  poly_set_box (r, buff, dim);
}

// Task assumption is that polys dims will be a power of 2
void
poly_karatsuba_mult (Poly const *const p1, Poly const *const p2, Poly *const r)
{

#if SHOW_DEBUG
  if (((p1->n) == 0) || ((p2->n) == 0))
    {
      printf ("Dims mismatched! Zero dims action!\n");
      printf ("p1 dim = %d, p2 dim = %d\n", p1->n, p2->n);
      poly_print (p1);
      poly_print (p2);
      abort ();
    }
#endif

#if SHOW_DEBUG
  if ((p1->n) != (p2->n))
    {
      printf ("Dims mismatched! Not equal dims of operands\n");
      printf ("p1 dim = %d, p2 dim = %d\n", p1->n, p2->n);
      poly_print (p1);
      poly_print (p2);
      abort ();
    }
#endif

  // Task assumption requirements
  assert (p1->n == p2->n);

  // Reverse recursion at dim equal to 2^8 = 256 with naive algo
  if (((p1->n) == 256) && ((p2->n) == 256))
    {
      poly_naive_mult (p1, p2, r);
      return;
    }

#if SHOW_DEBUG
  if (!(((p1->n % 2) == 0) && ((p2->n % 2) == 0)))
    {
      printf ("Dims mismatched! Odd dims are in place!\n");
      printf ("p1 dim = %d, p2 dim = %d\n", p1->n, p2->n);
      poly_print (p1);
      poly_print (p2);
      abort ();
    }

#endif

  // Task assumption requirements
  assert (p1->n % 2 == 0 && p2->n % 2 == 0);

  Poly *A_0, *A_1, *B_0, *B_1, *S_1, *S_2, *S_3, *S_4, *S_5, *S_6, *S_7, *S_8,
      *S_9, *S_10, *S_11;
  S_1 = calloc (1, sizeof (Poly));
  S_2 = calloc (1, sizeof (Poly));
  S_3 = calloc (1, sizeof (Poly));
  S_4 = calloc (1, sizeof (Poly));
  S_5 = calloc (1, sizeof (Poly));
  S_6 = calloc (1, sizeof (Poly));
  S_7 = calloc (1, sizeof (Poly));
  S_8 = calloc (1, sizeof (Poly));
  S_9 = calloc (1, sizeof (Poly));
  S_10 = calloc (1, sizeof (Poly));
  S_11 = calloc (1, sizeof (Poly));

  A_0 = malloc (sizeof (Poly));
  A_0->n = p1->n / 2;
  A_0->box = &(p1->box[A_0->n]);

#if SHOW_DEBUG
  printf ("A_0\n");
  poly_print (A_0);
#endif

  A_1 = malloc (sizeof (Poly));
  A_1->n = p1->n / 2;
  A_1->box = &(p1->box[0]);

#if SHOW_DEBUG
  printf ("A_1\n");
  poly_print (A_1);
#endif

  B_0 = malloc (sizeof (Poly));
  B_0->n = p2->n / 2;
  B_0->box = &(p2->box[A_0->n]);

#if SHOW_DEBUG
  printf ("B_0\n");
  poly_print (B_0);
#endif

  B_1 = malloc (sizeof (Poly));
  B_1->n = p2->n / 2;
  B_1->box = &(p2->box[0]);

#if SHOW_DEBUG
  printf ("B_1\n");
  poly_print (B_1);
#endif

#if 1
  poly_karatsuba_mult (A_0, B_0, S_1);
  poly_karatsuba_mult (A_1, B_1, S_2);
  poly_sum (A_0, A_1, S_3);
  poly_sum (B_0, B_1, S_4);
  poly_karatsuba_mult (S_3, S_4, S_5);
  poly_substraction (S_5, S_2, S_6);
  poly_substraction (S_6, S_1, S_7);

  poly_extended_right_shift (S_1, p1->n, S_8);
  poly_extended_right_shift (S_7, p1->n / 2, S_9);

  poly_sum (S_8, S_9, S_10);
  poly_sum (S_10, S_2, S_11);

  poly_copy (S_11, r);
#endif

  free (A_0);
  free (A_1);
  free (B_0);
  free (B_1);

  polys_free (11, S_1, S_2, S_3, S_4, S_5, S_6, S_7, S_8, S_9, S_10, S_11);
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

#if SHOW_DEBUG
  printf ("Before\n");
  poly_print (p1);
  poly_print (p2);
  poly_print (p3);
  printf ("\n");

#if 0
  poly_sum(p1, p2, p3);
#elif 0
  poly_substraction (p1, p2, p3);
#elif 0
  poly_copy (p1, p3);
#elif 0
  poly_extended_right_shift (p1, 3, p3);
#else
  poly_karatsuba_mult (p1, p2, p3);
#endif

  printf ("After\n");
  poly_print (p1);
  poly_print (p2);
  poly_print (p3);
#else
  poly_karatsuba_mult (p1, p2, p3);
  // poly_naive_mult(p1, p2, p3);
  poly_print (p3);
#endif
  polys_free (3, p1, p2, p3);
  return 0;
}
