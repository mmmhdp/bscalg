#pragma once
#include <stdint.h>

#include "list.h"

typedef struct hshtbl HSHTBL;
typedef struct hsh_val HSH_VAL;

typedef struct hsh_key HSH_KEY;

typedef struct hsh_line HSH_LINE;

typedef NODE_DATA HSH_LINE_NODE_DATA;

typedef struct hsh_tbl HSH_TBL;

HSH_TBL *ht_init (int ht_sz);

void ht_free (HSH_TBL *ht);

void ht_add (HSH_TBL *ht, char *key, unsigned long key_len, void *value,
             size_t v_sz);
void ht_delete (HSH_TBL *ht, char *key, unsigned long key_len);

void ht_print (HSH_TBL *ht);

HSH_VAL *ht_find (HSH_TBL *ht, char *key, unsigned long key_len);

void *ht_get_value (HSH_VAL *v);
