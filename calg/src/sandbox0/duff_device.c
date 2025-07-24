#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void
s_copy (char *s, char *d, int cnt)
{
  int n;
  n = (cnt + 3) / 4;
  switch (cnt % 4)
    {
    case 0:
      do
        {
          *d++ = *s++;
          /* fall through */
        case 3:
          *d++ = *s++;
          /* fall through */
        case 2:
          *d++ = *s++;
          /* fall through */
        case 1:
          *d++ = *s++;
        }
      while (--n > 0);
    }
}

int
main (void)
{
  char *src = "Hello World!", *dst;
  dst = malloc (strlen (src) + 1);

  s_copy (src, dst, strlen (src) + 1);

  printf ("%s\n", dst);

  free (dst);
}
