#include "cache.h"
#include "hshtbl.h"
#include "list.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BASE_HSH_TBL_SIZE 28
#define KEY_PREFIX_SIZE 4
#define PREFIX "key_"

typedef struct cache
{
  unsigned long capacity;
  HSH_TBL *ht;
  LIST *cl;
} CACHE;

CACHE *
ch_init (unsigned long cache_capacity)
{
  CACHE *c = malloc (sizeof (CACHE));
  c->ht = ht_init (BASE_HSH_TBL_SIZE);
  c->cl = list_init ();
  c->capacity = cache_capacity;

  return c;
}

void
ch_free (CACHE *c)
{
  list_free (c->cl);
  ht_free (c->ht);
  free (c);
}

char *
ch_assemble_valid_key (void *value, size_t v_sz)
{
  size_t i;
  int *v;
  char *sv, *prefix, *buff;

  v = malloc (v_sz);
  memcpy (v, value, v_sz);
  *v += '0';

  sv = (char *)v;
  prefix = PREFIX;
  buff = calloc (v_sz + KEY_PREFIX_SIZE + 1, sizeof (char));

  i = 0;
  for (; i < KEY_PREFIX_SIZE; i++)
    buff[i] = prefix[i];

  for (; i < KEY_PREFIX_SIZE + v_sz; i++)
    buff[i] = *(sv + i - KEY_PREFIX_SIZE);

  buff[v_sz + KEY_PREFIX_SIZE] = '\0';

  free (v);

  return buff;
}

static void
ch_cache_hit_update (CACHE *c, HSH_VAL *ht_entrance)
{
  LIST_NODE **pcl_node, *node_to_promote;
  pcl_node = ht_get_value (ht_entrance);
  node_to_promote = *pcl_node;

  list_move_node_to_tail (c->cl, node_to_promote);
}

static void
ch_cl_val_copy (void **dst, void *value, size_t v_sz)
{
  NODE_DATA *tdst;

  tdst = malloc (sizeof (v_sz));
  memcpy (tdst, value, v_sz);

  *dst = tdst;
}

static LIST_NODE *
ch_add_to_cl (LIST *cl, void *value, size_t v_sz)
{
  LIST_NODE *added_node;
  added_node = list_add_node_by_caller (cl, value, v_sz, ch_cl_val_copy);

  return added_node;
}

static void
ch_create_ht_record (HSH_TBL *ht, void *raw_key, size_t key_sz,
                     LIST_NODE *new_cl_node)
{
  char *k;

  k = ch_assemble_valid_key (raw_key, key_sz);
  ht_add (ht, k, strlen (k), &new_cl_node, sizeof (LIST_NODE **));

  free (k);
}

static void
ch_clean_up_record (HSH_TBL *ht, void *raw_key, size_t raw_key_size)
{
  char *key;

  key = ch_assemble_valid_key (raw_key, raw_key_size);
  ht_delete (ht, key, strlen (key));

  free (key);
}

static void
ch_trim (CACHE *c)
{
  unsigned long len;
  LIST_NODE *least_used;
  NODE_DATA *least_used_data;
  void *raw_key_for_clean_up;
  size_t raw_key_for_clean_up_sz;

  len = list_get_len (c->cl);

  for (; len > c->capacity; len--)
    {
      least_used = list_get_top_node (c->cl);
      least_used_data = list_node_get_data (least_used);

      raw_key_for_clean_up = list_node_data_get_value (least_used_data);
      raw_key_for_clean_up_sz = list_node_data_get_value_sz (least_used_data);

      ch_clean_up_record (c->ht, raw_key_for_clean_up,
                          raw_key_for_clean_up_sz);
      list_delete_node (c->cl, least_used);
    }
}

static void
ch_cache_miss_update (CACHE *c, void *value, size_t v_sz)
{
  LIST_NODE *new_node;

  new_node = ch_add_to_cl (c->cl, value, v_sz);
  ch_create_ht_record (c->ht, value, v_sz, new_node);

  ch_trim (c);
}

void
ch_put (CACHE *c, void *value, size_t v_sz)
{
  HSH_VAL *ht_entrance;

  ht_entrance = ch_get (c, value, v_sz);
#if 1
  if (ht_entrance)
    ch_cache_hit_update (c, ht_entrance);
  else
    ch_cache_miss_update (c, value, v_sz);
#endif
}

HSH_VAL *
ch_get (CACHE *c, void *value, size_t v_sz)
{
  HSH_VAL *hv;
  void *prefixed_key;

  prefixed_key = ch_assemble_valid_key (value, v_sz);
  hv = ht_find (c->ht, prefixed_key, strlen (prefixed_key));
  free (prefixed_key);

  return hv;
}

static void
ch_list_node_data_printer_with_int_inner_value (NODE_DATA *d, int is_top_node)
{
  void *v;
  int *tv;
  int value;

  if (d == NULL)
    {
      printf ("(NULL node)");
      return;
    }

  v = list_node_data_get_value (d);
  tv = (int *)v;
  value = *tv;

  if (is_top_node)
    printf ("(Top node: %d)->", value);
  else
    printf ("(Node: %d)->", value);
}

void
ch_print (CACHE *c)
{
  printf ("Cache table:\n");
  ht_print (c->ht, NULL);
  printf ("\n");
  printf ("Cache:\n");
  list_print (c->cl, ch_list_node_data_printer_with_int_inner_value);
}
