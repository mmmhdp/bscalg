.intel_syntax noprefix
.text
.globl start
// --- on entry edi = x (unsigned) ---
// --- returns 1 if prime, 0 if composite ---
start:
    mov eax, 0
    cmp edi, 1
    jbe composite
    cmp edi, 3
    jbe prime
    test dil, 1
    je  composite
    mov ecx, 2
    
loop:
    cmp ecx, 2
    je start_fix

    add	ecx, 2
    mov	eax, ecx
    imul eax, ecx
    cmp	edi, eax
    jb	prime
    mov	eax, edi
    xor	edx, edx
    div	ecx
    test edx, edx

    jne	loop

composite:
    mov     eax, 0
    ret

prime:
    mov     eax, 1
    ret    

start_fix:
    mov ecx, 1
    jmp loop
