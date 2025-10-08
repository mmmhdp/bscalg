#include <stdio.h>
#include <stdlib.h>

#include "cache.h"

// #define VERBOSE

int
main (void)
{
  CACHE *c;
  int i, tmp, cache_hit_cnt, nitems, cache_sz, q_sz, *q;
  void *value;
  size_t value_sz;

  cache_hit_cnt = 0;
  nitems = scanf ("%d", &cache_sz);
  if (nitems != 1)
    exit (1);
  nitems = scanf ("%d", &q_sz);
  if (nitems != 1)
    exit (1);

  q = malloc (sizeof (int) * q_sz);

  for (i = 0; i < q_sz; i++)
    {
      nitems = scanf ("%d", &tmp);
      if (nitems != 1)
        exit (1);
      q[i] = tmp;
    }

  c = ch_init (cache_sz);
  ch_reset_stats (c);

  value_sz = sizeof (int);
  for (i = 0; i < q_sz; i++)
    {
      value = (void *)(q + i);
      ch_put (c, value, value_sz);
    }

  cache_hit_cnt = ch_get_hits (c);

#ifdef VERBOSE
  int cache_miss_cnt, cache_tries_cnt;

  cache_miss_cnt = ch_get_misses (c);
  cache_tries_cnt = ch_get_tries (c);

  printf ("Current cache_size %d\n", cache_sz);
  printf ("misses [%d] hits [%d] with [%d] calls with nq [%d]\n",
          cache_miss_cnt, cache_hit_cnt, cache_tries_cnt, q_sz);
#endif
  printf ("%d", cache_hit_cnt);

  ch_free (c);
  free (q);

  return 0;
}
