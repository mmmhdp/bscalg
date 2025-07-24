#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct Stuff
{
  int nitems;
  int *weights;
  int *values;
} Stuff;

void
stuff_free (Stuff *st)
{
  if (!st)
    return;
  free (st->weights);
  st->weights = NULL;
  free (st->values);
  st->values = NULL;
  free (st);
  st = NULL;
}

typedef struct Backpack
{
  int weight;
  int optimal_packed_nitems;
  int optimal_packed_size;
  int optimal_packed_value;
  Stuff *stuff;
} Backpack;

void
backpack_free (Backpack *bp)
{
  stuff_free (bp->stuff);
  free (bp);
  bp = NULL;
}

int
max (int lhs, int rhs)
{
  return (lhs - rhs > 0) ? lhs : rhs;
}

void
show_table (int **t, int n, int m)
{
  int i, j;
  for (i = 0; i < n; i++)
    {
      for (j = 0; j < m; j++)
        {
          printf ("%10d ", t[i][j]);
        }
      printf ("\n");
    }
}

int
calc_pack (int *w, int *v, int nel, int W, int **V)
{
  int i, cw;
  for (i = 0; i < nel; i++)
    for (cw = 0; cw <= W; cw++)
      {
        if (i == 0)
          {
            if (w[0] <= cw)
              V[i][cw] = v[i];
            else
              V[i][cw] = 0;
          }
        else if (cw < w[i])
          V[i][cw] = V[i - 1][cw];
        else
          {
            V[i][cw] = max (V[i - 1][cw], V[i - 1][cw - w[i]] + v[i]);
          }
      }

  return V[nel - 1][W];
}

void
set_optimal_package (Backpack *bp)
{
  int i, **V;

  if (bp->weight <= 0 || bp->stuff->nitems <= 0)
    return;

  V = malloc (sizeof (int *) * bp->stuff->nitems);
  for (i = 0; i < bp->stuff->nitems; ++i)
    V[i] = calloc (bp->weight + 1, sizeof (int));

  bp->optimal_packed_nitems = calc_pack (bp->stuff->weights, bp->stuff->values,
                                         bp->stuff->nitems, bp->weight, V);

  for (i = 0; i < bp->stuff->nitems; ++i)
    free (V[i]);
  free (V);
}

int
main (void)
{
  int i, bpsz, nitems;

  scanf ("%d", &bpsz);
  scanf ("%d", &nitems);

  Stuff *st = malloc (sizeof (Stuff));
  st->nitems = nitems;
  st->weights = malloc (sizeof (int) * st->nitems);
  st->values = malloc (sizeof (int) * st->nitems);
  for (i = 0; i < nitems; i++)
    {
      scanf ("%d", st->weights + i);
      st->values[i] = 1;
    }

  Backpack *bp = malloc (sizeof (Backpack));
  bp->weight = bpsz;
  bp->optimal_packed_nitems = -1;
  bp->optimal_packed_size = -1;
  bp->optimal_packed_value = -1;
  bp->stuff = st;

  set_optimal_package (bp);

  printf ("%d\n", bp->optimal_packed_nitems);

  backpack_free (bp);
}
