#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#define VAL_BND 1000

int main(void) {
  int i, nitems, size, tmp, max_val;
  int *arr;
  int *cnt_vals = calloc(VAL_BND, sizeof(int));

  nitems = scanf("%d", &size);
  if (nitems != 1)
    abort();

  arr = malloc(size * sizeof(int));

  nitems = scanf("%d", &tmp);
  if (nitems != 1)
    abort();

  i = 0;
  arr[i] = tmp;
  cnt_vals[tmp] += 1;
  max_val = arr[i];
  i++;

  while (i < size) {
    nitems = scanf("%d", &tmp);
    if (nitems != 1)
      abort();

    arr[i] = tmp;
    cnt_vals[tmp] += 1;

    if (tmp > max_val)
      max_val = tmp;

    i++;
  }

  for (i = 0; i < max_val + 1; i++)
    printf("%d ", cnt_vals[i]);
  printf("\n");

  free(arr);
  free(cnt_vals);
  return 0;
}
