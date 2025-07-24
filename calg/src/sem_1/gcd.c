#include <assert.h>
#include <math.h>
#include <stdio.h>

int
imax (int a, int b)
{
  return a > b ? a : b;
}
int
iabs (int a)
{
  return a > 0 ? a : -a;
}
int
gcd (int a, int b)
{
  assert (a != 0);
  assert (b != 0);

#if 0
  int max, min;
  if (a >= b) {
    max = a;
    min = b;
  } else {
    max = b;
    min = a;
  }
  int mod = max % min;

  if (mod == 0) {
    return min;
  } else {
    return gcd(min, mod);
  }
#endif

  return a % b == 0 ? iabs (b) : iabs (gcd (b, a % b));
}

void
read_input (int *pa, int *pb)
{
  printf ("Input a and b: ");
  int nitems = scanf ("%d%d", pa, pb);
  if (nitems != 2)
    {
      printf ("Error : invalid input, expected any integer a and b\n");
    }
}

int
main ()
{
  int a, b, res;
  read_input (&a, &b);
  res = gcd (a, b);
  printf ("%d\n", res);
}
