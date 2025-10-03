#pragma once

#include "list.h"
#include <stddef.h>

typedef struct cache CACHE;

CACHE *ch_init (unsigned long cache_capacity);

void ch_free (CACHE *c);

LIST_NODE *ch_get (CACHE *c, void *key, unsigned long key_len);

void ch_put (CACHE *c, void *value, size_t v_sz);

void ch_print (CACHE *c);
