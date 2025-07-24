#include <stdio.h>

struct tree_t
{
  struct tree_t *left;
  struct tree_t *right;
  int data;
};

// preorder => VLR
void
print_pre (struct tree_t *top)
{
  if (!top)
    return;

  printf ("%d ", top->data);

  print_pre (top->left);
  print_pre (top->right);
}

int
main (void)
{
}
