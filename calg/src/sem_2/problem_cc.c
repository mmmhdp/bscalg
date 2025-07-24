#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#define TRUE 1
#define FALSE 0

#define SIEVE_CAP 10000000

#define ENDL printf ("\n");

typedef struct
{
  int n;
  char *s;
} Sieve;

typedef struct
{
  int *digits;
  int len;
  int val;
} Num;

void
_fill_sieve (Sieve *sv)
{
  sv->s[0] = 1;
  sv->s[1] = 1;

  for (int i = 2; i * i < sv->n; i++)
    {
      if (sv->s[i] == 0)
        {
          for (int gap = i + i; gap < sv->n; gap += i)
            {
              sv->s[gap] = 1;
            }
        }
    }
}

int
_sieve_bound (int num)
{
  double dnum, dres;
  if (num <= 20)
    return 100;
  dnum = num;
  dres = dnum * (log (dnum) + log (log (dnum)));
  return (int)round (dres);
}

int
_is_prime (int n, Sieve *sv)
{
  return sv->s[n] == 0 ? TRUE : FALSE;
}

Num *
_init_num (int k)
{
  int tmp_k = k, counter = 0, gap = 10;

  while (TRUE)
    {
      if (tmp_k / gap == 0)
        {
          counter++;
          break;
        }
      gap *= 10;
      counter++;
    }

  int *digits = malloc (sizeof (int) * counter);

  for (int i = counter - 1; i >= 0; i--)
    {
      digits[i] = tmp_k % 10;
      tmp_k = (tmp_k - (tmp_k % 10)) / 10;
    }

  Num *new_num = malloc (sizeof (Num));
  new_num->digits = digits;
  new_num->len = counter;
  new_num->val = k;

  return new_num;
}
void
_shift_circ_num (Num *_k)
{
  int tmp_left = _k->digits[(_k->len) - 1], tmp_right;
  for (int i = 0; i < _k->len; i++)
    {
      tmp_right = _k->digits[i];
      _k->digits[i] = tmp_left;
      tmp_left = tmp_right;
    }

  int tmp_val = 0;
  int multy = 1;
  for (int i = (_k->len) - 1; i >= 0; i--)
    {
      tmp_val += _k->digits[i] * multy;
      multy *= 10;
    }

  _k->val = tmp_val;
}

int
_is_circ (int k, Sieve *sv)
{
  Num *_k = _init_num (k);

  for (int i = 0; i < _k->len; i++)
    {
      if (!_is_prime (_k->val, sv))
        {
          return FALSE;
        }
      _shift_circ_num (_k);
    }

  return TRUE;
}

void
_find_circ (int N, Sieve *sv, int *cl_circ)
{
  int left = N - 1, right = N + 1;
  while (left > 1 || right <= sv->n)
    {
      if (left > 1)
        {
          if (_is_circ (left, sv))
            {
              *cl_circ = left;
              return;
            }
          else
            {
              left -= 1;
            }
        }

      if (right < SIEVE_CAP)
        {
          if (_is_circ (right, sv))
            {
              *cl_circ = right;
              return;
            }
          else
            {
              right += 1;
            }
        }
    }
}

void
get_closest_circ (int N, int *cl_circ)
{
  Sieve *sv = (Sieve *)malloc (sizeof (Sieve));
  sv->n = _sieve_bound (SIEVE_CAP);
  sv->s = (char *)calloc (sv->n, sizeof (char));

  _fill_sieve (sv);

  _find_circ (N, sv, cl_circ);

  free (sv->s);
  free (sv);
}

int
main (void)
{
  int nitems, N;

  nitems = scanf ("%d", &N);
  if (nitems != 1)
    {
      abort ();
    }

  int cl_circ;
  get_closest_circ (N, &cl_circ);
  printf ("%d\n", cl_circ);
}
