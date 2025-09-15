#include "llst.h"
#include "utils.h"
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct llst
{
  struct llst *top;
  struct llst *nxt;
  struct llst *prev;
  int v;
} LLST;

LLST *
llst_init (void)
{
  LLST *head = calloc (1, sizeof (LLST));
  head->top = head;
  return head;
}

LLST *
llst_add_node (LLST *head, int value)
{
  LLST *tmp = malloc (sizeof (LLST));
  tmp->v = value;
  tmp->prev = head;
  tmp->top = head->top;

  head->nxt = tmp;
  head = head->nxt;

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
llst_swap (LLST *src, LLST *dst)
{
  if (src == NULL && dst == NULL)
  {
      ERROR ("Forbidden operation. Attempt to swap with NULL node");
      exit (1);
  }
  if (src->top != dst->top)
    {
      ERROR ("Forbidden operation. Attempt to swap nodes from differen linked lists");
      exit (1);
    }

  if (src->top == src || src->top == dst)
  {
      ERROR ("Forbidden operation. Attempt to swap with top node");
      exit (1);
  }

  LLST *psrc, *csrc;
  int tmpv;

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
