#include <stdio.h>
#include <graphics.h>
#include <device.h>
#include "framebuffer.h"

/* Summary */
static int fbSetRenderTarget(IContext *pContext, ISurface *pSurface)
{
	FBContext *fbc = NULL;
	if ((fbc = (FBContext*)pContext) == NULL)
		return GI_ERROR;

	fbc->clipping.x = 0;
	fbc->clipping.y = 0;
	fbc->clipping.width = 0;
	fbc->clipping.height = 0;
	fbc->target = pSurface;

	return GI_SUCCESS;
}

/* Summary */
static int fbSetClipping(IContext *pContext, unsigned x, unsigned y, unsigned width, unsigned height)
{
	FBContext *fbc = NULL;
	if ((fbc = (FBContext*)pContext) == NULL)
		return GI_ERROR;

	fbc->clipping.x = GI_MAX(x, 0);
	fbc->clipping.y = GI_MAX(y, 0);
	fbc->clipping.width = width;
	fbc->clipping.height = height;

	return GI_SUCCESS;
}

/* Summary */
static int fbReleaseContext(IContext *pContext)
{
	FBContext *fbc = NULL;
	if ((fbc = (FBContext*)pContext) == NULL)
		return GI_ERROR;

	free(fbc);
	fbc = NULL;
	return GI_SUCCESS;
}

/* Summary */
int fbCreateContext(IDevice *pDevice, IContext **ppContext)
{
	FBDevice *fbd = NULL;
	if ((fbd = (FBDevice*)pDevice) == NULL)
		return GI_ERROR;

	if (ppContext == NULL)
		return GI_ERROR;

	FBContext *context = NULL;
	if ((context = (FBContext*)calloc(1, sizeof(FBContext))) == NULL)
	{
		return GI_ERROR;
	}

	context->device = fbd;
	context->target = NULL;
	context->handle = fbd->handle;
	context->base.SetClipping = fbSetClipping;
	context->base.SetRenderTarget = fbSetRenderTarget;
	context->base.FillRectangleWithColor = fbFillRectangleWithColor;
	context->base.Release = fbReleaseContext;
	(*ppContext) = &context->base;

	return GI_SUCCESS;
}

