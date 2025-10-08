#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hshtbl.h"
#include "list.h"

#define _GNU_SOURCE

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
  uint32_t (*hsf) (const char *key, int key_len);
  HSH_LINE **hlines;
} HSH_TBL;

#if !defined(get16bits)
#define get16bits(d)                                                          \
  ((((uint32_t)(((const uint8_t *)(d))[1])) << 8)                             \
   + (uint32_t)(((const uint8_t *)(d))[0]))
#endif

uint32_t
fash_hsf (const char *data, int len)
{
  uint32_t hash = len, tmp;
  int rem;

  if (len <= 0 || data == NULL)
    return 0;

  rem = len & 3;
  len >>= 2;

  /* Main loop */
  for (; len > 0; len--)
    {
      hash += get16bits (data);
      tmp = (get16bits (data + 2) << 11) ^ hash;
      hash = (hash << 16) ^ tmp;
      data += 2 * sizeof (uint16_t);
      hash += hash >> 11;
    }

  /* Handle end cases */
  switch (rem)
    {
    case 3:
      hash += get16bits (data);
      hash ^= hash << 16;
      hash ^= ((signed char)data[sizeof (uint16_t)]) << 18;
      hash += hash >> 11;
      break;
    case 2:
      hash += get16bits (data);
      hash ^= hash << 11;
      hash += hash >> 17;
      break;
    case 1:
      hash += (signed char)*data;
      hash ^= hash << 10;
      hash += hash >> 1;
    }

  /* Force "avalanching" of final 127 bits */
  hash ^= hash << 3;
  hash += hash >> 5;
  hash ^= hash << 4;
  hash += hash >> 17;
  hash ^= hash << 25;
  hash += hash >> 6;

  return hash;
}

static int
naive_hsf (HSH_TBL *ht, HSH_KEY *k)
{
  unsigned long i;
  int hs;
  unsigned char c;

  hs = 0;
  if (k->key_len == 0)
    return hs;

  for (i = 0; i < k->key_len; i++)
    {
      c = k->key[i];
      hs = (hs + ((HSH_A * c + HSH_B) % HSH_P)) % ht->capacity;
    }

  return hs;
}

static int
hsf (HSH_TBL *ht, char *key, unsigned long key_len)
{
  int hs;
  HSH_KEY *k;

  k = key_init (key, key_len);
  hs = ht->hsf (k->key, k->key_len);
  key_free (k);

  return hs;
}

HSH_TBL *
ht_init (int ht_sz)
{
  int i;
  HSH_TBL *ht = calloc (1, sizeof (HSH_TBL));

  ht->hsf = fash_hsf;
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
  HSH_LINE *hl;
  int hash;

  hash = naive_hsf (ht, k);

  hl = ht->hlines[hash];
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
  hash = naive_hsf (ht, k);

  hl = ht->hlines[hash];
  hline_free (hl);

  ht->hlines[hash] = hline_init ();

  key_free (k);
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
ht_print (HSH_TBL *ht, void (*node_printer) (NODE_DATA *d, int is_top_node))
{
  int i;
  HSH_LINE *hl;

  if (node_printer == NULL)
    node_printer = line_node_data_printer_with_int_inner_value;

  for (i = 0; i < ht->capacity; i++)
    {
      hl = ht->hlines[i];
      if (hl->last_inserted_key == NULL)
        {
          printf ("Hash index: [%d] Key: [empty] \n", i);
          continue;
        }
      printf ("Hash index: [%d] Key: [%s] ", i, hl->last_inserted_key->key);

      list_print (hl->line, node_printer);
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
  hash = naive_hsf (ht, k);
  if (hash < 0 || hash >= ht->capacity)
    printf ("\nFUCKED HASH %d but capacity is %d\n", hash, ht->capacity);

  hl = ht->hlines[hash];

  if (hl->last_inserted_key == NULL)
    {
      key_free (k);
      return NULL;
    }

  n = list_get_tail_node (hl->line);
  d = list_node_get_data (n);
  v = list_node_data_get_value (d);

  key_free (k);
  return v;
}

void *
ht_get_value (HSH_VAL *v)
{
  if (v == NULL)
    return NULL;

  return v->val;
}

HSH_FUNC
ht_get_hsf (void) { return hsf; }
