#include <stdio.h>

typedef int (*generator_t) (int);

int
generator (int seed)
{
  static int cnt = 0;
  if (cnt == 0)
    {
      cnt++;
      return 0;
    }

  seed = (3 * seed + 2) % 5 + 4;
  return seed;
}

unsigned
cycle_len (generator_t gen)
{
  unsigned cnt;
  int trash_val, r, t;

  cnt = 0;
  trash_val = gen (0);
  r = t = gen (trash_val);

  for (;;)
    {
      cnt++;
      r = gen (r);
      if (r == t)
        break;
    }

  return cnt;
}

int
main (void)
{
  int res;
  res = cycle_len (generator);
  printf ("%d\n", res);
}
