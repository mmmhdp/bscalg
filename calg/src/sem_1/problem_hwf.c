#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_FIB_IDX 47

int *
get_fib_stash ()
{
  int *fib_stash = malloc (sizeof (int) * MAX_FIB_IDX);
  int x_0 = 0, x_1 = 1, curr;
  fib_stash[0] = x_0;
  fib_stash[1] = x_1;

  for (int i = 2; i < MAX_FIB_IDX; i++)
    {
      curr = x_0 + x_1;
      x_0 = x_1;
      x_1 = curr;

      fib_stash[i] = curr;
    }

  return fib_stash;
}

int
next_turn (int total, int possible)
{
  int idx = MAX_FIB_IDX - 1, opt_move = -1;
  int *stash = get_fib_stash ();

  while (total > 0)
    {
      while ((idx > 0) && (stash[idx] - total > 0))
        {
          --idx;
        }
      total -= stash[idx];
    }
  opt_move = stash[idx];
  free (stash);

  if (opt_move > possible)
    return 1;

  return opt_move;
}

int
main ()
{
  int n, q, res;

  res = scanf ("%d%d", &n, &q);
  assert (res == 2);

  n = next_turn (n, q);
  printf ("%d\n", n);

  return 0;
}
