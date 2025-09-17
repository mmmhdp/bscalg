#include <assert.h>
#include <stdarg.h>
#include <stdlib.h>

#include "llst.h"
#include "utils.h"

typedef struct llst
{
  struct llst *top;
  struct llst *tail;
  struct llst *nxt;
  struct llst *prev;
  int v;
} LLST;

LLST *
llst_init (void)
{
  LLST *head = calloc (1, sizeof (LLST));
  head->top = head;
  head->tail = head;
  return head;
}

void
llst_free (LLST *top)
{
  LLST *tmp;
  while (top != NULL)
    {
      tmp = top->nxt;
      free (top);
      top = tmp;
    }
}

void
llst__free_node (LLST *node)
{
  if (node == NULL)
    return;

  if (node == node->top)
    {
      llst_free (node->top);
      return;
    }

  free (node);
}

LLST *
llst_add_node (LLST *head, int value)
{
  LLST *tmp = malloc (sizeof (LLST));
  tmp->v = value;
  tmp->prev = head;
  tmp->top = head->top;
  tmp->top->tail = tmp;

  head->nxt = tmp;
  head = head->nxt;

  return head;
}

LLST *
llst_delete_node (LLST *node)
{
  LLST *prev_node;
  prev_node = node->prev;

  if (node->nxt != NULL)
    {
      node->prev->nxt = node->nxt;
      node->nxt->prev = node->prev;
    }
  else
    {
      node->prev->nxt = NULL;
    }

  llst__free_node (node);

  return prev_node;
}

LLST *
llst_get_prev (LLST *node)
{
  return node->prev;
}

LLST *
llst_get_next (LLST *node)
{
  return node->nxt;
}

int
llst_get_val (LLST *node)
{
  return node->v;
}

LLST *
llst_set_val (LLST *node, int val)
{
  node->v = val;
  return node;
}

LLST *
llst_get_top (LLST *node)
{
  return node->top;
}

LLST *
llst_get_tail (LLST *node)
{
  return node->top->tail;
}

void
llst_swap (LLST *src, LLST *dst)
{
  if (src == NULL && dst == NULL)
    {
      ERROR ("Forbidden operation. Attempt to swap with NULL node");
      exit (1);
    }
  if (src->top != dst->top)
    {
      ERROR ("Forbidden operation. Attempt to swap nodes from differen linked "
             "lists");
      exit (1);
    }

  if (src->top == src || src->top == dst)
    {
      ERROR ("Forbidden operation. Attempt to swap with top node");
      exit (1);
    }

  LLST *psrc, *csrc;
  int tmpv;
  int src_is_tail, dst_is_tail;

  src_is_tail = (src->top->tail == src) ? TRUE : FALSE;
  dst_is_tail = (dst->top->tail == dst) ? TRUE : FALSE;

  if (src_is_tail)
    src->top->tail = dst;

  if (dst_is_tail)
    src->top->tail = src;

  psrc = src->prev;
  csrc = src->nxt;
  tmpv = src->v;

  src->prev = dst->prev;
  src->nxt = dst->nxt;
  src->v = dst->v;

  dst->prev = psrc;
  dst->nxt = csrc;
  dst->v = tmpv;
}

void
llst_move_node_to_tail (LLST *node)
{
  if (node == NULL)
    return;

  if (node->top->tail == node)
    return;

  if (node->top == node)
    {
      ERROR ("Forbidden operation. Attempt to move top to tail");
      exit (1);
    }

  LLST *pnode, *cnode, *tmp_tail;

  tmp_tail = node->top->tail;
  assert (tmp_tail->nxt == NULL);

  pnode = node->prev;
  cnode = node->nxt;

  pnode->nxt = tmp_tail;
  cnode->prev = tmp_tail;

  tmp_tail->prev->nxt = node;
  node->prev = tmp_tail->prev;
  node->nxt = NULL;

  tmp_tail->prev = pnode;
  tmp_tail->nxt = cnode;
}

int
llst_get_len (LLST *top)
{
  int cnt;
  LLST *tmp;

  cnt = 0;
  top = top->nxt;
  while (top != NULL)
    {
      cnt += 1;
      tmp = top->nxt;
      top = tmp;
    }

  return cnt;
}

int *
llst_convert_to_arr (LLST *top)
{
  int len, idx, *arr;
  LLST *tmp;

  len = llst_get_len (top);
  arr = calloc (len, sizeof (int));
  top = top->nxt;

  idx = 0;
  while (top != NULL)
    {
      arr[idx] = top->v;
      idx++;
      tmp = top->nxt;
      top = tmp;
    }

  return arr;
}

void
llst_print (LLST *top)
{
  LLST *tmp;
  while (top != NULL)
    {
      tmp = top->nxt;
      printf ("%d ", top->v);
      top = tmp;
    }
  printf ("\n");
}
