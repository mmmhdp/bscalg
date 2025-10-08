#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cache.h"
#include "hshtbl.h"
#include "list.h"
#include "utils.h"

#define BASE_HSH_TBL_SIZE 4096
#define KEY_PREFIX_SIZE 4
#define PREFIX "key_"

typedef struct cache_stats
{
  int nhits;
  int nmiss;
  int ntries;
} CACHE_STATS;

typedef struct cache
{
  unsigned long capacity;
  HSH_TBL *ht;
  LIST *cl;
  CACHE_STATS *stats;
} CACHE;

static CACHE_STATS *stats_init (void);

CACHE *
ch_init (unsigned long cache_capacity)
{
  CACHE *c = calloc (1, sizeof (CACHE));
  c->ht = ht_init (BASE_HSH_TBL_SIZE);
  c->cl = list_init ();
  c->capacity = cache_capacity;
  c->stats = stats_init ();

  return c;
}

typedef enum stats_update_target
{
  HIT,
  MISS,
  TRY
} STATS_UPDATE_TARGET;

static void
stats_update (CACHE_STATS *s, STATS_UPDATE_TARGET t)
{
  switch (t)
    {
    case HIT:
      s->nhits += 1;
      break;
    case MISS:
      s->nmiss += 1;
      break;
    case TRY:
      s->ntries += 1;
      break;
    default:
      return;
    }
}

static CACHE_STATS *
stats_init (void)
{
  CACHE_STATS *stats;

  stats = calloc (1, sizeof (CACHE_STATS));
  return stats;
}

static void
stats_free (CACHE_STATS *s)
{
  free (s);
}

void
stats_reset (CACHE_STATS *s)
{
  s->nhits = 0;
  s->nmiss = 0;
  s->ntries = 0;
}

int
ch_get_hits (CACHE *c)
{
  return c->stats->nhits;
}

int
ch_get_misses (CACHE *c)
{
  return c->stats->nmiss;
}

int
ch_get_tries (CACHE *c)
{
  return c->stats->ntries;
}

void
ch_reset_stats (CACHE *c)
{
  c->stats->nhits = 0;
  c->stats->ntries = 0;
  c->stats->nmiss = 0;
}

void
ch_free (CACHE *c)
{
  list_free (c->cl);
  ht_free (c->ht);
  stats_free (c->stats);
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

  stats_update (c->stats, HIT);
}

static void
ch_cl_val_copy (void **dst, void *value, size_t v_sz)
{
  NODE_DATA *tdst;

  tdst = malloc (v_sz);
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

  stats_update (c->stats, MISS);
}

void
ch_is_hit_legit_check (void *value, HSH_VAL *ht_entrance)
{
  LIST_NODE **pn, *n;
  NODE_DATA *nd;
  int curr, hv;
  curr = *((int *)value);

  pn = ht_get_value (ht_entrance);
  n = *pn;
  nd = list_node_get_data (n);

  hv = *((int *)list_node_data_get_value (nd));

  if (curr != hv)
    {
      ERROR ("CACHE HIT IS NOT LEGIT!\n hv [%d] != curr [%d]\n", hv, curr);
    }
}

void
ch_put (CACHE *c, void *value, size_t v_sz)
{
  HSH_VAL *ht_entrance;

  ht_entrance = ch_get (c, value, v_sz);

  stats_update (c->stats, TRY);

  if (ht_entrance)
    {
#ifdef HSH_CHECK_COLL
      ch_is_hit_legit_check (value, ht_entrance);
#endif
      ch_cache_hit_update (c, ht_entrance);
    }
  else
    ch_cache_miss_update (c, value, v_sz);
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
