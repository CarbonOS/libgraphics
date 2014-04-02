/* X86 Platform */
#ifdef __i386__
#include "asm-x86.h"
.intel_syntax noprefix

.extern PixelPrint

#define DRAW_ARGS_X    (eax+0)
#define DRAW_ARGS_Y    (eax+4)
#define DRAW_ARGS_W    (eax+8)
#define DRAW_ARGS_H    (eax+12)

/* ASM Version */
/* int FillRectangleRGBA32(DRAWARGS *args); */
ENTRY(FillRectangleRGBA32)
	push ebp
	mov ebp, esp

#	push ebp
	push edi
	push esi
	push ebx
	push edx

	sub esp, 0x08             # local data

	mov eax, [ebp+8]
	mov edi, [DRAW_ARGS_X]   # draw x
	mov esi, [DRAW_ARGS_Y]   # draw y
	mov ebx, [DRAW_ARGS_W]   # draw w
	mov edx, [DRAW_ARGS_H]   # draw h


	# y loop
.yloop:

	cmp esi, edx
	je .exit

	# x loop
#.xloop:
#
#	cmp edi, ebx
#	je .exit
#
#	inc edi
#	jmp .xloop

# pixel print

	push esi
	call PixelPrint
	pop esi

	add esi, 1
	jmp .yloop

.exit:
	mov eax, 0
	pop edx
	pop ebx
	pop esi
	pop edi
	pop ebp
	ret
END(FillRectangleRGBA32)

#endif
