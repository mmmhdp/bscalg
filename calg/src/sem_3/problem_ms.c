#include <stdio.h>
#include <stdlib.h>

/*merge two sorted parts of arr in merge sort alg*/
void merge(int arr[], int l, int m, int r) {
  int lhs_idx = l, rhs_idx = m + 1, idx = 0;
  int *buffer = malloc(sizeof(int) * (r - l + 1));

  while ((idx < r - l + 1) && (lhs_idx < m + 1) && (rhs_idx < r + 1)) {
    if (arr[lhs_idx] <= arr[rhs_idx]) {
      buffer[idx++] = arr[lhs_idx++];
    } else {
      buffer[idx++] = arr[rhs_idx++];
    }
  }

  if (lhs_idx == m + 1 && rhs_idx == r + 1) {
    ;
  } else if (rhs_idx < r + 1) {
    while (rhs_idx < r + 1)
      buffer[idx++] = arr[rhs_idx++];
  } else {
    while (lhs_idx < m + 1)
      buffer[idx++] = arr[lhs_idx++];
  }

  for (int i = 0, j = l; i < (r - l + 1) && j < r + 1;) {
    arr[j++] = buffer[i++];
  }

  free(buffer);
}

int main() {

  int arr[] = {50, 50, 60, 61, 1, 2, 3};
  int l = 0, r = 6, m = (l + r) / 2;

  merge(arr, l, m, r);

  for (int i = 0; i < r + 1; i++) {
    printf("%d ", arr[i]);
  }
  printf("\n");
}
