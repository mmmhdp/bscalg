#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#define BFSZ 1000

void
swap (int *arr, unsigned low, unsigned high)
{
  int tmp = arr[low];
  arr[low] = arr[high];
  arr[high] = tmp;
}

unsigned
partition (int *arr, unsigned low, unsigned high)
{
  assert (high >= low);

  int pivot = arr[low];
  low++;

  for (; low <= high;)
    {
      if (arr[low] <= pivot)
        {
          low++;
          continue;
        }

      if (arr[high] > pivot)
        {
          high--;
          continue;
        }

      swap (arr, low, high);
      low++;
      high--;
    }

  return high;
}

void
print_arr (int arr[], int len, int ipivot)
{
  assert (len >= ipivot);

  char is[BFSZ] = { 0 };
  char s[BFSZ] = { 0 };

  for (int i = 0; i < len; ++i)
    {
      if (i == ipivot)
        {
          sprintf (is + i * 7, "|>%4d<", i);
          sprintf (s + i * 7, "|>%4d<", arr[i]);
        }
      else
        {
          sprintf (is + i * 7, "| %4d ", i);
          sprintf (s + i * 7, "| %4d ", arr[i]);
        }
    }

  printf ("%s|\n", is);
  printf ("%s|\n", s);
}

int
main (void)
{
  unsigned int low, high, ipivot;
  int *arr, len, tmp;

  scanf ("%d", &len);
  arr = malloc (sizeof (int) * len);
  for (int i = 0; i < len; i++)
    {
      scanf ("%d", &tmp);
      arr[i] = tmp;
    }

  printf ("Before\n");
  print_arr (arr, len, len);

  low = 0;
  high = len - 1;
  ipivot = partition (arr, low, high);

  printf ("After\n");
  print_arr (arr, len, ipivot);
  printf ("Pivot index is: %d\n", ipivot);

  free (arr);
  return 0;
}
