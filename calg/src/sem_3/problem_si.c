#include <stdio.h>

unsigned
moveright (int *arr, int key, int last)
{
  int i;
  for (i = last - 1; i >= 0; i--)
    {
      if (arr[i] > key)
        {
          arr[i + 1] = arr[i];
        }
      else
        {
          i++;
          break;
        }
    }
  return i;
}

void
inssort (int *arr, int pos)
{
  int key;
  key = arr[pos];
  pos = moveright (arr, key, pos);
  arr[pos] = key;
}

int
main (void)
{
  int arr[6] = { 1, 2, 4, 5, 3, 2 };
  int arr_len = 6;
  int last = 4;

  inssort (arr, last);

  for (int i = 0; i < arr_len; i++)
    {
      printf ("%d ", arr[i]);
    }
}
