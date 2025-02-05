// low_idx - idx of pivot el of arr and at the same is the first el of partion
// high_idx - last pos in partion
//[0,1,...,low_idx,...,high_idx,..., arr_len - 1]

void swap(int *src, unsigned l_idx, unsigned r_idx) {
  int tmp = src[l_idx];
  src[l_idx] = src[r_idx];
  src[r_idx] = tmp;
}

unsigned partition(int *arr, unsigned low_idx, unsigned high_idx) {
  unsigned pivot_idx = low_idx;
  int pivot = arr[low_idx], l = low_idx + 1, r = high_idx;

  while (l <= r) {
    while (l <= r && arr[l] <= pivot)
      l++;
    while (l <= r && arr[r] > pivot)
      r--;

    if (l < r) {
      swap(arr, l, r);
      l++;
      r--;
    }
  }

  swap(arr, r, pivot_idx);
  return r;
}

int main() {}
