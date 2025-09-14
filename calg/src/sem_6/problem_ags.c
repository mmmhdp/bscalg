#include <stdio.h>

unsigned int
callee (unsigned int arg1, unsigned int arg2, unsigned int arg3)
{
  return (arg1 * arg2) % arg3;
}

unsigned int
caller (unsigned int base, unsigned int exp, unsigned int mod)
{
  int acc = 1, mult;

  if (exp == 0)
    return acc;

  mult = base % mod;

  while (exp != 0)
    {
      if (exp % 2 == 1)
        {
          acc = callee (acc, mult, mod);
          exp -= 1;
        }

      mult = callee (mult, mult, mod);
      exp >>= 1;
    }

  return acc;
}

int
main (void)
{
  // printf ("%d \n", caller (2, 1, 10));
}
