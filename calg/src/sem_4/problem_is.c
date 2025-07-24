#include <stdlib.h>

struct tree_t
{
  struct tree_t *left;
  struct tree_t *right;
  int data;
};

struct inorder_list_t
{
  int *box;
  int len;
};

void
get_inorder (struct tree_t *top, struct inorder_list_t *inoder)
{

  if (!top)
    return;

  get_inorder (top->left, inoder);

  inoder->box = realloc (inoder->box, sizeof (int) * (inoder->len + 1));

  inoder->box[inoder->len] = top->data;
  inoder->len++;

  get_inorder (top->right, inoder);
}

int
check_is_bst (struct tree_t *top)
{

  int i;
  struct inorder_list_t *inorder = calloc (1, sizeof (struct inorder_list_t));
  get_inorder (top, inorder);

  for (i = 1; i < inorder->len; i++)
    {
      if (inorder->box[i - 1] > inorder->box[i])
        {
          free (inorder->box);
          free (inorder);
          return 0;
        }
    }

  free (inorder->box);
  free (inorder);
  return 1;
}

int
main (void)
{
}
