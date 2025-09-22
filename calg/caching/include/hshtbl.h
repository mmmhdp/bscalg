#pragma once
#include <stdint.h>

typedef struct hshtbl HSHTBL;

uint32_t
ht_hsf (char *key, uint32_t hstb_sz);

HSHTBL *
ht_init(uint32_t hash_table_size);

void
ht_free(HSHTBL * ht);

void
ht_print(HSHTBL * ht);

void
ht_add(HSHTBL *ht, char *key, uint32_t val);
