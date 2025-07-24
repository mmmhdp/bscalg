#include <stdio.h>
#include <stdlib.h>

struct tree_t
{
  struct tree_t *left;
  struct tree_t *right;
  int data;
};

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
}
