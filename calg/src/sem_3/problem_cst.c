#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

int
find_max (int arr[], int len)
{
  int i, max = arr[0];
  for (i = 1; i < len; ++i)
    if (arr[i] > max)
      max = arr[i];
  return max;
}

void
fill_buckets (int arr[], int len, int buckets[], int max)
{
  int i;
  for (i = 0; i < len; ++i)
    {
      assert (arr[i] <= max);
      buckets[arr[i]] += 1;
    }
}

int
main (void)
{
  int nitems, i, n, *arr, max, *buckets;
  nitems = scanf ("%d", &n);
  assert (nitems == 1);

  arr = malloc (sizeof (int) * n);
  assert (arr != NULL);

  for (i = 0; i < n; ++i)
    {
      nitems = scanf ("%d", &(arr[i]));
      assert (nitems == 1);
    }

  max = find_max (arr, n);
#if 0
  //printf("Max el in arr is : %d\n", max);
#endif

  buckets = calloc (max + 1, sizeof (int));
  assert (buckets != NULL);

  fill_buckets (arr, n, buckets, max);

#if 0
  for (i = 0; i < n; ++i)
    printf("%d ", arr[i]);
  printf("\n");
#endif

  for (i = 0; i < max + 1; ++i)
    printf ("%d ", buckets[i]);

  free (buckets);
  free (arr);
}
