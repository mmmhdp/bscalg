#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hshtbl.h"
#include "list.h"

#define HSH_P 9973
#define HSH_A 123
#define HSH_B 23

typedef struct hsh_val
{
  void *val;
  size_t sz;
} HSH_VAL;

typedef struct hsh_key
{
  char *key;
  unsigned long key_len;
} HSH_KEY;

typedef struct hsh_line
{
  LIST *line;
  HSH_KEY *last_inserted_key;
} HSH_LINE;

typedef NODE_DATA HSH_LINE_NODE_DATA;

typedef struct hsh_tbl
{
  int capacity;
  HSH_LINE **hlines;
} HSH_TBL;

int
ht_hsf (HSH_TBL *ht, HSH_KEY *k)
{
  unsigned long i;
  int hs;
  char c;

  if (k->key_len == 0)
    hs = 0;

  for (i = 0; i < k->key_len; i++)
    {
      c = k->key[i];
      hs = ((HSH_A * c + HSH_B) % HSH_P) % ht->capacity;
    }

  return hs;
}

