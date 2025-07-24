#include <assert.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

char *
strstrci (char const *needle, char const *haystack)
{
  int offset;
  char *subs;
  char *needle_buff, *haystack_buff;
  char *nb, *hb;
  char const *n, *h;

  subs = NULL;
  needle_buff = malloc (strlen (needle) + 1);
  haystack_buff = malloc (strlen (haystack) + 1);

  for (nb = needle_buff, n = needle; (*nb = tolower (*n)) != '\0'; n++, nb++)
    ;

  for (hb = haystack_buff, h = haystack; (*hb = tolower (*h)) != '\0';
       h++, hb++)
    ;

  subs = strstr (haystack_buff, needle_buff);
  if (!subs)
    {
      free (haystack_buff);
      free (needle_buff);
      return NULL;
    }

  offset = subs - haystack_buff;

  free (haystack_buff);
  free (needle_buff);

  return (char *)haystack + offset;
}

int
main (void)
{
  char const *needle = "Ab", *src = "abracadaBra";
  char *pos1, *pos2, *pos3;
  pos1 = strstrci (needle, src);
  assert (pos1 != NULL);
  pos2 = strstrci (needle, pos1 + 2);
  assert (pos2 != NULL);
  pos3 = strstrci (needle, pos2 + 2);
  assert (pos3 == NULL);
}
