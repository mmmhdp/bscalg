#include <stdio.h>
#include <stdlib.h>

unsigned linear_min_search(int const *parr, unsigned len, unsigned bgn_idx) {
  int min_idx = bgn_idx;
  int min = parr[bgn_idx];
  for (unsigned i = bgn_idx + 1; i < len; i++) {
    if (parr[i] < min) {
      min = parr[i];
      min_idx = i;
    }
  }
  return min_idx;
}

void swap(int *v1, int *v2) {
  int tmp = *v1;
  *v1 = *v2;
  *v2 = tmp;
}

void selsort_one_iter(int *arr, unsigned len, unsigned target_idx) {
  unsigned curr_min_idx = linear_min_search(arr, len, target_idx);
  swap(&(arr[curr_min_idx]), &(arr[target_idx]));
}

int main() {
  int nitems, arr_size, target_idx;
  int *arr;

  nitems = scanf("%d", &arr_size);
  if (nitems != 1)
    abort();

  arr = (int *)malloc(sizeof(int) * arr_size);
  for (int i = 0; i < arr_size; i++) {
    nitems = scanf("%d", &(arr[i]));
    if (nitems != 1)
      abort();
  }

  nitems = scanf("%d", &target_idx);
  if (nitems != 1)
    abort();

  selsort_one_iter(arr, arr_size, target_idx);

  for (int i = 0; i < arr_size; i++) {
    printf("%d ", arr[i]);
  }
  printf("\n");

  free(arr);
}
