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
