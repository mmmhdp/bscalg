#include <stdlib.h>
#if 1
#define HASH_MAX 100
#endif

typedef int (*get_hash_t) (const char *s);
int ncollisions (char **strs, int n, get_hash_t f);

int
ncollisions (char **strs, int n, get_hash_t f)
{
  int i, *bf, h, ncolls;

  ncolls = 0;
  bf = calloc (HASH_MAX, sizeof (int));

  for (i = 0; i < n; i++)
    {
      h = f (strs[i]);
      if (bf[h]++ != 0)
        ncolls++;
    }

  free (bf);

  return ncolls;
}
int
main (void)
{
}
