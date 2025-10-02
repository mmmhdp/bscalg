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

HSH_TBL *
ht_init (int ht_sz)
{
  int i;
  HSH_TBL *ht = calloc (1, sizeof (HSH_TBL));

  ht->capacity = ht_sz;
  ht->hlines = calloc (ht->capacity, sizeof (HSH_LINE *));

  for (i = 0; i < ht->capacity; i++)
    {
      ht->hlines[i] = malloc (1 * sizeof (HSH_LINE));
      ht->hlines[i]->line = list_init ();
      ht->hlines[i]->last_inserted_key = NULL;
    }

  return ht;
}

static void
ht_val_free (HSH_VAL *v)
{
  free (v->val);
  free (v);
}

static void
ht_hline_node_free (HSH_LINE_NODE_DATA *d)
{
  void *v;
  v = list_node_data_get_value (d);
  ht_val_free (v);
}

static void
ht_free_hline (HSH_LINE *hl)
{
  free (hl->last_inserted_key);
  list_free_by_caller (hl->line, ht_hline_node_free);
  free (hl);
}

void
ht_free (HSH_TBL *ht)
{
  int i;

  for (i = 0; i < ht->capacity; i++)
    ht_free_hline (ht->hlines[i]);

  free (ht->hlines);
  free (ht);
}
