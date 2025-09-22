#include <assert.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

#include "list.h"
#include "utils.h"

typedef struct node_data
{
  void *value;
  size_t sz;
} NODE_DATA;

typedef struct node
{
  LIST_NODE *next;
  LIST_NODE *prev;
  NODE_DATA *data;
} LIST_NODE;

typedef struct list
{
  LIST_NODE *top;
  LIST_NODE *tail;
  int list_len;
} LIST;


LIST *
list_init (void)
{
  LIST *l = calloc (1, sizeof(LIST));
  return l;
}

static void
list_node_data_free (NODE_DATA *d)
{
  if (!d)
    return;

  free(d->value);
  free(d);
}

static NODE_DATA *
list_node_data_init(void *v, size_t vsz)
{
  NODE_DATA *d = calloc(1, sizeof(NODE_DATA));
  memcpy(d->value, v, vsz);
  d->sz = vsz;
  return d;
}

static void
list_node_free (LIST_NODE *n)
{
  if (!n)
    return;

  list_node_data_free(n->data);
  free(n);
}

static LIST_NODE *
list_node_init(NODE_DATA *data, LIST_NODE *prev, LIST_NODE *next)
{
  LIST_NODE *n = calloc(1, sizeof(LIST_NODE));
  n->next = next;
  n->prev = prev;
  n->data = data;
  return n;
}

void
list_free (LIST *l)
{
  LIST_NODE *t, *tn;

  t = l->top;
  if (!t)
  {
    free(l);
    return;
  }

  while (t)
  {
    tn = t->next;
    if (!tn)
    {
      list_node_free(t);
      free(l);
      return;
    }
    t = tn;
  }

  free(l);
}


void
list_add_node (LIST *l, void *v, int vsz)
{
  NODE_DATA *d;
  LIST_NODE *n;

  d = list_node_data_init(v, vsz);
  n = list_node_init(d, l->tail, NULL);
  
  l->tail->next = n;
  l->tail = n;

  l->list_len += 1;
}

static void
list_add_complete_node (LIST *l, LIST_NODE *n)
{
  n->next = NULL;
  l->tail->next = n;
  l->tail = n;

  l->list_len += 1;
}

LIST_NODE *
list_node_find(LIST *l, LIST_NODE *n)
{
  LIST_NODE *tn;

  tn = l->top;
  while (tn)
  {
    if (tn == n)
      return tn;

    tn = tn->next;
  }

  return NULL;
}

/*
 * Returns
 * 0 on success;
 * 1 if one of nodes doesn't belong to list;
 * */
static int
list_nodes_link(LIST *l, LIST_NODE *nparent, LIST_NODE *nchild )
{
  LIST_NODE *tnp, *tnc;

  if (!nparent)
  {
    tnc = list_node_find(l, nchild);
    if (tnc)
      tnc->prev = NULL;
    return 0;
  }

  if (!nchild)
  {
    tnp = list_node_find(l, nparent);
    if (tnp)
      tnp->next = NULL;
    return 0;
  }

  tnp = list_node_find(l, nparent);
  tnc = list_node_find(l, nchild);

  if (!tnp || !tnc)
  {
    ERROR("Attempt to link nodes %p and %p, but they don't belong to provided list %p",
          (void *) nparent, (void *) nchild,  (void *) l);
    return 1;
  }

  assert(nparent != NULL);
  assert(nchild != NULL);

  nparent->next = nchild;
  nchild->prev = nparent;

  return 0;
}

int
list_delete_node (LIST *l, LIST_NODE *n)
{
  int error_code;
  LIST_NODE *tn;

  tn = list_node_find(l, n);
  if (!tn)
  {
    ERROR("Attempt to delete node %p that doesn't belong to provided list %p",
          (void *) n, (void *) l);
    return 1;
  }

  error_code = list_nodes_link(l, tn->prev, tn->next);
  if (error_code != 0)
    abort();

  list_node_free(tn);
  l->list_len -= 1;

  return 0;
}


LIST_NODE *
list_node_get_prev (LIST_NODE *n)
{
  return n->prev;
}

LIST_NODE *
list_node_get_next (LIST_NODE *n)
{
  return n->next;
}

NODE_DATA *
list_node_data_get_data (LIST_NODE *n)
{
  return n->data;
}


LIST_NODE *
list_get_top_node (LIST *l)
{
  return l->top;
}

LIST_NODE *
list_get_tail_node (LIST *l)
{
  return l->tail;
}


void
list_move_node_to_tail (LIST *l, LIST_NODE *n)
{
  LIST_NODE *nprev, *nnext;
  if (!n)
    return;

  nprev = n->prev; 
  nnext = n->next;
  
  list_nodes_link(l, nprev, nnext);
  list_add_complete_node(l, n);
  /* 
   * To balance l->list_len change in list_add_complete_node
   * */
  l->list_len -= 1;
}


int
list_get_len (LIST *l)
{
  return l->list_len;
}

void **
list_convert_to_arr (LIST *l)
{
  int i;
  LIST_NODE *tn;
  void **arr;

  arr = calloc(l->list_len, sizeof(void *));

  for (tn = l->top, i = 0; tn != NULL; tn = tn->next)
    memcpy(arr[i], tn->data->value, tn->data->sz);

  return arr;
}


void
list_print (LIST *l, void (*node_printer) (NODE_DATA *d))
{
  LIST_NODE* tn; 

  tn = l->top;
  if (!tn)
  {
    printf("{Empty list}");
    return;
  }

  while (tn)
  {
    node_printer(tn->data);
    printf("\n");

    tn = tn->next;
  }
  printf("\n");
}
