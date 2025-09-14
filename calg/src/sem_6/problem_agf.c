// --- int f(int x, int y) ---
// edi = x
// esi = y
// return value = eax
/*
  .globl  foo
  .type foo, @function
foo:
  mov eax, edi      // eax = x
  imul  eax, edi    // eax = x^2
  imul  eax, edi    // eax = x^3
  test  eax, eax    // eax & eax => if eax 0 -> SF set to 1 else eax
  js  .L4           // js - jump if SF => sign check of eax, proceed to .L4 if
                    // eax is negative
  ret               // else return eax

.L4:
  imul  esi, esi    // esi = y^2
  add edi, edi      // edi = 2 * x
  mov edx, 17       // edx = 17
  sub edx, edi      // edx = 17 - 2 * x
  sub edx, esi      // edx = 17 - 2 * x - y^2
  add eax, edx      // eax = x^3 + 17 - 2 * x - y^2
  ret
*/

#include <stdio.h>

int
foo (int x, int y)
{
  int tmp, res;
  tmp = x * x * x;

  if (tmp < 0)
    res = tmp + 17 - (2 * x) - (y * y);
  else
    res = tmp;

  return res;
}

#if 1
int
main (void)
{
  int x = 2, y = 3, res;
  res = foo (x, y);
  printf ("%d", res);
}
#endif
