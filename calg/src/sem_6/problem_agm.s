// --- int foo(int *x, int y, int z) ---
// edi = x
// esi = y
// edx = z
// return value = eax
  .globl  foo
  .type foo, @function
foo:
  xor r8d, r8d
  test  edx, edx
  je  .L1
  mov eax, edx
  sub eax, 1
  js  .L7
  cdqe
  jmp .L4
.L5:
  mov DWORD PTR 4[rdi+rax*4], edx
  sub rax, 1
  test  eax, eax
  js  .L1
.L4:
  mov edx, DWORD PTR [rdi+rax*4]
  mov r8d, eax
  cmp edx, esi
  jg  .L5
  add r8d, 1
.L1:
  mov eax, r8d
  ret
.L7:
  mov r8d, edx
  jmp .L1
