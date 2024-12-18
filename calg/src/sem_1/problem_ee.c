#include <assert.h>
#include <stdio.h>

#if 1
long long llabs(long long x) { return (x < 0) ? -x : x; }

long long eu_mod(long long x, long long y) {
  long long r;
  assert(y != 0);
  r = x % y;
  if (r < 0)
    r += llabs(y);
  return r;
}

long long gcd(long long a, long long b, long long *x, long long *y) {
  long long x_hat, y_hat, d;

  if (b == 0) {
    *x = 1;
    *y = 0;
    return a;
  }

  d = gcd(b, a % b, &x_hat, &y_hat);

  *x = y_hat;
  *y = x_hat - (a / b) * y_hat;

  return d;
}

int main() {
  long long x, y, a, b, d;
  scanf("%lld%lld", &a, &b);
  d = gcd(a, b, &x, &y);
  if (d < 0) {
    d = llabs(d);
    x = -x;
    y = -y;
  }
  printf("%lld %lld %lld\n", x, y, d);
}
#endif
