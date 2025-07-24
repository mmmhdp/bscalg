#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

int
rcmp (const void *lhs, const void *rhs)
{
  int l, r;
  l = *((int *)lhs);
  r = *((int *)rhs);
  return r - l;
}

int
min (int a, int b)
{
  return (a - b < 0) ? a : b;
}

int
fill_break_table (int total, int ncoins, int *coins, int *table)
{
  int i;
  int cur;

  // 0 - is a lowest possible value for total
  // so it's a return point
  if (total == 0)
    return 0;
  // we somehow should signal, that way is unreacheable
  // since we pick later min, then INT_MAX won't be a chosen one
  if (total < 0)
    return INT_MAX;
  // meoize check part, we don't need a recursion call, if
  // calcualtions are already done
  if (table[total] != INT_MAX)
    return table[total];

  for (i = 0; i < ncoins; i++)
    {
      // recursive call to get V(T(x_t, a_t))
      cur = fill_break_table (total - coins[i], ncoins, coins, table);
      // if cur == INT_MAX - current path is not legit, we skip it
      if (cur == INT_MAX)
        continue;
      // if cur != INT_MAX - we can recalculate value in table
      // since we iterate over all possible options for exchange
      // we will be with the best possible value for table[total] after
      table[total] = min (table[total], cur + 1);
    }

  return table[total];
}

int
get_optimal_break (int sum, int ncoins, int *coins)
{
  int i, nbrk, *break_table;
  nbrk = 0;

  break_table = malloc (sizeof (int) * (sum + 1));
  for (i = 0; i < sum + 1; ++i)
    break_table[i] = (i == 0) ? 0 : INT_MAX;

  fill_break_table (sum, ncoins, coins, break_table);

  nbrk = break_table[sum];

  free (break_table);
  return nbrk;
}

int
main (void)
{
  int i, nbrkcoins, sum, nnoms, *coins;
  scanf ("%d", &sum);
  scanf ("%d", &nnoms);

  coins = malloc (sizeof (int) * nnoms);
  for (i = 0; i < nnoms; i++)
    scanf ("%d", coins + i);

  qsort (coins, nnoms, sizeof (int), rcmp);

  nbrkcoins = get_optimal_break (sum, nnoms, coins);

  printf ("%d\n", nbrkcoins);

  free (coins);
}
