#include <limits.h>
#include <stdio.h>

unsigned
msb (const void *const px, int byte_size)
{
  int bit_idx = 0, i, j;

  const unsigned char *byte_arr = (const unsigned char *)px;

  for (i = 0; i < byte_size; i++)
    {
      int byte = byte_arr[i];
      for (j = 0; j < CHAR_BIT; j++)
        {
          if (byte & (1 << j))
            bit_idx = i * CHAR_BIT + j;
        }
    }

  return bit_idx;
}

unsigned
lsb (const void *const px, int byte_size)
{
  int bit_idx = 0, i, j;

  const unsigned char *byte_arr = (const unsigned char *)px;

  for (i = 0; i < byte_size && !bit_idx; i++)
    {
      int byte = byte_arr[i];
      for (j = 0; j < CHAR_BIT && !bit_idx; j++)
        {
          if (byte & (1 << j))
            {
              bit_idx = i * CHAR_BIT + j;
            }
        }
    }

  return bit_idx;
}

int
main (void)
{
  int x;
  unsigned x_msb, x_lsb;

#if 1
  x = 269971274;
  printf ("For number %d\n", x);

  x_msb = msb (&x, sizeof (x));
  printf ("Most valuable bit index is: %d\n", x_msb);

  x_lsb = lsb (&x, sizeof (x));
  printf ("Least valuable bit index is: %d\n", x_lsb);
  printf ("\n");
#endif

#if 1
  x = 4;
  printf ("For number %d\n", x);

  x_msb = msb (&x, sizeof (x));
  printf ("Most valuable bit index is: %d\n", x_msb);

  x_lsb = lsb (&x, sizeof (x));
  printf ("Least valuable bit index is: %d\n", x_lsb);
  printf ("\n");
#endif

  return 0;
}
