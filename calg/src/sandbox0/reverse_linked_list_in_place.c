#include <stdio.h>
#include <stdlib.h>

typedef struct Node
{
  int val;
  struct Node *next;
} Node;

void
ll_free (Node *root)
{
  Node *tmp;
  while (root != NULL)
    {
      tmp = root->next;
      free (root);
      root = tmp;
    }
}

void
ll_show (Node const *root)
{
  while (root->next != NULL)
    {
      printf ("%d ", root->val);
      root = root->next;
    }
  printf ("\n");
}

void
ll_print_reverse (Node *root)
{
  if (root->next->next == NULL && root->next->val == 0)
    {
      printf ("%d ", root->val);
      return;
    }

  ll_print_reverse (root->next);

  printf ("%d ", root->val);
}

Node **
ll_reverse (Node **root)
{
  Node **prev;
  Node *curr, *new_root, *beforehand;

  curr = *root;

  new_root = curr->next;

  if (new_root->next == NULL && new_root->val == 0)
    {
      new_root = curr->next;

      new_root->val = curr->val;
      new_root->next = curr;

      curr->val = 0;
      curr->next = NULL;

      return root;
    }

  prev = ll_reverse (&curr->next);
  beforehand = *prev;

  beforehand->val = curr->val;
  beforehand->next = curr;

  curr->val = 0;
  curr->next = NULL;

  return root;
}

int
main (void)
{
  int n;
  Node *root, *tail;

  scanf ("%d", &n);

  root = calloc (1, sizeof (Node));
  tail = root;

  for (int i = 0; i < n; i++)
    {
      scanf ("%d", &tail->val);
      tail->next = calloc (1, sizeof (Node));
      tail = tail->next;
    }

  printf ("Before:\n");
  ll_show (root);

  ll_reverse (&root);
  root = tail;

  printf ("After:\n");
  ll_show (root);

  ll_free (root);
}
