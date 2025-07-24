#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#if 0
#define DEBUG
#endif

struct node_t
{
  struct node_t *next;
  int data;
};

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
find_max (int arr[], int len)
{
  int i, max = arr[0];
  for (i = 1; i < len; ++i)
    {
      if (arr[i] > max)
        max = arr[i];
    }

  return max;
}

int
find_min (int arr[], int len)
{
  int i, min = arr[0];
  for (i = 1; i < len; ++i)
    {
      if (arr[i] < min)
        min = arr[i];
    }

  return min;
}

void
update_bucket (struct node_t **bks, int val, int idx)
{
  struct node_t *top, *prev, *tmp;
  prev = top = bks[idx];
  tmp = calloc (1, sizeof (struct node_t));
  tmp->data = val;

  if (top == NULL)
    {
      bks[idx] = tmp;
      return;
    }

  while (top != NULL)
    {
      if (tmp->data < top->data && top == prev)
        {
          bks[idx] = tmp;
          tmp->next = top;
          return;
        }

      if (tmp->data < top->data)
        {
          assert (top != prev);
          prev->next = tmp;
          tmp->next = top;
          return;
        }

      prev = top;
      top = top->next;
    }

  prev->next = tmp;
  return;
}

void
bucket_sort (int arr[], int len)
{
  int tmp, i, j, upper_bound, lower_bound, left, right, nbk, *bk_bounds,
      bk_step, bk_idx, *naive_bks;
  struct node_t **bks;

  upper_bound = find_max (arr, len);
  lower_bound = find_min (arr, len);
  lower_bound++;
  nbk = len;
  bk_step = upper_bound / nbk;

  bk_bounds = calloc (nbk * 2, sizeof (int));
  bks = calloc (nbk, sizeof (struct node_t *));
  naive_bks = calloc (nbk, sizeof (int));

  left = 0;
  right = bk_step;
  for (i = 0; i < nbk * 2; i += 2)
    {
      bk_bounds[i] = left;
      bk_bounds[i + 1] = right;
      left = right;
      right = left + bk_step;
    }

#ifdef DEBUG
  for (i = 0; i < nbk * 2; i += 2)
    {
      printf ("BK IDX: %d\n", i / 2);
      printf ("%d ", bk_bounds[i]);
      printf ("%d ", bk_bounds[i + 1]);
      printf ("\n");
    }
#endif

  for (i = 0; i < len; ++i)
    {
      tmp = arr[i], bk_idx = 0;
      left = bk_bounds[0];
      right = bk_bounds[1];
      j = 2;
      while (j < (nbk * 2))
        {
          if (left <= tmp && tmp < right)
            break;
          bk_idx++;
          left = bk_bounds[j];
          right = bk_bounds[j + 1];
          j += 2;
        }

#ifdef DEBUG
      printf ("\n");
      printf ("Bucket before:\n");
      show_list (bks[bk_idx]);

      printf ("\nEl %d, in bucket %d\n", tmp, bk_idx);
      printf ("left %d, right %d\n", bk_bounds[bk_idx * 2],
              bk_bounds[bk_idx * 2 + 1]);
#endif
      update_bucket (bks, tmp, bk_idx);

#ifdef DEBUG
      printf ("Bucket after insertion\n");
      show_list (bks[bk_idx]);
      printf ("\n");
#endif
    }

  for (i = 0; i < nbk; ++i)
    {
      show_list (bks[i]);
      printf ("0 ");
    }

  free (naive_bks);
  free (bk_bounds);
  for (i = 0; i < nbk; ++i)
    delete_list (bks[i]);

  free (bks);
}

int
main (void)
{
  int i, nitems, n, *arr;
  nitems = scanf ("%d", &n);
  assert (nitems == 1);
  arr = calloc (n, sizeof (int));
  assert (arr != NULL);

  for (i = 0; i < n; ++i)
    {
      nitems = scanf ("%d", arr + i);
      assert (nitems == 1);
    }

#ifdef DEBUG
  for (i = 0; i < n; ++i)
    printf ("%d ", arr[i]);
  printf ("\n");

  printf ("Max value is %d \n", find_max (arr, n));
#endif

  bucket_sort (arr, n);

  free (arr);
  return 0;
}
