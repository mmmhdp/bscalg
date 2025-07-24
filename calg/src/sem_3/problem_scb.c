#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef int (*cmp_t) (const void *lhs, const void *rhs);

int
cmp_int (void const *lhs, void const *rhs)
{
  int *plhs = (int *)lhs;
  int *prhs = (int *)rhs;
  if ((*plhs) < (*prhs))
    return -1;
  if ((*plhs) > (*prhs))
    return 1;
  return 0;
}

void *
cbsearch (const void *key, const void *base, int num, int size, cmp_t cmp)
{
  int il = 0, ir = num - 1, imid, byte_size = size * sizeof (char);
  void *tmp;

  for (; il <= ir;)
    {
      imid = il + (ir - il) / 2;

      tmp = (void *)((char *)base + imid * byte_size);

      if (cmp (key, tmp) == 0)
        return tmp;
      if (cmp (key, tmp) < 0)
        {
          ir = imid - 1;
          continue;
        }
      else
        {
          il = imid + 1;
          continue;
        }
    }

  return NULL;
}

int
main (void)
{
  int base[] = { 1, 2, 3, 4, 5, 6 }, key = 0, num = 6;
  cmp_t cmp = cmp_int;
  void *pkey = cbsearch ((void *)&key, (void *)base, num, sizeof (int), cmp);

  if (pkey)
    printf ("%d", *((int *)pkey));
  else
    printf ("no key found\n");
}
