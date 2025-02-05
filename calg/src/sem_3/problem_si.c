#include <stdio.h>

int moveright(int *arr, int key, int last) {
  int idx = last - 1;
  for (; idx >= 0;) {
    if (arr[idx] > key) {
      arr[idx + 1] = arr[idx];
      idx--;
    } else {
      break;
    }
  }
  idx += 1;

  return idx;
}

void inssort(int *arr, int len) {
  int i;
  for (i = 0; i < len; ++i) {
    int key, pos;
    key = arr[i];
    pos = moveright(arr, key, i);
    arr[pos] = key;
  }
}

int main() {
  int arr[6] = {1, 2, 4, 5, 3, 2};
  inssort(arr, 6);
  for (int i = 0; i < 6; i++)
    printf("%d ", arr[i]);
  printf("\n");

  int arr2[6] = {5, 2, 4, 1, 3, 2};
  inssort(arr2, 6);
  for (int i = 0; i < 6; i++)
    printf("%d ", arr[i]);
  printf("\n");
}
