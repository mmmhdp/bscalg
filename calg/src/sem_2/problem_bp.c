#include <stdio.h>
#include <stdlib.h>

int msbi(int val) {
  int idx;
  if (val == 0)
    return -1;

  idx = 0;
  while (val >>= 1) {
    idx += 1;
  }
  return idx;
}

int lsbi(int val) {
  int idx;
  if (val == 0)
    return -1;

  idx = 0;
  if ((val >> 0) & 1) {

  } else {
    idx += 1;
    while (!((val >>= 1) & 1)) {
      idx += 1;
    }
  }

  return idx;
}

void print_binary_int(int val) {
  char *rev_str = malloc(sizeof(char));
  int insert_idx = 0;
  int size = 1;
  do {
    rev_str[insert_idx] = val & 1;
    rev_str = realloc(rev_str, size + 1);
    insert_idx += 1;
    size += 1;
  } while (val >>= 1);

  for (int i = insert_idx - 1; i >= 0; i--) {
    printf("%d", rev_str[i]);
  }
  printf("\n");
}

int main() {

#if 1
  int N, nitems;
  nitems = scanf("%d", &N);

  if (nitems != 1) {
    printf("Invalid args");
    abort();
  }

#endif
#if 0
  for (int k = 0; k < N; k++) {
    printf("BINARY FORM FOR N = %d IS :\n", k);
    print_binary_int(k);
    printf("MSB INDEX OF %d IS %d\n", k, msbi(k));
    printf("LSB INDEX OF %d IS %d\n", k, lsbi(k));
  }
#endif
  if ((msbi(N) != -1) && (lsbi(N) != -1)) {
    printf("%d ", msbi(N));
    printf("%d", lsbi(N));
  } else {
    printf("NO");
  }

  return 0;
}
