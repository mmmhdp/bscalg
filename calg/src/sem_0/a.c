#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

int iabs(int x) { return (x < 0) ? -x : x; }

int eu_mod(int x, int y) {
  int r;
  assert(y != 0);
  r = x % y;
  if (r < 0)
    r += iabs(y);
  return r;
}

int gcd(int x, int y) {
  int q;
  q = eu_mod(x, y);

  if (q == 0)
    return iabs(y);

  for (;;) {
    q = eu_mod(x, y);
    if (q == 0)
      break;
    x = y;
    y = q;
  }
  return y;
}

int get_copr_seg(int l, int r) {
  int lhs = l, rhs = l, copr_cnt = 0;
  for (; lhs <= r; lhs++) {
    rhs = lhs;
    while (rhs <= r) {
      if (!(lhs & 1) && !(rhs & 1)) {
        rhs++;
        continue;
      }
      if (gcd(lhs, rhs) == 1) {
        copr_cnt++;
        rhs++;
        break;
      } else {
        rhs++;
      }
    }
  }
  return copr_cnt;
}

int get_copr_seg_fast(int l, int r) {
  int cnt = 0;
  if (l == r)
    return (l == 1) ? 1 : 0;

  if (l == 1) {
    cnt++;
    l++;
  }
  cnt += r - l;

  return cnt;
}

int main() {
#if 1
  int nitems, ntsts, l, r, ncopr_seg;
  nitems = scanf("%d", &ntsts);
  if (nitems != 1)
    abort();
  for (int tst_idx = 0; tst_idx < ntsts; tst_idx++) {
    nitems = scanf("%d %d", &l, &r);
    if (nitems != 2)
      abort();
    ncopr_seg = get_copr_seg_fast(l, r);
    printf("%d\n", ncopr_seg);
  }
#endif

#if 0
  int a,b;
  scanf("%d %d", &a, &b);
  printf("%d", gcd(a,b));
#endif
}
