#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

void iswap(int *source, int *dist) {
  int tmp = *source;
  *source = *dist;
  *dist = tmp;
}

int *get_frac_repr(int *a, int *b, int *depth, int *frac_repr) {
  int quotient, mod, numer, denom;
  int *memory_check_pointer;

  numer = *a;
  denom = *b;
  assert(denom != 0);

  for (int i = 0;; i++) {
    quotient = numer / denom;
    mod = numer % denom;

    memory_check_pointer = realloc(frac_repr, (i + 1) * sizeof(int));
    if (!memory_check_pointer) {
      printf("Memory allocation failed");
      exit(1);
    } else {
      frac_repr = memory_check_pointer;
    }

    frac_repr[i] = quotient;

    if (mod == 0) {
      *depth = i + 1;
      break;
    } else {
      numer = mod;
      iswap(&numer, &denom);
    }
  }
  return frac_repr;
}
int main() {

  int a, b;
  int *depth = (int *)calloc(1, sizeof(int));
  int *frac_repr = (int *)malloc(sizeof(int));
  int *res;

  int nitems = scanf("%d%d", &a, &b);

  if (nitems != 2 || b == 0) {
    printf("Invalid args. Expected any int a and int b != 0.");
    return 0;
  }

  res = get_frac_repr(&a, &b, depth, frac_repr);

  for (int i = 0; i < *depth; i++) {
    printf("%d ", res[i]);
  }
  printf("\n");

  free(depth);
  free(res);

  return 0;
}
