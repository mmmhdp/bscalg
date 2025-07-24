#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

void
print_postorder (int ipre_root, int in_start_idx, int in_end_idx, int len,
                 int *pre, int *in)
{
  int i, in_isplit, root;

  if (ipre_root >= len || in_start_idx > in_end_idx)
    return;

  root = pre[ipre_root];

  in_isplit = -1;
  for (i = in_start_idx; i <= in_end_idx; i++)
    if (in[i] == root)
      in_isplit = i;

  print_postorder (ipre_root + 1, in_start_idx, in_isplit - 1, len, pre, in);
  print_postorder (ipre_root + (in_isplit - in_start_idx) + 1, in_isplit + 1,
                   in_end_idx, len, pre, in);

  printf ("%d ", root);
}

void
print_traversal (int *arr, int len)
{
  int i;
  for (i = 0; i < len; i++)
    printf ("%d ", arr[i]);
  printf ("\n");
}

int
main (void)
{
  int i, nitems, n;

  nitems = scanf ("%d", &n);
  assert (nitems == 1);

  int *pre = malloc (sizeof (int) * n);
  int *in = malloc (sizeof (int) * n);

  for (i = 0; i < n; i++)
    {
      nitems = scanf ("%d", &(pre[i]));
      assert (nitems == 1);
    }

  for (i = 0; i < n; i++)
    {
      nitems = scanf ("%d", &(in[i]));
      assert (nitems == 1);
    }

#if 0
  printf("Preorder:\n");
  print_traversal(pre, n);

  printf("Inorder:\n");
  print_traversal(in, n);

  printf("Postorder:\n");
#endif
  print_postorder (0, 0, n - 1, n, pre, in);
  printf ("\n");

  free (in);
  free (pre);
}
