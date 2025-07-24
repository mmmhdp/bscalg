#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#if NAIVE_LOL
#define DEFAULT_BASE 10

typedef struct
{
  int val;
  int idx;
} Element;

int
element_cmp (const void *lhs, const void *rhs)
{
  const Element *elhs = lhs;
  const Element *erhs = rhs;

  return elhs->val - erhs->val;
}

void
radix_sort (int arr[], int len, int radix)
{
  int i, expn, *arr_bf;
  Element *el_bf;

  expn = 1;
  for (i = 0; i < radix; ++i)
    expn *= 10;

  el_bf = calloc (len, sizeof (Element));

  for (i = 0; i < len; ++i)
    {
      el_bf[i].val = (arr[i] / expn) % DEFAULT_BASE;
      el_bf[i].idx = i;
    }

  qsort ((void *)el_bf, len, sizeof (Element), element_cmp);

  arr_bf = malloc (sizeof (int) * len);
  for (i = 0; i < len; i++)
    {
      arr_bf[i] = arr[el_bf[i].idx];
    }

  for (i = 0; i < len; i++)
    {
      arr[i] = arr_bf[i];
    }

  free (arr_bf);
  free (el_bf);
}
#endif

void
radix_sort (int arr[], int len, int radix, int base)
{
  int i, expn, *bf, *cnts, k;

  assert (base > 0);

  bf = calloc (len, sizeof (int));
  assert (bf != NULL);
  cnts = calloc (base, sizeof (int));
  assert (cnts != NULL);

  // Define base for key extraction
  expn = 1;
  for (i = 0; i < radix; ++i)
    expn *= base;

  // Count intrance of each key
  for (i = 0; i < len; ++i)
    {
      k = (int)(arr[i] / expn) % base;
      assert (k < base);
      cnts[k] += 1;
    }

  // Assemble cumulutive sum
  for (i = 1; i < base; ++i)
    cnts[i] += cnts[i - 1];

  // Sort part
  for (i = len - 1; i >= 0; --i)
    {
      k = (int)(arr[i] / expn) % base;
      assert (k < base);

      bf[cnts[k] - 1] = arr[i];
      cnts[k]--;
    }

  // Swap buffer with original array
  for (i = 0; i < len; ++i)
    arr[i] = bf[i];

  free (cnts);
  free (bf);
}

int
main (void)
{
  int nitems, i, n, *arr, radix, base;

  base = 10;

  nitems = scanf ("%d", &n);
  assert (nitems == 1);

  arr = malloc (sizeof (int) * n);
  assert (arr != NULL);

  for (i = 0; i < n; ++i)
    {
      nitems = scanf ("%d", &(arr[i]));
      assert (nitems == 1);
    }

  nitems = scanf ("%d", &radix);
  assert (nitems == 1);

  radix_sort (arr, n, radix, base);

  for (i = 0; i < n; ++i)
    {
      printf ("%d ", arr[i]);
    }
  printf ("\n");

  free (arr);
}
