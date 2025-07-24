#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void
c_swap (char *s, char *d)
{
  char t;
  t = *s, *s = *d, *d = t;
}

void
s_reverse (char *s, int len)
{
  char *l, *r;

  for (l = s, r = s + len - 1; l < r; l++, r--)
    {
      c_swap (l, r);
    }
}

void
s_substitute (char *s, char *ptrn)
{
  char *ptrn_srch;
  unsigned long long ptrn_len = strlen (ptrn);

  if (ptrn_len == 0)
    return;

  while ((ptrn_srch = strstr (s, ptrn)) != NULL)
    {
      s_reverse (ptrn_srch, ptrn_len);
      s = ptrn_srch + ptrn_len;
    }
}

void
s_fill (char *s, int len)
{
  int i;
  char c;

  i = 0;
  for (;;)
    {
      c = getchar ();
      if (isspace (c))
        continue;

      if (c == EOF)
        {
          s[i] = '\0';
          return;
        }

      s[i++] = c;
      break;
    }

  c = getchar ();
  while (c != '\n' && c != EOF && i < len)
    {
      s[i++] = c;
      c = getchar ();
    }

  s[len - 1] = '\0';
}

int
main (void)
{
  char *subs, *s;
  int ssubs, ss, subs_len, s_len;

  subs = NULL;
  s = NULL;

  scanf ("%d", &ssubs);
  subs_len = ssubs + 1;
  subs = malloc (subs_len);

  s_fill (subs, subs_len);

  scanf ("%d", &ss);
  s_len = ss + 1;
  s = malloc (s_len);

  s_fill (s, s_len);

  s_substitute (s, subs);

  printf ("%s", s);

  free (s);
  free (subs);

  return 0;
}
