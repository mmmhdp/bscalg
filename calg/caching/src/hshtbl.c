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

static void line_node_data_printer_with_int_inner_value (NODE_DATA *d,
                                                         int is_top_node);

static void val_copy (void **dst, void *v, size_t v_sz);

static void hline_update_last_key (HSH_LINE *hl, HSH_KEY *k);

static void ht_add_k_v_pair (HSH_TBL *ht, HSH_KEY *k, HSH_VAL *v);

static HSH_KEY *key_init (char *key, unsigned long key_len);

static HSH_VAL *val_init (void *value, size_t v_sz);

static void key_free (HSH_KEY *k);

static void val_free (HSH_VAL *v);

static void hline_node_free (HSH_LINE_NODE_DATA *d);

static void hline_free (HSH_LINE *hl);

static HSH_LINE *hline_init (void);

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

typedef LIST LINE;
typedef NODE_DATA HSH_LINE_NODE_DATA;

typedef struct hsh_tbl
{
  int capacity;
  HSH_LINE **hlines;
} HSH_TBL;

int
hsf (HSH_TBL *ht, HSH_KEY *k)
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
    ht->hlines[i] = hline_init ();

  return ht;
}

static HSH_KEY *
key_init (char *key, unsigned long key_len)
{
  HSH_KEY *k;

  k = calloc (1, sizeof (HSH_KEY));
  k->key = malloc (key_len + 1);
  memcpy (k->key, key, key_len);
  k->key[key_len] = '\0';

  k->key_len = key_len;

  return k;
}

static HSH_VAL *
val_init (void *value, size_t v_sz)
{
  HSH_VAL *v;

  v = calloc (1, sizeof (HSH_VAL));
  v->val = malloc (v_sz);
  memcpy (v->val, value, v_sz);

  v->sz = v_sz;

  return v;
}

static HSH_LINE *
hline_init (void)
{
  HSH_LINE *hl;
  hl = calloc (1, sizeof (HSH_LINE));
  hl->last_inserted_key = NULL;
  hl->line = list_init ();

  return hl;
}

static void
key_free (HSH_KEY *k)
{
  if (k == NULL)
    return;
  free (k->key);
  free (k);
}

static void
val_free (HSH_VAL *v)
{
  if (v == NULL)
    return;
  free (v->val);
  free (v);
}

static void
hline_node_free (HSH_LINE_NODE_DATA *d)
{
  void *v;
  v = list_node_data_get_value (d);
  val_free (v);
}

static void
hline_free (HSH_LINE *hl)
{
  key_free (hl->last_inserted_key);
  list_free_by_caller (hl->line, hline_node_free);
  free (hl);
}

void
ht_free (HSH_TBL *ht)
{
  int i;

  if (ht == NULL)
    return;

  for (i = 0; i < ht->capacity; i++)
    hline_free (ht->hlines[i]);

  free (ht->hlines);
  free (ht);
}

static void
val_copy (void **dst, void *v, size_t v_sz)
{
  HSH_VAL *tv, *tdst;
  tv = v;

  tdst = malloc (v_sz);
  memcpy (tdst, v, v_sz);

  tdst->val = malloc (tv->sz);
  memcpy (tdst->val, tv->val, tv->sz);

  *dst = tdst;
}

static void
hline_update_last_key (HSH_LINE *hl, HSH_KEY *k)
{
  key_free (hl->last_inserted_key);
  hl->last_inserted_key = key_init (k->key, k->key_len);
}

static void
ht_add_k_v_pair (HSH_TBL *ht, HSH_KEY *k, HSH_VAL *v)
{
  int hash = hsf (ht, k);

  HSH_LINE *hl = ht->hlines[hash];
  list_add_node_by_caller (hl->line, v, sizeof (HSH_VAL), val_copy);

  hline_update_last_key (hl, k);
}

void
ht_add (HSH_TBL *ht, char *key, unsigned long key_len, void *value,
        size_t v_sz)
{
  HSH_KEY *k;
  HSH_VAL *v;

  k = key_init (key, key_len);
  v = val_init (value, v_sz);

  ht_add_k_v_pair (ht, k, v);

  key_free (k);
  val_free (v);
}

void
ht_delete (HSH_TBL *ht, char *key, unsigned long key_len)
{
  HSH_VAL *v;
  HSH_KEY *k;
  HSH_LINE *hl;
  int hash;

  v = ht_find (ht, key, key_len);

  if (v == NULL)
    return;

  k = key_init (key, key_len);
  hash = hsf (ht, k);

  hl = ht->hlines[hash];
  hline_free (hl);

  ht->hlines[hash] = hline_init ();
}

static void
line_node_data_printer_with_int_inner_value (NODE_DATA *d, int is_top_node)
{
  void *v;
  HSH_VAL *tv;
  int value;

  if (d == NULL)
    {
      printf ("(NULL node)");
      return;
    }

  v = list_node_data_get_value (d);
  tv = (HSH_VAL *)v;
  value = *((int *)tv->val);

  if (is_top_node)
    printf ("(Top node: %d)->", value);
  else
    printf ("(Node: %d)->", value);
}

void
ht_print (HSH_TBL *ht)
{
  int i;
  HSH_LINE *hl;

  for (i = 0; i < ht->capacity; i++)
    {
      hl = ht->hlines[i];
      if (hl->last_inserted_key == NULL)
        {
          printf ("Key: [empty] \n");
          continue;
        }
      printf ("Key: [%s] ", hl->last_inserted_key->key);
      list_print (hl->line, line_node_data_printer_with_int_inner_value);
      printf ("\n");
    }
}

HSH_VAL *
ht_find (HSH_TBL *ht, char *key, unsigned long key_len)
{
  HSH_VAL *v;
  HSH_KEY *k;
  HSH_LINE *hl;
  LIST_NODE *n;
  NODE_DATA *d;
  int hash;

  k = key_init (key, key_len);
  hash = hsf (ht, k);
  hl = ht->hlines[hash];

  if (hl->last_inserted_key == NULL)
    return NULL;

  n = list_get_tail_node (hl->line);
  d = list_node_get_data (n);
  v = list_node_data_get_value (d);

  return v;
}
