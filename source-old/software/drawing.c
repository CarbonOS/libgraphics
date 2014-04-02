#include <stdio.h>
#include <graphics.h>
#include <device.h>
#include "framebuffer.h"

#define I_MIN(a, b) (a < b ? a : b)
#define I_MAX(a, b) (a > b ? a : b)

/* Summary */
static int fillRectangleRGBA8(unsigned char *buffer, unsigned x, unsigned y, unsigned width, unsigned height, Color4F color)
{
	unsigned x1 = x;
	unsigned y1 = y;
	unsigned w1 = width;
	unsigned h1 = height;
	unsigned p1 = 0;
	unsigned p2 = 0;

	return 0;
}

/* Summary */
int fbFillRectangleWithColor(IContext *pContext, unsigned x, unsigned y, unsigned width, unsigned height, Color4F color)
{
	FBContext *fbc = NULL;
	if ((fbc = (FBContext*)pContext) == NULL)
		return GI_ERROR;

	FBSurface *target;
	if ((target = (FBSurface*)fbc->target) == NULL)
		return GI_ERROR;

	unsigned char pixel[4];
	unsigned char *data = NULL;
	if ((data = target->mapped) == NULL)
		return GI_ERROR;

	unsigned x1 = GI_MAX(0, x);
	unsigned y1 = GI_MAX(0, y);
	unsigned w1 = target->base.width;
	unsigned h1 = target->base.height;
	unsigned w2 = GI_MIN(w1, width);
	unsigned h2 = GI_MIN(h1, height);
	unsigned p1 = (target->base.bpp / 8);
	unsigned p2 = (target->base.pitch);
	unsigned p3 = 0;
	unsigned p4 = 0;
	unsigned p5 = 0;

	fillRectangleRGBA8(data, x1, y1, w2, h2, color);

	return GI_SUCCESS;
}

