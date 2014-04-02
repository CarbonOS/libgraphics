#include <stdio.h>
#include <xmmintrin.h>
#include <emmintrin.h>

struct pixel
{
	float r;
	float g;
	float b;
	float a;
};

struct reg
{
	float x;
	float y;
	float z;
	float w;
};

struct pos
{
	float x;
	float y;
};

struct effectvm
{
	struct reg tmp[12];
	struct pixel* (*getpixel)(float x, float y);	
};

void effectvm_run(struct effectvm *vm)
{
	volatile struct reg tmp[12];
	struct pixel *pixel;
	pixel = vm->getpixel(100, 100);
	__asm("movups (%0), %%xmm0" :: "r"(pixel));
	__asm("mulps %xmm0, %xmm0");

	__asm("movups (%0), %%xmm0" :: "r"(&tmp[0]));
	__asm("movups (%0), %%xmm1" :: "r"(&tmp[1]));
	return;
}

static struct pixel* evm_getpixel(int x, int y)
{
	static struct pixel p;
	return &p;
}

	
