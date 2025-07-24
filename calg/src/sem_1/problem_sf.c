#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

/*
0 1 1 2 3 5 8 13

0 1 2 3 4 5 6 7
  0 1 2 3 4 5 6
    1|0 0 0 0 0  1         -> 1
    0 1|0 0 0 0  2         -> 10
    1 0 1|0 0 0  4         -> 101
    0 0 1 0 1|0  11        -> 10100
                 123456
*/

void
toSf (long long x)
{
  if (x == 0)
    {
      printf ("0");
      return;
    }
  if (x == 1)
    {
      printf ("1");
      return;
    }
  assert (x > 1);

  long long idxs = 1;
  long long *fibTable = malloc (sizeof (long long) * (idxs + 1));
  long long prev = 0, mid = 1, next = -1;
  fibTable[0] = prev;
  fibTable[1] = mid;

  for (;; ++idxs)
    {
      next = prev + mid;
      prev = mid;
      mid = next;

      if (x < next)
        {
          break;
        }

      fibTable = realloc (fibTable, sizeof (long long) * (idxs + 1 + 1));
      fibTable[idxs + 1] = next;
    }

  char *xfs = malloc (sizeof (char) * (idxs + 1));
  for (long long t = 0; t < idxs + 1; t++)
    {
      xfs[t] = '0';
    }

  for (long long i = idxs; i > 0 && x > 0;)
    {
      if ((x - fibTable[i]) >= 0)
        {
          xfs[i] = '1';
          x -= fibTable[i];
          i -= 2;
          continue;
        }
      else
        {
          i -= 1;
        }
    }

  for (long long t = idxs; t > 1; t--)
    {
      printf ("%c", xfs[t]);
    }

  free (fibTable);
  free (xfs);
}

int
main ()
{
  long long x;
  scanf ("%lld", &x);
  toSf (x);
}
