#include <stdio.h>
#include <stdlib.h>

// #define RES

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
  int nitems, cnt, tmp;
  struct node_t *top, *even_tail, *even_tail_prev, *odd_top, *odd_tail_prev,
      *odd_tail;

  if (inp == NULL)
    return NULL;

  top = even_tail = even_tail_prev = NULL;
  odd_top = odd_tail = odd_tail_prev = NULL;

  even_tail = calloc (1, sizeof (struct node_t));
  top = even_tail;
  even_tail_prev = NULL;

  odd_tail = calloc (1, sizeof (struct node_t));
  odd_top = odd_tail;
  odd_tail_prev = NULL;

  for (cnt = 0; (nitems = fscanf (inp, "%d", &tmp)) == 1; cnt++)
    {
      if (tmp & 1)
        {
          odd_tail->data = tmp;
          odd_tail->next = calloc (1, sizeof (struct node_t));
          odd_tail_prev = odd_tail;
          odd_tail = odd_tail->next;
        }
      else
        {
          even_tail->data = tmp;
          even_tail->next = calloc (1, sizeof (struct node_t));
          even_tail_prev = even_tail;
          even_tail = even_tail->next;
        }
    }

  if (cnt == 0)
    {
      free (even_tail);
      free (odd_tail);

      top = even_tail = even_tail_prev = NULL;
      odd_top = odd_tail = odd_tail_prev = NULL;

      return NULL;
    }

  if (top->next == NULL)
    {
      free (even_tail);
      top = even_tail = even_tail_prev = NULL;

      odd_tail_prev->next = NULL;
      free (odd_tail);
      odd_tail = NULL;

      return odd_top;
    }

  if (odd_top->next == NULL)
    {
      free (odd_tail);
      odd_top = odd_tail = odd_tail_prev = NULL;

      even_tail_prev->next = NULL;

      free (even_tail);
      even_tail = NULL;

      return top;
    }

  // Merge even and odd part
  even_tail->data = odd_top->data;
  even_tail->next = odd_top->next;

  // Clean suffix part
  odd_tail_prev->next = NULL;

  if (odd_top != odd_tail)
    {
      free (odd_top);
      odd_top = NULL;
    }
  free (odd_tail);
  odd_tail = NULL;

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
  if (!fd)
    abort ();

  struct node_t *top = read_list (fd);

  show_list (top);

  delete_list (top);
  fclose (fd);
}
#endif
