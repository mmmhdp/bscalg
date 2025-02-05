#include <stdio.h>
#include <stdlib.h>

int main() {
  int nitems, arr_size, min, max, tmp;
  int *arr;

  nitems = scanf("%d", &arr_size);
  if (nitems != 1)
    abort();

  if (arr_size == 0) {
    printf("0 0");
    return 0;
  }

  arr = (int *)malloc(sizeof(int) * arr_size);
  for (int i = 0; i < arr_size; i++) {
    nitems = scanf("%d", &(arr[i]));
    if (nitems != 1)
      abort();
  }

  if (arr_size == 1) {
    printf("%d", arr[0]);
    return 0;
  }

  max = arr[0] > arr[1] ? arr[0] : arr[1];
  min = arr[0] < arr[1] ? arr[0] : arr[1];

  for (int i = 2; i < arr_size; i++) {
    tmp = arr[i];
    if (tmp > max) {
      max = tmp;
    } else if (tmp < min) {
      min = tmp;
    }
  }

  printf("%d %d", min, max);
  return 0;
}
