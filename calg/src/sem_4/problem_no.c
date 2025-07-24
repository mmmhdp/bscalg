#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#define FILENAME "in.dat"

typedef struct tree_t
{
  struct tree_t *left;
  struct tree_t *right;
  int data;
} bt;

void
bt_free (bt *top)
{
  if (!top)
    return;

  bt_free (top->left);
  bt_free (top->right);

  free (top);
}

void
bt_add_node (bt **root, int val)
{
  bt *top = *root;
  bt *curr = NULL;

  if (!top)
    {
      top = calloc (1, sizeof (bt));
      top->data = val;
      *root = top;
      return;
    }

  if (val <= top->data)
    {
      if (!top->left)
        {
          curr = calloc (1, sizeof (bt));
          curr->data = val;
          top->left = curr;

          return;
        }

      bt_add_node (&top->left, val);
    }
  else
    {
      if (!top->right)
        {
          curr = calloc (1, sizeof (bt));
          curr->data = val;
          top->right = curr;

          return;
        }
      bt_add_node (&top->right, val);
    }
}

struct stack_node
{
  struct stack_node *p;
  struct tree_t *t;
};

void
push (struct stack_node **s, struct tree_t *t)
{
  struct stack_node *n = malloc (sizeof (struct stack_node));
  n->p = *s;
  n->t = t;

  *s = n;
}

struct tree_t *
pop (struct stack_node **s)
{
  struct stack_node *node = (*s);
  if (!node)
    return NULL;

  struct tree_t *t = node->t;
  *s = node->p;

  free (node);

  return t;
}

void
print_pre (struct tree_t *top)
{
  struct stack_node *s = NULL;
  struct tree_t *t = NULL;

  push (&s, top);

  while (s)
    {
      t = pop (&s);
      if (!t)
        continue;

      printf ("%d ", t->data);
      push (&s, t->right);
      push (&s, t->left);
    }
}

int
main (void)
{
  int nitems, i, n, tmp;
  FILE *fd;

  fd = fopen (FILENAME, "r");
  if (!fd)
    return 1;

  nitems = fscanf (fd, "%d", &n);
  assert (nitems == 1);

  bt *top = NULL;

  for (i = 0; i < n; i++)
    {
      nitems = fscanf (fd, "%d", &tmp);
      assert (nitems == 1);

      bt_add_node (&top, tmp);
    }

  print_pre (top);
  bt_free (top);

  fclose (fd);
  return 0;
}
