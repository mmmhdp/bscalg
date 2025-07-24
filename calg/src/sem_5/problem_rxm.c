#include <regex.h>
#include <stdio.h>
#include <stdlib.h>

#define FINAL

void
s_swap (char *s, int src, int dst)
{
  char tmp;
  tmp = s[src], s[src] = s[dst], s[dst] = tmp;
}

void
s_reverse (char *s, int len)
{
  int l, r;
  l = 0, r = len - 1;
  while (l < r)
    {
      s_swap (s, l, r);
      l++;
      r--;
    }
}

void
s_substitute_rev (char *s, char *pattern)
{
  int reg_res, offset;
  char *cur;
  regex_t regex;
  regmatch_t *m;

  reg_res = regcomp (&regex, pattern, REG_EXTENDED);
  m = malloc (sizeof (regmatch_t));

  offset = 0;
  cur = s;
  while ((reg_res = regexec (&regex, cur, 1, m, 0)) == 0)
    {
      int start = m->rm_so;
      int end = m->rm_eo;
      s_reverse (s + offset + start, end - start);

      offset += end;
      cur += end;
    }

  regfree (&regex);
  free (m);
}

int
main (void)
{
  int i, pattern_size, s_size;
  char *pattern, *s, tmp;

  scanf ("%d ", &pattern_size);
  pattern = malloc (pattern_size + 1);

  for (i = 0; i < pattern_size; i++)
    {
      scanf ("%c", &tmp);
      pattern[i] = tmp;
    }
  pattern[pattern_size] = '\0';

#ifndef FINAL
  printf ("Pattern :||%s||\n", pattern);
#endif

  scanf ("%d ", &s_size);
  s = malloc (s_size + 1);

  for (i = 0; i < s_size; i++)
    {
      scanf ("%c", &tmp);
      s[i] = tmp;
    }
  s[s_size] = '\0';

#ifndef FINAL
  printf ("s :||%s||\n", s);
#endif

  s_substitute_rev (s, pattern);
  printf ("%s", s);

  free (s);
  free (pattern);
}
