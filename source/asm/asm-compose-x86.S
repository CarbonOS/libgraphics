/* X86 Platform */
#ifdef __i386__
#include "asm-x86.h"
.intel_syntax noprefix

/* ASM Version */
/* int ComposeRGBA32Chunk(DRAWARGS *args); */
ENTRY(ComposeRGBA32Chunk)
	push ebp
	mov ebp, esp
	mov eax, 0
	pop ebp
	ret
END(ComposeRGBA32Chunk)

#endif
