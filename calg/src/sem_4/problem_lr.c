#include <stdio.h>
#include <stdlib.h>

#if 1
#define DEBUG
#endif

#define FALSE 0
#define TRUE 1

#ifndef DEBUG
struct node_t
{
  struct node_t *next;
  int data;
};
#endif

struct node_t
{
  struct node_t *next;
  int data;
};

struct node_t *
reverse (struct node_t *top)
{
  struct node_t *l, *m, *r;

  if (top == NULL)
    return NULL;

  if (top->next == NULL)
    return top;

  l = NULL;
  m = top;
  r = top->next;

  while (r != NULL)
    {
      m->next = l;
      l = m;
      m = r;
      r = r->next;
    }

  m->next = l;
  l = m;
  m = r;

  return l;
}

#ifndef DEBUG

struct node_t *
recursive_reverse (struct node_t *top)
{
  struct node_t *xs;
  if (NULL == top)
    return NULL;
  if (NULL == top->next)
    return top;

  xs = reverse (top->next);

  top->next->next = top;
  top->next = NULL;
  return xs;
}

struct node_t *
read_list (FILE *inp)
{
  int nitems, tmp;
  struct node_t *top, *tail, *tail_prev;

  if (inp == NULL)
    return NULL;

  top = tail = tail_prev = NULL;

  tail = calloc (1, sizeof (struct node_t));
  top = tail;
  tail_prev = NULL;

  while ((nitems = fscanf (inp, "%d", &tmp)) == 1)
    {
      tail->data = tmp;
      tail->next = calloc (1, sizeof (struct node_t));
      tail_prev = tail;
      tail = tail->next;
    }

  tail_prev->next = NULL;
  free (tail);

  return top;
}

void
delete_list (struct node_t *top)
{
  struct node_t *tmp;

  while (top != NULL)
    {
      tmp = top->next;
      free (top);
      top = tmp;
    }
}

void
show_list (struct node_t const *const top)
{
  if (top == NULL)
    return;

  printf ("%d ", top->data);

  show_list (top->next);
}

int
main (void)
{
  FILE *fd = fopen ("in.dat", "r");
  struct node_t *top = read_list (fd);

  printf ("Before:\n");
  show_list (top);
  printf ("\n");

  // top = recursive_everse(top);

  // printf("After first reverse:\n");
  // show_list(top);
  // printf("\n");

  top = reverse (top);

  printf ("After second reverse:\n");
  show_list (top);
  printf ("\n");

  delete_list (top);
}
#endif

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct node_t *reverse (struct node_t *top);

void
delete_list (struct node_t *top)
{
  struct node_t *tmp;

  while (top != NULL)
    {
      tmp = top->next;
      free (top);
      top = tmp;
    }
}

int
main ()
{
  int i, ncorr, nnodes, res;
  struct node_t *top = NULL, *cur = NULL, *sub_top;

  res = scanf ("%d", &nnodes);
  assert (res == 1);
  assert (nnodes >= 0);

  if (nnodes > 0)
    {
      top = calloc (1, sizeof (struct node_t));
      top->data = 0;
      cur = top;
    }

  for (i = 1; i < nnodes; ++i)
    {
      cur->next = calloc (1, sizeof (struct node_t));
      cur->next->data = cur->data + 1;
      cur = cur->next;
    }

  ncorr = nnodes;
  top = reverse (top);
  sub_top = top;
  while (top && top->next)
    {
      if (top->data <= top->next->data)
        {
          ncorr -= 1;
          printf ("Wrong: %d\n", top->data);
        }
      top = top->next;
    }
  printf ("%d\n", ncorr);

  delete_list (sub_top);
}
