#include <stdio.h>
#include <stdlib.h>

int
myrand ()
{
  static unsigned long int seed = 1;
  seed = seed * 1103515245 + 12345;
  return (unsigned int)(seed / 65536) % 32768;
}

void
get_one (int *box)
{
  int nitems;
  nitems = scanf ("%d", box);
  if (nitems != 1)
    abort ();
}

void
swap (int *src, int *dist)
{
  int tmp;
  tmp = *src;
  *src = *dist;
  *dist = tmp;
}

void
shuffle (int *arr, int len)
{
  int rand_idx;
  for (int i = len - 1; i > 0; i--)
    {
      rand_idx = myrand () % (i + 1);
      swap (&arr[i], &arr[rand_idx]);
    }
}

int
main (void)
{
  int arr_len, *arr, tmp;
  get_one (&arr_len);

  if (arr_len == 0)
    {
      printf ("0\n");
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

  shuffle (arr, arr_len);

  for (int i = 0; i < arr_len; i++)
    printf ("%d ", arr[i]);
  printf ("\n");

  free (arr);

  return 0;
}
