#include <assert.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "list.h"
#include "utils.h"

static NODE_DATA *list_node_data_init (const void *v, size_t vsz);
static NODE_DATA *list_node_data_init_by_caller (
    const void *v, size_t vsz,
    void (*val_copy) (void **dst, const void *v, size_t v_sz));
static LIST_NODE *list_node_init (NODE_DATA *data, LIST_NODE *prev,
                                  LIST_NODE *next);
static void list_node_data_free (NODE_DATA *d);
static void list_node_free (LIST_NODE *n);
static void list_node_free_by_caller (LIST_NODE *n,
                                      void (*free_val) (NODE_DATA *d));
static void list_add_complete_node (LIST *l, LIST_NODE *n);
static int list_nodes_link (LIST *l, LIST_NODE *nparent, LIST_NODE *nchild);

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
list_node_get_data (LIST_NODE *n)
{
  return n->data;
}

void *
list_node_data_get_value (NODE_DATA *d)
{
  return d->value;
}

size_t
list_node_data_get_value_sz (NODE_DATA *d)
{
  return d->sz;
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

int
list_get_len (LIST *l)
{
  return l->list_len;
}

size_t
list_node_get_size (void)
{
  return sizeof (LIST_NODE);
}

LIST *
list_init (void)
{
  LIST *l = calloc (1, sizeof (LIST));
  if (!l)
    {
      ERROR ("Not enough memory to allocate LIST");
      goto fail;
    }

  return l;

fail:
  return NULL;
}

static NODE_DATA *
list_node_data_init (const void *v, size_t vsz)
{
  NODE_DATA *d;

  if (!v || vsz == 0)
    {
      ERROR ("Allocation of NODE_DATA with v = NULL or vsize = 0");
      goto fail;
    }

  d = calloc (1, sizeof (NODE_DATA));
  if (!d)
    {
      ERROR ("Not enough memory for NODE_DATA allocation");
      goto fail;
    }

  d->value = malloc (vsz);
  if (!d->value)
    {
      list_node_data_free (d);
      ERROR ("Not enough memory for value inside NODE_DATA allocation");
      goto fail;
    }

  memcpy (d->value, v, vsz);
  d->sz = vsz;
  return d;

fail:
  return NULL;
}

static NODE_DATA *
list_node_data_init_by_caller (const void *v, size_t vsz,
                               void (*val_copy) (void **dst, const void *v,
                                                 size_t v_sz))
{
  NODE_DATA *d;

  d = calloc (1, sizeof (NODE_DATA));
  if (!d)
    {
      ERROR ("Not enough memory for NODE_DATA allocation");
      goto fail;
    }

  val_copy (&(d->value), v, vsz);
  d->sz = vsz;

  return d;

fail:
  return NULL;
}

static LIST_NODE *
list_node_init (NODE_DATA *data, LIST_NODE *prev, LIST_NODE *next)
{
  LIST_NODE *n = calloc (1, sizeof (LIST_NODE));
  if (!n)
    {
      ERROR ("Not enough memory for LIST_NODE allocation");
      goto fail;
    }

  n->next = next;
  n->prev = prev;
  n->data = data;
  return n;

fail:
  return NULL;
}

static void
list_node_data_free (NODE_DATA *d)
{
  if (!d)
    return;

  free (d->value);
  free (d);
}

static void
list_node_data_free_by_caller (NODE_DATA *d, void (*free_val) (NODE_DATA *d))
{
  if (!d)
    return;

  free_val (d);
  free (d);
}

static void
list_node_free (LIST_NODE *n)
{
  if (!n)
    return;

  list_node_data_free (n->data);
  free (n);
}

static void
list_node_free_by_caller (LIST_NODE *n, void (*free_val) (NODE_DATA *d))
{
  if (!n)
    return;

  list_node_data_free_by_caller (n->data, free_val);
  free (n);
}

void
list_free (LIST *l)
{
  LIST_NODE *t, *tn;

  t = l->top;
  if (!t)
    {
      free (l);
      return;
    }

  while (t != NULL)
    {
      tn = t->next;
      list_delete_node (l, t);
      if (!tn)
        {
          free (l);
          return;
        }
      t = tn;
    }

  free (l);
}

void
list_free_by_caller (LIST *l, void (*free_val) (NODE_DATA *d))
{
  LIST_NODE *t, *tn;

  t = l->top;
  if (!t)
    {
      free (l);
      return;
    }

  while (t != NULL)
    {
      tn = t->next;
      list_delete_node_by_caller (l, t, free_val);
      if (!tn)
        {
          free (l);
          return;
        }
      t = tn;
    }

  free (l);
}

int
list_delete_node (LIST *l, LIST_NODE *n)
{
  LIST_NODE *tn;

  tn = list_node_find (l, n);
  if (!tn)
    {
      ERROR (
          "Attempt to delete node %p that doesn't belong to provided list %p",
          (void *)n, (void *)l);
      return 1;
    }

  list_nodes_link (l, tn->prev, tn->next);
  list_node_free (tn);
  l->list_len -= 1;

  return 0;
}

int
list_delete_node_by_caller (LIST *l, LIST_NODE *n,
                            void (*free_val) (NODE_DATA *d))
{
  int error_code;
  LIST_NODE *tn;

  tn = list_node_find (l, n);
  if (!tn)
    {
      ERROR (
          "Attempt to delete node %p that doesn't belong to provided list %p",
          (void *)n, (void *)l);
      return 1;
    }

  error_code = list_nodes_link (l, tn->prev, tn->next);
  if (error_code != 0)
    abort ();

  list_node_free_by_caller (tn, free_val);
  l->list_len -= 1;

  return 0;
}

static void
list_add_complete_node (LIST *l, LIST_NODE *n)
{
  if (!l || !n)
    return;

  n->next = NULL;
  if (!l->tail)
    {
      n->prev = NULL;
      l->top = l->tail = n;
    }
  else
    {
      l->tail->next = n;
      n->prev = l->tail;
      l->tail = n;
    }

  l->list_len += 1;
}

LIST_NODE *
list_add_node (LIST *l, void *v, int vsz)
{
  NODE_DATA *d;
  LIST_NODE *n;

  d = list_node_data_init (v, vsz);
  n = list_node_init (d, l->tail, NULL);

  l->list_len += 1;

  if (l->top == NULL)
    {
      l->top = l->tail = n;
      return n;
    }

  l->tail->next = n;
  l->tail = n;

  return n;
}

LIST_NODE *
list_add_node_by_caller (LIST *l, const void *v, int vsz,
                         void (*val_copy) (void **dst, const void *v,
                                           size_t v_sz))
{
  NODE_DATA *d;
  LIST_NODE *n;

  d = list_node_data_init_by_caller (v, vsz, val_copy);
  n = list_node_init (d, l->tail, NULL);

  l->list_len += 1;

  if (l->top == NULL)
    {
      l->top = l->tail = n;
      return n;
    }

  l->tail->next = n;
  l->tail = n;

  return n;
}

LIST_NODE *
list_node_find (LIST *l, LIST_NODE *n)
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
list_nodes_link (LIST *l, LIST_NODE *lhs, LIST_NODE *rhs)
{
  if (!lhs && !rhs)
    return 0;

  if (!lhs)
    {
      l->top = rhs;
      rhs->prev = NULL;
      return 0;
    }

  if (!rhs)
    {
      l->tail = lhs;
      lhs->next = NULL;
      return 0;
    }

  lhs->next = rhs;
  rhs->prev = lhs;

  return 0;
}

void
list_move_node_to_tail (LIST *l, LIST_NODE *n)
{
  LIST_NODE *nprev, *nnext;
  int linked_with_problems;

  if (!n || !l)
    return;

  if (!list_node_find (l, n))
    {
      ERROR (
          "Attempt to move node %p, that doesn't belong to provided list %p\n",
          (void *)n, (void *)l);
      return;
    }

  if (l->tail == n)
    return;

  nprev = n->prev;
  nnext = n->next;

  linked_with_problems = list_nodes_link (l, nprev, nnext);
  if (linked_with_problems)
    {
      ERROR ("Attempt to link nodes %p and %p, "
             "and list %p enden with errcode %d",
             (void *)nprev, (void *)nnext, (void *)l, linked_with_problems);
    }

  list_add_complete_node (l, n);
  /*
   * To balance l->list_len change in list_add_complete_node
   * */
  l->list_len -= 1;
}

void **
list_convert_to_arr (LIST *l)
{
  int i;
  LIST_NODE *tn;
  void **arr;

  arr = calloc (l->list_len, sizeof (void *));
  if (!arr)
    {
      ERROR ("Not enough memory to allocate arr for list to arr conversion");
      goto fail;
    }

  for (tn = l->top, i = 0; tn != NULL; tn = tn->next)
    memcpy (arr[i], tn->data->value, tn->data->sz);

  return arr;

fail:
  return NULL;
}

void
list_print (LIST *l, void (*node_printer) (NODE_DATA *d, int is_top_node))
{
  LIST_NODE *tn;

  tn = l->top;
  if (!tn)
    {
      printf ("{Empty list}");
      return;
    }

  node_printer (tn->data, TRUE);

  tn = tn->next;
  while (tn)
    {
      node_printer (tn->data, FALSE);
      tn = tn->next;
    }

  node_printer (NULL, FALSE);
}

void
list_print_pointers (LIST *l)
{
  LIST_NODE *tn;

  tn = l->top;
  if (!tn)
    {
      printf ("{Empty list}");
      return;
    }

  printf ("(Top node p: [%p])->", (void *)tn);

  tn = tn->next;
  while (tn)
    {
      printf ("(Node p: [%p])->", (void *)tn);
      tn = tn->next;
    }

  printf ("(NULL node p: [%p])", (void *)tn);
}

LIST_ARR *
list_arr_init (LIST *l)
{
  LIST_ARR *la;
  LIST_NODE *ln;

  size_t la_sz;
  size_t *la_sizes, *tmp;
  void *arr;
  int idx, copy_offset;

  if (!l || !l->top)
    goto fail;

  la = calloc (1, sizeof (LIST_ARR));
  if (!la)
    {
      ERROR ("Fail to allocate memory for an arr of list as array "
             "representation");
      goto fail;
    }

  idx = 0;
  la_sz = 0;
  copy_offset = 0;
  arr = NULL;
  la_sizes = calloc (l->list_len, sizeof (size_t));
  if (!la)
    {
      ERROR ("Fail to allocate memory for an arr of list as array "
             "representation");
      goto fail;
    }

  ln = l->top;

  while (ln)
    {
      la_sizes[idx] = ln->data->sz;
      copy_offset = la_sz;
      la_sz += ln->data->sz;

      tmp = realloc (arr, la_sz);
      if (!tmp)
        {
          ERROR ("Memory reallocation for next node in array failed during "
                 "transition from list to array");
          free (la);
          free (la_sizes);
          goto fail;
        }

      arr = tmp;
      memcpy ((char *)arr + copy_offset, ln->data->value, ln->data->sz);

      ln = ln->next;
      idx++;
    }

  la->el_szs = la_sizes;
  la->arr_sz = l->list_len;
  la->arr = arr;

  return la;

fail:

  return NULL;
}

void
list_arr_free (LIST_ARR *la)
{
  if (!la)
    return;
  free (la->arr);
  free (la->el_szs);
  free (la);
}

int
list_arr_is_equal (LIST_ARR *l, LIST_ARR *r,
                   int (*is_equal) (void *lhs, void *rhs))
{
  int i, loffset, roffset;
  char *pla, *pra;

  if (l->arr_sz != r->arr_sz)
    return FALSE;
  for (i = 0; i < (int)l->arr_sz; i++)
    if (l->el_szs[i] != r->el_szs[i])
      return FALSE;

  pla = (char *)l->arr;
  pra = (char *)r->arr;

  loffset = 0;
  roffset = 0;
  for (i = 0; i < (int)l->arr_sz; i++)
    {
      if (!is_equal (pla + loffset, pra + roffset))
        return FALSE;

      loffset += l->el_szs[i];
      roffset += r->el_szs[i];
    }

  return TRUE;
}
