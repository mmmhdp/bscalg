	.file	"problem_agf.c"
	.intel_syntax noprefix
	.text
	.p2align 4
	.globl	foo
	.type	foo, @function
foo:
.LFB23:
	.cfi_startproc
	endbr64
	mov	eax, edi
	imul	eax, edi
	imul	eax, edi
	test	eax, eax
	js	.L4
	ret
	.p2align 4,,10
	.p2align 3
.L4:
	imul	esi, esi
	add	eax, 17
	add	edi, edi
	sub	eax, edi
	sub	eax, esi
	ret
	.cfi_endproc
.LFE23:
	.size	foo, .-foo
	.ident	"GCC: (Ubuntu 13.3.0-6ubuntu2~24.04) 13.3.0"
	.section	.note.GNU-stack,"",@progbits
	.section	.note.gnu.property,"a"
	.align 8
	.long	1f - 0f
	.long	4f - 1f
	.long	5
0:
	.string	"GNU"
1:
	.align 8
	.long	0xc0000002
	.long	3f - 2f
2:
	.long	0x3
3:
	.align 8
4:
