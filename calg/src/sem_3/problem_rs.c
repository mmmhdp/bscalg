#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

void get_ready(int *size, int *digit, int **arr) {
  int i = 0;

  if (scanf("%d", size) != 1)
    abort();

  *arr = malloc(*size * sizeof(int));

  while (i < *size)
    if (scanf("%d", &(*arr)[i++]) != 1)
      abort();

  if (scanf("%d", digit) != 1)
    abort();
}

void radix_sort(int *arr, int size, int ndigit) {
  int j, key;
  int *arrbuff = calloc(size, sizeof(int));

// full radix_sort implementation
#if 0
  int i;
  int expn = 1;
  for (i = 0; i < size; i++){
    int cnt[10] = {0};

    for(j = 0; j < size; j++)
      cnt[ arr[j]/expn % 10 ] += 1;

    for(j = 1; j < 10; j++)
      cnt[j] += cnt[j - 1];

    for(j = size - 1; j >= 0; j--){
      key = ((arr[j]/expn) % 10); 
      arrbuff[ cnt[key] - 1 ] = arr[j];
      --cnt [key];
    }

    for(j = 0; j < size; j++)
      arr[j] = arrbuff[j];
    
    expn *= 10;
  }
#else
  // radix sort by base equal to ndigit
  int expn = 1;
  for (j = 0; j < ndigit; j++)
    expn *= 10;

  int cnt[10] = {0};

  for (j = 0; j < size; j++)
    cnt[arr[j] / expn % 10] += 1;

  for (j = 1; j < 10; j++)
    cnt[j] += cnt[j - 1];

  for (j = size - 1; j >= 0; j--) {
    key = ((arr[j] / expn) % 10);
    arrbuff[cnt[key] - 1] = arr[j];
    --cnt[key];
  }

  for (j = 0; j < size; j++)
    arr[j] = arrbuff[j];

#endif

  free(arrbuff);
}

void print_arr(int *arr, int size) {
  for (int j = 0; j < size; j++)
    printf("%d ", arr[j]);
  printf("\n");
}

int main(void) {
  int size, ndigit;
  int *arr = NULL;

  get_ready(&size, &ndigit, &arr);
  radix_sort(arr, size, ndigit);
  print_arr(arr, size);

  free(arr);
}
