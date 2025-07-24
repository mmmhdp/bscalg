#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#define DBG

typedef enum
{
  RED,
  BLACK
} Color;

typedef struct q
{
  int lb;
  int rb;
} Query;

typedef struct tree_t
{
  struct tree_t *parent;
  struct tree_t *left;
  struct tree_t *right;
  int data;
  Color color;
} Bt;

void
bt_free (Bt *top)
{
  if (!top)
    return;

  bt_free (top->left);
  bt_free (top->right);

  free (top);
}

void
bt_left_rotation (Bt **root, Bt **target)
{
  Bt *node, *right_child;

  node = *target;
  right_child = node->right;

  if (!node->parent)
    {
      *root = right_child;
    }
  else if (node->parent->left == node)
    {
      node->parent->left = right_child;
    }
  else
    {
      node->parent->right = right_child;
    }

  right_child->parent = node->parent;

  node->right = right_child->left;
  if (right_child->left)
    right_child->left->parent = node;

  right_child->left = node;
  node->parent = right_child;
}

void
bt_right_rotation (Bt **root, Bt **target)
{
  Bt *node = *target, *left_child;

  node = *target;
  left_child = node->left;

  if (!node->parent)
    {
      *root = left_child;
    }
  else if (node->parent->left == node)
    {
      node->parent->left = left_child;
    }
  else
    {
      node->parent->right = left_child;
    }

  left_child->parent = node->parent;

  node->left = left_child->right;
  if (left_child->right)
    left_child->right->parent = node;

  left_child->right = node;
  node->parent = left_child;
}

void
bt_balance (Bt **root, Bt **target)
{
  Bt *x = *target;
  Bt *unc = NULL;

  while (x != *root && x->parent->color == RED)
    {
      if (!x->parent || !x->parent->parent)
        break;

      if (x->parent->parent->left == x->parent)
        {
          unc = x->parent->parent->right;

          if (unc && unc->color == RED)
            {
              x->parent->color = BLACK;
              unc->color = BLACK;
              x->parent->parent->color = RED;
              x = x->parent->parent;
            }
          else
            {
              if (x == x->parent->right)
                {
                  x = x->parent;
                  bt_left_rotation (root, &x);
                }
              x->parent->color = BLACK;
              x->parent->parent->color = RED;
              bt_right_rotation (root, &(x->parent->parent));
            }
        }
      else
        {
          unc = x->parent->parent->left;

          if (unc && unc->color == RED)
            {
              x->parent->color = BLACK;
              unc->color = BLACK;
              x->parent->parent->color = RED;
              x = x->parent->parent;
            }
          else
            {
              if (x == x->parent->left)
                {
                  x = x->parent;
                  bt_right_rotation (root, &x);
                }
              x->parent->color = BLACK;
              x->parent->parent->color = RED;
              bt_left_rotation (root, &(x->parent->parent));
            }
        }
    }

  (*root)->color = BLACK;
}

Bt **
bt_add_node_imp (Bt **root, Bt **tree_top, int val)
{
  Bt *top = *tree_top;
  Bt *curr = NULL;

  if (!top)
    {
      top = calloc (1, sizeof (Bt));
      top->data = val;
      *root = top;
      return root;
    }

  if (val <= top->data)
    {
      if (!top->left)
        {
          curr = calloc (1, sizeof (Bt));
          curr->data = val;
          curr->parent = top;
          top->left = curr;

          return &(top->left);
        }

      return bt_add_node_imp (root, &top->left, val);
    }
  else
    {
      if (!top->right)
        {
          curr = calloc (1, sizeof (Bt));
          curr->data = val;
          curr->parent = top;
          top->right = curr;

          return &(top->right);
        }

      return bt_add_node_imp (root, &top->right, val);
    }
}

void
bt_add_node (Bt **root, int val)
{
  Bt **curr = bt_add_node_imp (root, root, val);
#ifndef DBG
  printf ("\nCURRENT INSERTED NODE IS %d\n", (*curr)->data);
#endif
  bt_balance (root, curr);
}

int
q_answer (Query *q, Bt *top)
{
  int ntop, nleft, nright;

  ntop = nleft = nright = 0;

  if (!top)
    return 0;

#ifndef DBG
  printf ("Node with value %d and color %d is reached\n", top->data,
          top->color);
  if (top->parent)
    printf ("Nodes parent value is %d and color %d\n", top->parent->data,
            top->parent->color);
  else
    printf ("Parent is not presented. Should be root node now\n");
#endif

  if (q->lb <= top->data && top->data <= q->rb)
    {
      ntop += 1;
      nright = q_answer (q, top->right);
      nleft = q_answer (q, top->left);
    }

  if (top->data < q->lb)
    nright = q_answer (q, top->right);

  if (top->data > q->rb)
    nleft = q_answer (q, top->left);

  ntop += nleft + nright;

  return ntop;
}

void
bt_print_pre (struct tree_t *top)
{
  if (!top)
    return;

  printf ("%d ", top->data);

  bt_print_pre (top->left);
  bt_print_pre (top->right);
}

int
main (void)
{
  int nitems, i, tmp, left, right, nels, nqueries;
  Query *qlist;

  Bt *top = NULL;

  scanf ("%d", &nels);

#ifndef DBG
  printf ("Tree will have %d elements\n", nels);
#endif

  for (i = 0; i < nels; i++)
    {
      nitems = scanf ("%d", &tmp);
      assert (nitems == 1);

#ifndef DBG
      printf ("Curr added el %d\n", tmp);
      printf ("Tree before insertion:\n");
      bt_print_pre (top);
      printf ("\n");

#endif

      bt_add_node (&top, tmp);

#ifndef DBG
      printf ("Tree after insertion:\n");
      bt_print_pre (top);
      printf ("\n");
#endif
    }

  scanf ("%d", &nqueries);

  qlist = calloc (nqueries, sizeof (Query));

  for (i = 0; i < nqueries; i++)
    {
      nitems = scanf ("%d", &left);
      assert (nitems == 1);

      nitems = scanf ("%d", &right);
      assert (nitems == 1);

      qlist[i].lb = left;
      qlist[i].rb = right;
    }

#ifndef DBG
  printf ("Bt preopder:\n");
  bt_print_pre (top);
  printf ("\n");
#endif

  for (i = 0; i < nqueries; i++)
    {
      tmp = q_answer (&qlist[i], top);
#ifndef DBG
      printf ("Query from %d to %d = ", qlist[i].lb, qlist[i].rb);
      printf ("%d \n", tmp);
#else
      printf ("%d ", tmp);
#endif
    }

  free (qlist);
  bt_free (top);
  return 0;
}
