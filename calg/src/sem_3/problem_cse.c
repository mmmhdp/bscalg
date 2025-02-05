#include <assert.h>
#include <limits.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef int (*cmp_t)(void const *lhs, void const *rhs);

void swap(void *v1, void *v2, int eltsize) {
  void *tmp = malloc(eltsize);

  memcpy(tmp, v1, eltsize);
  memcpy(v1, v2, eltsize);
  memcpy(v2, tmp, eltsize);

  free(tmp);
}

int selstep(void *parr, int eltsize, int numelts, int nsorted, cmp_t cmp) {
  char *arr = (char *)parr;
  unsigned min_idx = nsorted;
  char *min = malloc(eltsize);

  memcpy(min, arr + nsorted * eltsize, eltsize);

  for (int i = nsorted + 1; i < numelts; i++) {
    if (cmp(arr + i * eltsize, min)) {
      memcpy(min, arr + i * eltsize, eltsize);
      min_idx = i;
    }
  }

  swap(arr + nsorted * eltsize, arr + min_idx * eltsize, eltsize);

  free(min);
  return 0;
}

struct triple {
  int x, y, z;
};

int tcmp(void const *key, void const *elt) {
  struct triple const *lhs = (struct triple const *)key;
  struct triple const *rhs = (struct triple const *)elt;
  if (lhs->x == rhs->x && lhs->y == rhs->y)
    return (lhs->z < rhs->z);
  if (lhs->x == rhs->x)
    return (lhs->y < rhs->y);
  return (lhs->x < rhs->x);
}

typedef int (*cmp_t)(const void *lhs, const void *rhs);

int selstep(void *parr, int eltsize, int n, int last, cmp_t cmp);

int main() {
  int i, res, n, last;
  struct triple *parr;

  res = scanf("%d", &n);
  assert(res == 1);
  assert(n > 2);

  parr = (struct triple *)calloc(n / 3, sizeof(struct triple));

  for (i = 0; i < n / 3; ++i) {
    res = scanf("%d %d %d", &parr[i].x, &parr[i].y, &parr[i].z);
    assert(res == 3);
  }
  res = scanf("%d", &last);
  assert(res == 1);
  assert(last < n);

  selstep(parr, sizeof(struct triple), n / 3, last / 3, &tcmp);

  for (i = 0; i < n / 3; ++i)
    printf("%d %d %d\n", parr[i].x, parr[i].y, parr[i].z);

  free(parr);
  return 0;
}

#line 10000
