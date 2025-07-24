#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#define BUFF_SIZE 1000

int
main ()
{
  unsigned int x, y, nitems, counter, tmp;
  int in_y_num_system[BUFF_SIZE] = { 0 };
  nitems = scanf ("%ud%ud", &x, &y);
  if (nitems != 2)
    {
      printf ("Invalid args.");
      exit (1);
    }

  assert (x > 0);
  assert (y < 11);
  assert (y > 1);

  for (int i = 0;; i++)
    {
      tmp = x % y;
      in_y_num_system[i] = tmp;
      x = x / y;
      if (x < y)
        {
          in_y_num_system[i + 1] = x;
          counter = i + 1;
          break;
        }
    }

  for (int i = counter; i >= 0; i--)
    {
      printf ("%d", in_y_num_system[i]);
    }
  printf ("\n");
}
