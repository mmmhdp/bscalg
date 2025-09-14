.intel_syntax noprefix
  .text
  .globl  initital_caller
  .type initital_caller, @function


// seems like callee is a function that 
// pow by 2 
// initital_caller is ipowm()

// int initital_caller (int base, int exp, int modulo)
// edi = base 
// esi = exp
// edx = modulo


initital_caller:
  push  r13
  push  r12
  push  rbp
  push  rbx

  sub rsp, 8
  mov r12d, 1
  test  esi, esi
  je  epilogue

  // jump is exp == 0
  // and return 1 

  mov eax, edi 
  //eax = base 
  mov ebx, esi
  //ebx = exp
  mov r13d, edx
  //r13d = modulo 

  mov edx, 0
  div r13d 
  // div eax % r13d = base % modulo
  // eax = eax / r13d
  // edx = base % modulo

  mov rbp, rdx
  // rbp = base % modulo

  mov r12d, 1
  jmp .L5

.L3:
  mov edi, ebp
  mov edx, r13d
  mov esi, ebp

  call callee

  mov ebp, eax
  shr ebx, 1
  // ebx = ebx / 2

.L4:
  test  ebx, ebx
  je  epilogue
  // jmp to end if ebx == 0

.L5:
  test  bl, 1
  // does rbp have an odd value
  je  .L3
  // if it does, jmp to .L3


  // if even
  // prepare initital_caller-saved 
  // registers and setup 
  // args for callee call

  mov edx, r13d
  // set modulo
  mov esi, ebp
  // set exp
  mov edi, r12d
  // set base 
  call  callee
  // int callee (int base, int exp, int modulo)
  mov r12d, eax
  // r12d = eax (return value from callee)
  sub ebx, 1
  jmp .L4

epilogue:
  mov eax, r12d
  add rsp, 8

  pop rbx
  pop rbp
  pop r12
  pop r13
  ret
