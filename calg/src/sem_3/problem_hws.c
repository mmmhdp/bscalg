#include <assert.h>
#include <linux/limits.h>
#include <stdio.h>
#include <stdlib.h>

#define LOG 0
#define NAIVE 0
#define MERGE_TO_INSERT_BOUND 16

#define NATURAL_RUN_SIZE 16
// TODO implement natural run detection and merge stategy

static int sort_col_index = 0;

typedef struct
{
  int nrows;
  int ncols;
  int **table;
} RecordTable;

RecordTable *
rt_init (int nrows, int ncols)
{
  int **table, i;

  RecordTable *rt = malloc (sizeof (RecordTable));

  table = (int **)calloc (nrows, sizeof (int *));
  for (i = 0; i < nrows; i++)
    {
      table[i] = (int *)calloc (ncols, sizeof (int));
    }

  rt->nrows = nrows;
  rt->ncols = ncols;
  rt->table = table;

  return rt;
}

void
rt_free (RecordTable *rt)
{
  int i;
  for (i = 0; i < rt->nrows; i++)
    {
      free (rt->table[i]);
      rt->table[i] = NULL;
    }

  free (rt->table);
  rt->table = NULL;
  free (rt);
  rt = NULL;
}

void
rt_show (const RecordTable *rt)
{
  int i, j;
  for (i = -1; i < rt->nrows; i++)
    {
      if (i == -1)
        printf ("   |");
      else
        printf ("%3d|", i);
    }
  printf ("\n");

  for (i = -1; i < rt->nrows; i++)
    {
      printf ("____");
    }
  printf ("\n");

  for (i = 0; i < rt->nrows; i++)
    {
      printf ("%3d|", i);
      for (j = 0; j < rt->ncols; j++)
        {
          printf ("%3d ", rt->table[i][j]);
        }
      printf ("\n");
    }
}

void
rt_show_result (const RecordTable *rt)
{
  int i;
  for (i = 0; i < rt->nrows; i++)
    printf ("%d ", rt->table[i][i % rt->ncols]);
  printf ("\n");
}

void
rt_swap_rows (RecordTable *rt, int il, int ir)
{
  int *tmp;

  tmp = rt->table[il];
  rt->table[il] = rt->table[ir];
  rt->table[ir] = tmp;
}

int
rt_cmp_rows_by_col (RecordTable *rt, int il, int ir, int ic)
{
  return rt->table[il][ic] - rt->table[ir][ic];
}

int
row_cmp (const void *lr, const void *hr)
{
  int diff;
  int *const *plr = (int *const *)lr;
  int *const *phr = (int *const *)hr;

  diff = (*plr)[sort_col_index] - (*phr)[sort_col_index];
  if (diff < 0)
    return -1;
  else if (diff == 0)
    return 0;
  else
    return 1;
}

int
rt_row_cmp (const void *lr, const void *hr)
{
  int diff;
  int const *plr = (int const *)lr;
  int const *phr = (int const *)hr;

  diff = plr[sort_col_index] - phr[sort_col_index];
  if (diff < 0)
    return -1;
  else if (diff == 0)
    return 0;
  else
    return 1;
}

void
rt_merge (RecordTable *rt, int l, int m, int r)
{
  int i, left, right, **bf, bfsz, *lp, *rp, diff;

  left = l;
  right = m;

  bfsz = r - l + 1;
  bf = calloc (bfsz, sizeof (int *));
  assert (bf != NULL);

  // Merge main loop
  right++;
  for (i = 0; left <= m && right <= r; i++)
    {
      lp = rt->table[left];
      rp = rt->table[right];

      diff = rt_row_cmp (lp, rp);
      if (diff <= 0)
        {
          bf[i] = lp;
          left++;
        }
      else
        {
          bf[i] = rp;
          right++;
        }
    }

  // Suffix

  while (right <= r)
    {
      assert (i < bfsz);

      rp = rt->table[right];
      bf[i++] = rp;
      right++;
    }

  while (left <= m)
    {
      assert (i < bfsz);

      lp = rt->table[left];
      bf[i++] = lp;
      left++;
    }

  // From buff to table

  for (i = 0; i < bfsz; i++)
    rt->table[i + l] = bf[i];

  free (bf);
}

int
rt_moveright (RecordTable *rt, int ikey, int left_bound)
{
  int i, *cmp, *key;

  key = rt->table[ikey];
  for (i = ikey - 1; i >= left_bound; i--)
    {
      cmp = rt->table[i];
      if (rt_row_cmp (key, cmp) >= 0)
        {
          break;
        }
      else
        {
          rt_swap_rows (rt, i + 1, i);
        }
    }

  i++;
  return i;
}

void
rt_insertion_sort_imp (RecordTable *rt, int l, int r)
{
  int i, *key, pos;
  for (i = l; i <= r; i++)
    {
      key = rt->table[i];
      pos = rt_moveright (rt, i, l);
      rt->table[pos] = key;
    }
}

void
rt_reverse (RecordTable *rt)
{
  int i;
  for (i = 0; i < rt->nrows / 2; i++)
    {
      rt_swap_rows (rt, i, rt->nrows - 1 - i);
    }
}

void
rt_insertion_sort (RecordTable *rt)
{
  rt_insertion_sort_imp (rt, 0, rt->nrows - 1);
}

void
rt_merge_sort_imp (RecordTable *rt, int l, int r)
{
  int m;

  if (l >= r)
    return;

#if 1
  if (r - l + 1 < MERGE_TO_INSERT_BOUND)
    {
      rt_insertion_sort_imp (rt, l, r);
      return;
    }
#endif

  m = (l + r) / 2;
  rt_merge_sort_imp (rt, l, m);
  rt_merge_sort_imp (rt, m + 1, r);
  rt_merge (rt, l, m, r);
}

void
rt_merge_sort_by_col (RecordTable *rt)
{
  rt_merge_sort_imp (rt, 0, rt->nrows - 1);
}

void
rt_sort_by_col_idx (RecordTable *rt, int search_idx)
{
  sort_col_index = search_idx;
#if NAIVE
  qsort (rt->table, rt->nrows, sizeof (int *), row_cmp);
#elif 1
  rt_insertion_sort (rt);
#else
  rt_merge_sort_by_col (rt);
#endif
}

int
main (void)
{
  int i, j, tmp, N, K, X, nitems;
  RecordTable *rt;
  /*
   * N - num of RECORS
   * K - num of COLUMNS
   * X - IDX of COLUMN to SORT BY
   * */
  nitems = scanf ("%d%d%d", &N, &K, &X);
  if (nitems != 3)
    abort ();

  rt = rt_init (N, K);

  for (i = 0; i < N; i++)
    {
      for (j = 0; j < K; j++)
        {
          nitems = scanf ("%d", &tmp);
          if (nitems != 1)
            abort ();
          rt->table[i][j] = tmp;
        }
    }

#if LOG
  printf ("Before:\n");
  rt_show (rt);

#if 1
  rt_sort_by_col_idx (rt, X);
#elif 0
  // rt_swap_rows(rt, 0, 1);
  printf ("\nCompare results are %d\n", rt_cmp_rows_by_col (rt, 0, 1, 2));
#else
#endif

  printf ("After:\n");
  rt_show (rt);

  printf ("Result:\n");
  rt_show_result (rt);
#else

  rt_sort_by_col_idx (rt, X);
  rt_show_result (rt);
#endif

  rt_free (rt);
}
