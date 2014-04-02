	.file	"effectvm.c"
	.intel_syntax noprefix
	.text
	.p2align 4,,15
	.globl	effectvm_run
	.type	effectvm_run, @function
effectvm_run:
.LFB489:
	.cfi_startproc
	sub	esp, 220
	.cfi_def_cfa_offset 224
	mov	eax, DWORD PTR [esp+224]
	fld	DWORD PTR .LC0
	fst	DWORD PTR [esp+4]
	fstp	DWORD PTR [esp]
	call	[DWORD PTR [eax+192]]
#APP
# 38 "effectvm.c" 1
	movups (eax), %xmm0
# 0 "" 2
# 39 "effectvm.c" 1
	mulps %xmm0, %xmm0
# 0 "" 2
#NO_APP
	lea	eax, [esp+16]
#APP
# 41 "effectvm.c" 1
	movups (eax), %xmm0
# 0 "" 2
#NO_APP
	lea	eax, [esp+32]
#APP
# 42 "effectvm.c" 1
	movups (eax), %xmm1
# 0 "" 2
#NO_APP
	add	esp, 220
	.cfi_def_cfa_offset 4
	ret
	.cfi_endproc
.LFE489:
	.size	effectvm_run, .-effectvm_run
	.section	.rodata.cst4,"aM",@progbits,4
	.align 4
.LC0:
	.long	1120403456
	.ident	"GCC: (GNU) 4.7.1"
	.section	.note.GNU-stack,"",@progbits
