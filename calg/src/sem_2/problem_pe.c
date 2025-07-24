#include <assert.h>
#include <limits.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#define ENDL (printf ("\n"));
#define TRUE 1
#define FALSE 0

struct sieve_t
{
  int n;
  char *s;
};

void
fill_sieve (struct sieve_t *sv)
{
  sv->s[0] |= (1);
  sv->s[0] |= (1 << 1);

  for (int i = 2; i * i < (sv->n * CHAR_BIT); i++)
    {
      int target = i;
      int box_idx = target / CHAR_BIT;
      int bit_idx = target % CHAR_BIT;
      if (!((sv->s[box_idx] >> bit_idx) & 1))
        {
          for (int bit_gap = target + target; bit_gap < (sv->n * CHAR_BIT);
               bit_gap += target)
            {
              int gap_box_idx = bit_gap / CHAR_BIT;
              int gap_bit_idx = bit_gap % CHAR_BIT;
              sv->s[gap_box_idx] |= (1 << gap_bit_idx);
            }
        }
    }
}

int
is_prime (struct sieve_t *sv, unsigned n)
{
  int target = n;
  int box_idx = target / CHAR_BIT;
  int bit_idx = target % CHAR_BIT;
  if (((sv->s[box_idx] >> bit_idx) & 1))
    return FALSE;
  return TRUE;
}

void
print_char_bitwise (int n, char *name)
{
  if (name != NULL)
    {
      while (*name != '\0')
        printf ("%c", *name++);
      ENDL;
    }

  for (int i = CHAR_BIT; i >= 0; i--)
    {
      printf ("%c", (n & (1 << i)) ? '1' : '0');
    }
}

int
sieve_bound (int num)
{
  double dnum, dres;
  if (num <= 20)
    return 100;
  dnum = num;
  dres = dnum * (log (dnum) + log (log (dnum)));
  return (int)round (dres);
}

int
main ()
{
  int n;
  struct sieve_t *s;

  n = 10;
  scanf ("%d", &n);

  s = (struct sieve_t *)malloc (sizeof (struct sieve_t));
  s->n = sieve_bound (n);
  s->s = (char *)calloc (s->n, sizeof (char));

  fill_sieve (s);

  if (is_prime (s, n))
    {
      printf ("%d IS A PRIME", n);
      ENDL
    }
  else
    {
      printf ("%d IS NOT A PRIME", n);
      ENDL
    }

  free (s->s);
  free (s);
}
