#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#define BFSZ 1000

void
merge (int *arr, int l, int m, int r)
{
  int i, lhs, rhs, idx, is_left_exhausted;

  int ibegin = l, ipivot = m;

  int bfsz = r - l + 1;
  int *buff = calloc (bfsz, sizeof (int));

  // merge root loop
  m++;
  for (lhs = rhs = idx = 0; l <= ipivot && m <= r; idx++)
    {
      lhs = arr[l];
      rhs = arr[m];

      if (lhs < rhs)
        {
          buff[idx] = lhs;
          l++;
        }
      else
        {
          buff[idx] = rhs;
          m++;
        }
    }

  // merge siffix loop
  is_left_exhausted = ipivot - l;
  if (is_left_exhausted < 0)
    {
      for (; m <= r; idx++)
        {
          assert (idx < bfsz);

          buff[idx] = arr[m];
          m++;
        }
    }
  else
    {
      for (; l <= ipivot; idx++)
        {
          assert (idx < bfsz);

          buff[idx] = arr[l];
          l++;
        }
    }

  // buff to main arr

  for (i = 0; i < bfsz; i++)
    {
      arr[ibegin + i] = buff[i];
    }

  free (buff);
}

void
merge_sort_imp (int *arr, int l, int r)
{
  if (l >= r)
    return;
  int m = (l + r) / 2;
  merge_sort_imp (arr, l, m);
  merge_sort_imp (arr, m + 1, r);
  merge (arr, l, m, r);
}

void
merge_sort (int *arr, int n)
{
  merge_sort_imp (arr, 0, n - 1);
}

void
print_arr (int arr[], int len)
{
  for (int i = 0; i < len; i++)
    printf ("%d ", arr[i]);
  printf ("\n");
}

int
main (void)
{
  int *arr, len, tmp;

  scanf ("%d", &len);
  arr = malloc (sizeof (int) * len);
  for (int i = 0; i < len; i++)
    {
      scanf ("%d", &tmp);
      arr[i] = tmp;
    }

  printf ("Before\n");
  print_arr (arr, len);

  merge_sort (arr, len);

  printf ("After\n");
  print_arr (arr, len);

  free (arr);
  return 0;
}
