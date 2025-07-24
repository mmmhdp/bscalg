#include <stdio.h>
#include <stdlib.h>

#define BUFF_SIZE 1000

int
main ()
{
  unsigned int x, nitems, max_frac_basis, tmp;
  int factorial_buff[BUFF_SIZE] = { 1 };
  nitems = scanf ("%ud", &x);
  if (nitems != 1)
    {
      printf ("Invalid args.");
      exit (1);
    }

  max_frac_basis = 1;
  for (int i = 1;; i++)
    {
      if (x >= max_frac_basis)
        {
          max_frac_basis *= i;
        }
      else
        {
          max_frac_basis = i - 2;
          break;
        }
    }

  for (int i = 1; i < max_frac_basis; i++)
    {
      factorial_buff[i] = factorial_buff[i - 1] * (i + 1);
    }

  for (int i = max_frac_basis - 1; i >= 0; i--)
    {
      tmp = x % factorial_buff[i];
      factorial_buff[i] = x / factorial_buff[i];
      x = tmp;
    }

  for (int i = max_frac_basis - 1; i >= 0; i--)
    {
      printf ("%d.", factorial_buff[i]);
    }
  printf ("\n");
}
