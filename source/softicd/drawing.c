/* Software Drawing */

#ifndef POFFSET
#define POFFSET(p1, w1, p2, w2) \
    (((p2 + w1) > w2) ? (w1 - ((p2 + w1) - w2) - p2) : (w1 - p2))
#endif

int FillRectangleRGBA32(DRAWARGS *args);


struct var
{
	float r;
	float g;
	float b;
	float a;
};
int FillRectangleChunk2(DRAWARGS *args)
{
	int ypos = 0;
	int xpos = 0;
	int sample = 0;;

	assert((args != NULL));
	assert((args->dst.data != NULL));
	COLOR4UB *color = ((COLOR4UB*)args->dst.data);

	unsigned int d = 0x437f0000; // 255.0f;
	__asm("movd (%0), %%xmm1" :: "r"(&d));
	__asm("movups (%0), %%xmm3" :: "r"(&args->color1));
	__asm("pshufd $0x0, %xmm1, %xmm1");

	ypos = args->y;
	while (ypos < args->h) {

		xpos = args->x;
		while (xpos < args->w)
		{
			sample = ((ypos * args->dst.width) + xpos);
/*
			__asm("pxor %xmm0, %xmm0");
			__asm("movd (%0), %%xmm2" :: "r"(&color[sample]));
			__asm("punpcklbw %xmm0, %xmm2");
			__asm("punpcklwd %xmm0, %xmm2");
			__asm("cvtdq2ps %xmm2, %xmm2");
			__asm("divps %xmm1, %xmm2");

			__asm("movaps %xmm3, %xmm2");

			__asm("mulps %xmm1, %xmm2");
			__asm("cvttps2dq %xmm2, %xmm2");    // float > int
			__asm("packssdw %xmm2, %xmm2");     // 64Bit
			__asm("packuswb %xmm2, %xmm2");     // 32Bit
			__asm("movd %%xmm2, (%0)" :: "r"(&color[sample]));
*/
			color[sample].r = 0;
			color[sample].g = 0;
			color[sample].b = 255;
			color[sample].a = 255;
			xpos++;

		}

		ypos++;
	}

	return 1;
}


/* Software FillRectangle */
static int FillRectangleSP(IContext *handle, COLOR *color, RECT *bounds)
{
	IContextSP *context = NULL;
	if (!(context = (IContextSP*)handle)) {
		return 0;
	}

	IRenderTargetSP *target = NULL;
	if (!(target = context->target)) {
		return 0;
	}

	int xd = ((bounds->x < 0)) ? 0 : bounds->x;
	int yd = ((bounds->y < 0)) ? 0 : bounds->y;
	int xs = ((bounds->x < 0)) ? (0 - bounds->x) : 0;
	int ys = ((bounds->y < 0)) ? (0 - bounds->y) : 0;
	int cx = (POFFSET(x, bounds->width, xs, target->base.width));
	int cy = (POFFSET(y, bounds->height, ys, target->base.height));
	int ix = (0);
	int iy = (0);

	void *dstptr = NULL;
	if (!(dstptr = MapRenderTargetSP(target))) {
		return 0;
	}

	DRAWARGS args;
	args.x = xd;
	args.y = yd;
	args.w = cx;
	args.h = cy;
	args.dst.width = target->base.width;
	args.dst.height = target->base.height;
	args.color1.r = color->r;
	args.color1.g = color->g;
	args.color1.b = color->b;
	args.color1.a = color->a;

//	printf("off-x: %d\n", __builtin_offsetof(DRAWARGS, x));
//	printf("off-y: %d\n", __builtin_offsetof(DRAWARGS, y));
//	printf("off-w: %d\n", __builtin_offsetof(DRAWARGS, w));
//	printf("off-h: %d\n", __builtin_offsetof(DRAWARGS, h));
//	printf("off-dst.width: %d\n", __builtin_offsetof(DRAWARGS, dst.width));
//	printf("off-dst.data: %d\n", __builtin_offsetof(DRAWARGS, dst.data));

	args.dst.data = (dstptr + ((xs * 4) + (ys * target->base.stride)));
	FillRectangleChunk2(&args);
/*
	int index;
	div_t dY_block = div(cy, context->threads);

	int y1 = (cy / (context->threads));
	int y2 = (cy % (context->threads));

	printf("%d, %d . %d\n", y1, y2, cy);
	printf("%d, %d\n", dY_block.quot, dY_block.rem);

	int dY_start = 0;
	int dY_end = dY_block.quot + dY_block.rem;

	for (index = 0; index < (context->threads); ++index)
	{
		printf("%d, %d\n", dY_start, dY_end);
		dY_start = dY_end;
		dY_end += dY_block.quot;

	}
*/

	return 1;
}


