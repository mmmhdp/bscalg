#include <stdio.h>
#include <stdlib.h>

void
get_one (int *box)
{
  int nitems;
  nitems = scanf ("%d", box);
  if (nitems != 1)
    abort ();
}

void
find_min_max (const int *arr, const int len, int *const pmin, int *const pmax)
{
  int tmp_min, tmp_max, idx = 3;
  tmp_min = arr[0] < arr[1] ? arr[0] : arr[1];
  tmp_max = arr[0] > arr[1] ? arr[0] : arr[1];

  while (idx < len)
    {
      if (arr[idx] < tmp_min)
        tmp_min = arr[idx];
      if (arr[idx] > tmp_max)
        tmp_max = arr[idx];

      idx++;
    }

  *pmin = tmp_min;
  *pmax = tmp_max;
}

int
main (void)
{
  int arr_len, *arr, tmp, arr_min, arr_max;
  get_one (&arr_len);

  if (arr_len == 0)
    {
      printf ("0 0\n");
      return 0;
    }

  if (arr_len == 1)
    {
      get_one (&tmp);
      printf ("%d\n", tmp);
      return 0;
    }

  arr = malloc (arr_len * sizeof (int));

  for (int i = 0; i < arr_len; i++)
    get_one (&(arr[i]));

  find_min_max (arr, arr_len, &arr_min, &arr_max);
  printf ("%d %d\n", arr_min, arr_max);

  free (arr);

  return 0;
}
