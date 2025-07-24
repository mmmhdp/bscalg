#include <stdio.h>
#include <stdlib.h>

// #define RES
#define FALSE 0
#define TRUE 1

#ifndef RES
struct node_t
{
  struct node_t *next;
  int data;
};
#endif

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

int
get_loop_len (struct node_t *t, struct node_t *r)
{
  int cnt;

  cnt = 0;
  for (;;)
    {
      r = r->next;
      if (r != t)
        cnt++;
      else
        break;
    }

  return ++cnt;
}

int
list_is_a_loop (struct node_t *top)
{
  int res, loopsz;
  struct node_t *t, *r;

  res = FALSE;
  loopsz = 0;
  t = r = top;

  // No elements or only one can't assemble a loop
  if (top == NULL || top->next == NULL)
    return res;

  while (r != NULL)
    {
      t = t->next;
      r = r->next;

      if (r != NULL)
        r = r->next;
      else
        {
          res = loopsz;
          break;
        }

      if (t == r)
        {
          loopsz = get_loop_len (t, r);
          return loopsz;
        }
    }

  return loopsz;
}

#ifndef RES
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

  show_list (top);

  printf ("\n");
  int is_a_loop = list_is_a_loop (top);
  if (is_a_loop)
    printf ("List has a loop!\n");
  else
    printf ("List is staight!\n");

  delete_list (top);
}
#endif
