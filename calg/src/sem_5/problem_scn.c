#include <stdio.h>

int
main (void)
{
  int sum;
  char tmp;

  sum = 0;

  while (scanf ("%c", &tmp) != EOF)
    {
      sum += tmp;
    }

  printf ("%d\n", sum);
}
