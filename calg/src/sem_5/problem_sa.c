#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef unsigned long long ull;

char *
strcat_r (char *dest, const char *src, int *bufsz)
{
  char *tmp;
  ull catlen = strlen (dest) + strlen (src);
  ull catsz = catlen + 1;
  if (*bufsz < catsz)
    {
      tmp = realloc ((void *)dest, catsz);
      if (tmp)
        dest = tmp;
      else
        {
          fprintf (stderr, "Reallocation failed for dest: %s",
                   strerror (errno));
          abort ();
        }

      *bufsz = catsz;
    }

  strcat (dest, src);

  return dest;
}

int
main (void)
{
}
