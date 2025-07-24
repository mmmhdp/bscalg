#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

int
majority_element (const int *parr, int len)
{
  int idx, mel, sum, cnt;

  sum = 0;
  mel = parr[0];

  for (idx = 0; idx < len; idx++)
    {
      if (parr[idx] == mel)
        {
          sum++;
          continue;
        }

      sum--;

      if (sum <= 0)
        {
          mel = parr[idx];
          sum = 0;
        }
    }

  cnt = 0;
  for (idx = 0; idx < len; idx++)
    {
      if (parr[idx] == mel)
        cnt++;
    }

  if (cnt > len / 2)
    return mel;

  return -1;
}

int
main (void)
{
  int *arr, len, idx, me;
  scanf ("%d", &len);
  arr = calloc (len, sizeof (int));
  for (idx = 0; idx < len; idx++)
    scanf ("%d", &(arr[idx]));

  me = majority_element (arr, len);
  printf ("%d\n", me);

  free (arr);
}
