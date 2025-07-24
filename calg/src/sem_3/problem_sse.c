#include <stdio.h>
#include <stdlib.h>

#define FALSE 0
#define TRUE 1

int
check_scanf_res (int expected, int nitems)
{
  int status_code = (expected != nitems) ? FALSE : TRUE;
  return status_code;
}

int
check_malloc_res (int *p)
{
  int status_code = (p == NULL) ? FALSE : TRUE;
  return status_code;
}

void
fill_arr (int *arr, int len)
{
  int i, tmp, nitems;
  for (i = 0; i < len; i++)
    {
      nitems = scanf ("%d", &tmp);
      if (!check_scanf_res (1, nitems))
        abort ();
      arr[i] = tmp;
    }
}

unsigned
get_idx_min (int *arr, int len, unsigned last_sorted_idx)
{
  unsigned min_idx = last_sorted_idx;
  int tmp_min = arr[last_sorted_idx];

  for (int i = last_sorted_idx + 1; i < len; i++)
    {
      if (tmp_min > arr[i])
        {
          tmp_min = arr[i];
          min_idx = i;
        }
    }

  return min_idx;
}

void
swap (int *arr, unsigned isrc, unsigned idist)
{
  int tmp = arr[isrc];
  arr[isrc] = arr[idist];
  arr[idist] = tmp;
}

void
sort_iteration (int *arr, unsigned len, unsigned idx)
{
  if (idx == len - 1)
    return;
  int imin = get_idx_min (arr, len, idx);
  swap (arr, idx, imin);
}

void
print_arr (int *arr, int len)
{
  for (int i = 0; i < len; i++)
    printf ("%d ", arr[i]);
  printf ("\n");
}

int
main (void)
{
  int len, nitems, *arr, last_sorted_idx;

  nitems = scanf ("%d", &len);
  if (!check_scanf_res (1, nitems))
    abort ();

  arr = (int *)malloc (len * sizeof (int));
  if (!check_malloc_res (arr))
    abort ();

  fill_arr (arr, len);

  nitems = scanf ("%d", &last_sorted_idx);
  if (!check_scanf_res (1, nitems))
    abort ();

#if 0
  printf("Before:\n");
  print_arr(arr, len);

  sort_iteration(arr, len, last_sorted_idx);

  printf("After:\n");
  print_arr(arr, len);
#else
  sort_iteration (arr, len, last_sorted_idx);

  print_arr (arr, len);
#endif

  free (arr);
}
