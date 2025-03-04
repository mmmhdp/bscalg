#include <linux/limits.h>
#include <stdio.h>
#include <stdlib.h>

#if 1
#define LOG
#endif

typedef struct {
  int nrows;
  int ncols;
  int **table;
} RecordTable;

RecordTable *rt_init(int nrows, int ncols) {
  int **table, i;

  RecordTable *rt = malloc(sizeof(RecordTable));

  table = (int **)calloc(nrows, sizeof(int *));
  for (i = 0; i < nrows; i++) {
    table[i] = (int *)calloc(ncols, sizeof(int));
  }

  rt->nrows = nrows;
  rt->ncols = ncols;
  rt->table = table;

  return rt;
}

void rt_free(RecordTable *rt) {
  int i;
  for (i = 0; i < rt->nrows; i++) {
    free(rt->table[i]);
    rt->table[i] = NULL;
  }

  free(rt->table);
  rt->table = NULL;
  free(rt);
  rt = NULL;
}

void rt_show(const RecordTable *rt) {
  int i, j;
  for (i = 0; i < rt->nrows; i++) {
    for (j = 0; j < rt->ncols; j++) {
      printf("%d ", rt->table[i][j]);
    }
    printf("\n");
  }
}

void rt_sort_by_col_idx(RecordTable *rt, int search_idx) {}

int main(void) {
  int i, j, tmp, N, K, X, nitems;
  RecordTable *rt;
  /*
   * N - num of RECORS
   * K - num of COLUMNS
   * X - IDX of COLUMN to SORT BY
   * */
  nitems = scanf("%d%d%d", &N, &K, &X);
  if (nitems != 3)
    abort();

  rt = rt_init(N, K);

  for (i = 0; i < N; i++) {
    for (j = 0; j < K; j++) {
      nitems = scanf("%d", &tmp);
      if (nitems != 1)
        abort();
      rt->table[i][j] = tmp;
    }
  }

#ifdef LOG
  printf("before:");
  rt_show(rt);

  rt_sort_by_col_idx(rt, X);

  printf("after:");
  rt_show(rt);
#endif
  rt_free(rt);
}
