#include <stdio.h>
#include <stdlib.h>

#define BUFF_SIZE (int)1000000
/*
 Returns an element, with is represented in the array in
 more than 50 percent of cases

 !!!
 if all parr[i] >= 0 || len <= 1000000
*/
int majority_element(const int parr[], int len) {
  int mj_idx = 0, mj_cnt = 1, mj = parr[0], tmp_el;
  int *pcnt_buff = (int *)calloc(BUFF_SIZE, sizeof(int));

  for (int i = 0; i < len; i++) {
    tmp_el = parr[i];
    if (tmp_el > BUFF_SIZE)
      abort();
    pcnt_buff[tmp_el] += 1;
  }

  for (int i = 1; i < len; i++) {
    tmp_el = parr[i];
    if (pcnt_buff[tmp_el] > mj_cnt) {
      mj_idx = i;
      mj_cnt = pcnt_buff[tmp_el];
    }
  }

  free(pcnt_buff);

  if (mj_cnt <= len / 2)
    return -1;

  mj = parr[mj_idx];

  return mj;
}

/* recursive version of majority_element
 * */
int majority_element_rec(const int parr[], int len) {
  int r_mj, l_mj, r_mj_cnt, l_mj_cnt;

  if (len == 1)
    return parr[0];

  l_mj = majority_element_rec(parr, len / 2);
  r_mj = majority_element_rec(parr + len / 2, len - (len / 2));

  r_mj_cnt = 0;
  l_mj_cnt = 0;
  for (int i = 0; i < len; i++) {
    if (parr[i] == r_mj)
      r_mj_cnt++;
    if (parr[i] == l_mj)
      l_mj_cnt++;
  }

  if (r_mj_cnt <= len / 2 && l_mj_cnt <= len / 2)
    return -1;

  return (r_mj_cnt > l_mj_cnt) ? r_mj : l_mj;
}

int main() {
  int nitems, arr_size, tmp;
  nitems = scanf("%d", &arr_size);
  if (nitems != 1)
    abort();
  int *arr = malloc(arr_size * sizeof(int));

  for (int i = 0; i < arr_size; i++) {
    nitems = scanf("%d", &tmp);
    if (nitems != 1)
      abort();
    arr[i] = tmp;
  }

  int mj_arr_el = majority_element_rec(arr, arr_size);
#if 0 
  for (int i = 0; i < arr_size; i++)
    printf("%d ", arr[i]);
  printf("\n");
  printf("WITH MAJORITY ELEM %d\n", mj_arr_el);
#endif

  printf("%d\n", mj_arr_el);

  free(arr);
}
