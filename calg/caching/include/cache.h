#pragma once

#include "hshtbl.h"
#include "list.h"
#include <stddef.h>

typedef struct cache CACHE;

CACHE *ch_init (unsigned long cache_capacity);

void ch_free (CACHE *c);

HSH_VAL *ch_get (CACHE *c, void *key, unsigned long key_len);

void ch_put (CACHE *c, void *value, size_t v_sz);

void ch_print (CACHE *c);

char *ch_assemble_valid_key (void *value, size_t v_sz);

int ch_get_hits (CACHE *c);

int ch_get_misses (CACHE *c);

int ch_get_tries (CACHE *c);

void ch_reset_stats (CACHE *c);
