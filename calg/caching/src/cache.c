#include "cache.h"
#include "hshtbl.h"
#include "list.h"
#include <stdio.h>
#include <stdlib.h>

#define BASE_HSH_TBL_SIZE 15

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

void
ch_put (CACHE *c, void *value, size_t v_sz)
{
  LIST_NODE *v_node, *last_added_node, *top_node, *to_delete_node;
  NODE_DATA *d;
  HSH_VAL *to_delete_value;
  unsigned long cl_len;
  size_t to_delete_value_sz, node_sz;

  v_node = ch_get (c, value, v_sz);

  if (v_node == NULL)
    {
      list_add_node (c->cl, value, v_sz);
      last_added_node = list_get_tail_node (c->cl);
      node_sz = sizeof (LIST_NODE *);

      ht_add (c->ht, value, v_sz, &last_added_node, node_sz);

      cl_len = list_get_len (c->cl);
      if (c->capacity < cl_len)
        {
          top_node = list_get_top_node (c->cl);
          to_delete_node = list_node_get_next (top_node);

          d = list_node_get_data (to_delete_node);
          to_delete_value = (HSH_VAL *)list_node_data_get_value (d);
          to_delete_value_sz = list_node_data_get_value_sz (d);

          ht_delete (c->ht, (char *)to_delete_value, to_delete_value_sz);
          list_delete_node (c->cl, to_delete_node);
        }
    }
  else
    {
      list_move_node_to_tail (c->cl, v_node);
    }
}

LIST_NODE *
ch_get (CACHE *c, void *key, unsigned long key_len)
{
  HSH_VAL *hv;
  LIST_NODE *ln;

  hv = ht_find (c->ht, key, key_len);
  if (hv == NULL)
    return NULL;

  ln = (LIST_NODE *)ht_get_value (hv);
  return ln;
}

static void
cache_list_node_data_printer_with_int_inner_value (NODE_DATA *d,
                                                   int is_top_node)
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
  list_print (c->cl, cache_list_node_data_printer_with_int_inner_value);
}
