#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#undef DBG

unsigned
f_to_u (float x)
{
  unsigned u;
  memcpy (&u, &x, sizeof (unsigned));
  return u;
}

float
u_to_f (unsigned x)
{
  float f;
  memcpy (&f, &x, sizeof (float));
  return f;
}

unsigned
invert_odd_bits_in_frac (unsigned u)
{
  int i;
  int tb;
  for (i = 1; i < 22; i += 2)
    {
      tb = 1 << i;
      if (u & tb)
        u &= ~(1 << i);
      else
        u |= (1 << i);
    }

  return u;
}

int
main (void)
{
  float v, res;
  unsigned tmp, nitems;

  nitems = scanf ("%f", &v);
  if (nitems != 1)
    abort ();

#ifdef DBG
  printf ("%f\n", v);
#endif

  tmp = f_to_u (v);

#ifdef DBG
  printf ("tmp %b\n", tmp);
  printf ("tmp %x\n", tmp);
#endif

  tmp = invert_odd_bits_in_frac (tmp);

#ifdef DBG
  printf ("inv %b\n", tmp);
  printf ("inv %x\n", tmp);
#endif

  res = u_to_f (tmp);

  printf ("%.5f\n", res);
}
