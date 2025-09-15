#include "cache.h"
#include "hshtbl.h"
#include "llst.h"

#include <stdlib.h>

typedef enum
{
  LRU,
  ARC,
  LFU,
  LIRS,
  LRU_2,
  SLRU,
  RRIP,
  Q2
} CACHE_T;

typedef struct cache
{
  CACHE_T cache_t;
  int cache_sz;
  HSHTBL *hst;
  LLST *llst;
} CACHE;

CACHE *
cache_init (CACHE_T cache_t, int cache_sz)
{
  CACHE *ch = malloc (sizeof (CACHE));
  ch->cache_t = cache_t;
  ch->cache_sz = cache_sz;
  return ch;
}

void
cache_free (CACHE_T cache_t, int cache_sz)
{
}
