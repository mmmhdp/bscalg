#include <stdio.h>

int
foo (int *arr, int target, int arrsz)
{
  int tmp;
  int idx;
  int residx = 0;

  if (arrsz == 0)
    return residx;

  idx = arrsz - 1;
  if (idx < 0)
    return arrsz;

  while (idx > 0)
    {
      tmp = arr[idx];
      residx = idx;
      if (tmp <= target)
        {
          residx += 1;
          return residx;
        }
      else
        {
          arr[idx + 1] = tmp;
          idx -= 1;
        }
    }

  return residx;
}

int
my_add (int x, int y)
{
  int res = x;

  asm ("add %0, %1" : "+r"(res) : "r"(y) : "cc");

  return res;
}

int
main (void)
{
#if 0
  int x[] = { -1, 1, 5, 9 }, res;
  res = foo (x, 10, 3);
  printf ("%d", res);

#else
  int x, y, res;

  x = 1;
  y = 2;
  res = my_add (x, y);

  printf ("%d", res);

#endif
  return 0;
}
