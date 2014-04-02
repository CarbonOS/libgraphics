#ifndef __GRAPHICS_UTILS__
#define __GRAPHICS_UTILS__

/* Color Utils */
static void __attribute__ ((always_inline)) Color4UB2Color4F(void *color1)
{
	__asm("pxor xmm0, xmm0");

	__asm("movd xmm4, %0" :: "m"(color1));

	__asm("punpcklbw xmm4, xmm0"); // target 64Bit

	__asm("punpcklwd xmm4, xmm0"); // target 128Bit

	__asm("cvtdq2ps xmm4, xmm4"); // target > float

	return;
}

/* Color Utils */
static void __attribute__ ((always_inline)) Color4F2Color4UB(void *color1)
{
	__asm("cvttps2dq xmm5, xmm5"); // float > int

	__asm("packssdw xmm5, xmm5"); // 64Bit

	__asm("packuswb xmm5, xmm5"); // 32Bit

	//__asm("movd %0, xmm5" :: "m"(dst[sample]));

	return;
}

#endif
