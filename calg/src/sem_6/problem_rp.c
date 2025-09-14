#include <fenv.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

struct FloatValue
{
  float base;
  unsigned sgn;
  unsigned exp;
  unsigned mnts;
};

void
fv_print (struct FloatValue *fv)
{
#if 0
  if (fv->sgn == 0)
    printf ("0x");
  printf ("%#x ", fv->sgn);
#endif
  if (fv->exp == 0)
    printf ("0x");
  printf ("%#x ", fv->exp);
  if (fv->mnts == 0)
    printf ("0x");
  printf ("%#x ", fv->mnts);
}

struct FloatValue
fv_create_from_numer_and_denom (int numer, int denom, int rnd_type)
{
  struct FloatValue fv;
  unsigned utmp;
  float ftmp, dtmp;

  fesetround (rnd_type);

  dtmp = (double)numer / (double)denom;
  ftmp = (float)dtmp;
  utmp = f_to_u (ftmp);

  fv.base = ftmp;
  fv.sgn = (utmp & (1 << 31)) >> 31;
  fv.exp = (utmp & (0xff << 23)) >> 23;
  fv.mnts = utmp & 0x007fffff;

  return fv;
}

int
fv_eq (struct FloatValue fv1, struct FloatValue fv2)
{
  int eq_sgn, eq_exp, eq_mnts;
  eq_sgn = fv1.sgn == fv2.sgn ? 1 : 0;
  eq_exp = fv1.exp == fv2.exp ? 1 : 0;
  eq_mnts = fv1.mnts == fv2.mnts ? 1 : 0;

  return (eq_sgn && eq_exp && eq_mnts);
}

int
main (void)
{
  int numer, denom, nitems;

  nitems = scanf ("%d%d", &numer, &denom);
  if (nitems != 2)
    abort ();

  struct FloatValue fv_down, fv_up;

  fv_down = fv_create_from_numer_and_denom (numer, denom, FE_DOWNWARD);
  fv_up = fv_create_from_numer_and_denom (numer, denom, FE_UPWARD);

  if (fv_eq (fv_up, fv_down))
    {
      fv_print (&fv_up);
    }
  else
    {
      if (fv_up.sgn)
        {
          fv_print (&fv_up);
          fv_print (&fv_down);
        }
      else
        {
          fv_print (&fv_down);
          fv_print (&fv_up);
        }
    }
}
