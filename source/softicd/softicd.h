#ifndef __SOFTWARE_ICD__
#define __SOFTWARE_ICD__

/* Includes */
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <assert.h>
#include <sys/mman.h>
#include <system/task.h>
//#include <xmmintrin.h>
//#include <emmintrin.h>
#include "private.h"

/* Software Effect Object */
typedef struct __IEffectSP
{
	IEffect base;
//	__m128 stack[24];

} IEffectSP;

/* Software RenderTarget Object */
typedef struct __IRenderTargetSP
{
	IRenderTarget base;
	int fd;

} IRenderTargetSP;

/* Software Context object */
typedef struct __IContextSP
{
	IContext base;
	HTASK task[4];
	IRenderTargetSP *target;
	IEffectSP *effect;
	int threads;
	RECT clip;

} IContextSP;

/* RenderState Object */
typedef struct __IRenderState
{
	IRenderTarget base;
	int fd;

} IRenderState;

/* Drawing Arguments */
typedef struct __DRAWARGS
{
	int x;
	int y;
	int w;
	int h;
	struct {
		int width;
		int height;
		int pitch;
		void *data;
	} dst;
	struct {
		int width;
		int height;
		int pitch;
		void *data;
	} src;
	COLOR color1;
	COLOR color2;

} DRAWARGS;

/* Drawing Helper */
typedef struct __FILLRECTARGS
{
	DRAWARGS base;

} FILLRECTARGS;

/* Drawing Helper */
typedef struct __COLOR4UB
{
	unsigned char r;
	unsigned char g;
	unsigned char b;
	unsigned char a;

} COLOR4UB;

#endif
