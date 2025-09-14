#include <stdio.h>

#define PRIME 1
#define COMPOSITE 0
int
is_prime (unsigned x)
{
  if (x <= 1)
    return COMPOSITE;

  if (x <= 3)
    return PRIME;

  if (!(x & 0x1))
    return COMPOSITE;

  for (int judge = 3, step = 2; judge * judge <= x; judge += step)
    if (!(x % judge))
      return COMPOSITE;

  return PRIME;
}

extern int start (int x);

void
test_prime (int testv)
{
  int res;
  // res = start (testv);
  // res = is_prime(testv);
  if (res)
    printf ("%d is a prime\n", testv);
  else
    printf ("%d is a composite\n", testv);
}

int
main (void)
{
  test_prime (1);
  test_prime (2);
  test_prime (3);
  test_prime (4);
  test_prime (5);
  test_prime (6);
  test_prime (7);
  test_prime (8);
  test_prime (9);
  test_prime (10);
  test_prime (11);
  test_prime (12);
  return 0;
}
