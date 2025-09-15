#include <stdio.h>
#include <stdlib.h>

int
main (void)
{
  int i, tmp, nitems, cache_sz, q_sz, *q;

  nitems = scanf ("%d", &cache_sz);
  if (nitems != 1)
    exit (1);
  nitems = scanf ("%d", &q_sz);
  if (nitems != 1)
    exit (1);

  q = malloc (sizeof (int) * q_sz);

  for (i = 0; i < q_sz; i++)
    {
      nitems = scanf ("%d", &tmp);
      if (nitems != 1)
        exit (1);
      q[i] = tmp;
    }

  for (i = 0; i < q_sz; i++)
    {
      printf ("%d ", q[i]);
    }
  printf ("\n");

  free (q);

  return 0;
}
