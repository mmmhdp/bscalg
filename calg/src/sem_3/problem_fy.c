#include <stdio.h>
#include <stdlib.h>

int myrand() {
  static unsigned long int seed = 1;
  seed = seed * 1103515245 + 12345;
  return (unsigned int)(seed / 65536) % 32768;
}

void swap(int src_idx, int dst_idx, int *arr) {
  int tmp = arr[dst_idx];
  arr[dst_idx] = arr[src_idx];
  arr[src_idx] = tmp;
}

int *fisher_yates_shuffle(int arr[], int arr_size) {
  int last_idx = arr_size - 1, swp_idx;
  for (int i = 0; i < arr_size - 1; i++) {
    swp_idx = myrand() % (last_idx + 1);
    swap(swp_idx, last_idx, arr);
    last_idx -= 1;
  }
  return arr;
}

int main() {
  int nitems, arr_size;
  int *arr;

  nitems = scanf("%d", &arr_size);
  if (nitems != 1)
    abort();

  if (arr_size == 0) {
    printf("0");
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

  fisher_yates_shuffle(arr, arr_size);

  for (int i = 0; i < arr_size; i++) {
    printf("%d ", arr[i]);
  }
  return 0;
}
