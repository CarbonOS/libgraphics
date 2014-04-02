#include <stdio.h>
#include <graphics.h>
#include <device.h>
#include "private.h"

/* Include Static Functions */
//#include "surface.c"
//#include "drawing.c"
//#include "shaders.c"
//#include "buffer.c"

/* Summary */
static int galReleaseDevice(IDevice *pContext)
{
	GIDevice *idevice = NULL;
	if ((idevice = (GIDevice*)pContext) == NULL)
	{
		return GI_ERROR;
	}

	free(idevice);
	return GI_SUCCESS;
}

static inline int ClearRasterState(GIDevice *icontext)
{
	struct pipe_context *context;
	if ((context = icontext->context) == NULL)
	{
		return GI_ERROR;
	}

	if ((icontext->rs_state == NULL))
	{
		return 0;
	}

	context->bind_rasterizer_state(context, NULL);
	context->delete_rasterizer_state(context, icontext->rs_state);
	icontext->rs_state = NULL;

	return 0;
}

static inline int UpdateRasterState(GIDevice *icontext)
{
	struct pipe_context *context;
	if ((context = icontext->context) == NULL)
	{
		return GI_ERROR;
	}

	void *rs_state;
	ClearRasterState(icontext);
	struct pipe_rasterizer_state rstate;
	memset(&rstate, 0, sizeof(rstate));
	rstate.line_width = 4.0f;
	rstate.line_smooth = 1;
	rstate.gl_rasterization_rules = 1;
	if ((rs_state = CreateRasterizerState(context, &rstate)) == NULL)
	{
		return 0;
	}

	context->bind_rasterizer_state(context, rs_state);
	icontext->rs_state = rs_state;
	return 1;
}


/* Set the transformation for this Graphics context. */
static int galSetTransform(IDevice *pContext, Matrix4x4F *pMatrix)
{
	GIDevice *icontext = NULL;
	if ((icontext = (GIDevice*)pContext) == NULL)
	{
		return GI_ERROR;
	}

	if ((pMatrix == NULL))
	{
		return GI_ERROR;
	}

	memcpy((void*)&icontext->transform, (void*)pMatrix, sizeof(Matrix4x4F));

	pipe_buffer_write(icontext->context, icontext->constant, 16, sizeof(icontext->transform), &icontext->transform);

	return GI_SUCCESS;

}

/* Set the surface to which the Device context will now render. */
static int galSetRenderTarget(IDevice *pDevice, ISurface *pSurface)
{
	GIDevice *icontext = NULL;
	if ((icontext = (GIDevice*)pDevice) == NULL)
	{
		return GI_ERROR;
	}

	GISurface *isurface = NULL;
	if ((isurface = (GISurface*)pSurface) == NULL)
	{
		return GI_ERROR;
	}

	struct pipe_context *context;
	if ((context = icontext->context) == NULL)
	{
		return GI_ERROR;
	}

	struct pipe_resource *resource;
	if ((resource = isurface->resource) == NULL)
	{
		return GI_ERROR;
	}

	struct pipe_surface *surface;
	if ((surface = isurface->surface) == NULL)
	{
		return GI_ERROR;
	}

	struct pipe_viewport_state viewport;
	memset(&viewport, 0, sizeof(viewport));
	viewport.scale[0] = resource->width0;
	viewport.scale[1] = resource->height0;
	viewport.scale[2] = 1.0f;
	viewport.scale[3] = 1.0f;
	context->set_viewport_state(context, &viewport);

	struct pipe_framebuffer_state state;
	memset(&state, 0, sizeof(state));
	state.nr_cbufs = 1;
	state.width = resource->width0;
	state.height = resource->height0;
	state.cbufs[0] = surface;
	context->set_framebuffer_state(context, &state);

	float transform[4];
	transform[0] = resource->width0;
	transform[1] = resource->height0;
	transform[2] = 1;
	transform[3] = 1;
	pipe_buffer_write(context, icontext->constant, 0, sizeof(transform), transform);

	return GI_SUCCESS;
}

static int galClear(IDevice *pDevice, Color4F color)
{
	GIDevice *idevice = NULL;
	if ((idevice = (GIDevice*)pDevice) == NULL)
	{
		return GI_ERROR;
	}

	struct pipe_context *context;
	if ((context = idevice->context) == NULL)
	{
		return GI_ERROR;
	}

	union pipe_color_union c1;
	c1.f[0] = color.r;
	c1.f[1] = color.g;
	c1.f[2] = color.b;
	c1.f[3] = color.a;

	context->clear(context, PIPE_CLEAR_COLOR, &c1, 0, 0);

	return GI_SUCCESS;
}

static int galFlush(IDevice *pDevice)
{
	GIDevice *idevice = NULL;
	if ((idevice = (GIDevice*)pDevice) == NULL)
	{
		return GI_ERROR;
	}

	struct pipe_context *context;
	if ((context = idevice->context) == NULL)
	{
		return GI_ERROR;
	}

	context->flush(context, NULL);
	return GI_SUCCESS;
}

/* Summary */
/*
int galCreateContext(IDevice *pDevice, IContext **ppContext)
{
	GIDevice *idevice = NULL;
	if ((idevice = (GIDevice*)pDevice) == NULL)
	{
		return GI_ERROR;
	}

	if (ppContext == NULL)
	{
		return GI_ERROR;
	}

	struct pipe_context *context;
	if (((context = idevice->context) == NULL))
	{
		return GI_ERROR;
	}

	GIContext *icontext = NULL;
	if ((icontext = (GIContext*)calloc(1, sizeof(GIContext))) == NULL)
	{
		context->destroy(context);
		return GI_ERROR;
	}

	icontext->handle = 0;
	icontext->context = context;
	icontext->base.Release = galReleaseContext;
	icontext->base.DrawLineF = galDrawLine;
	icontext->base.DrawImage = galDrawImage;
	icontext->base.DrawRectangleF = galDrawRectangle;
	icontext->base.SetRenderTarget = galSetRenderTarget;
	icontext->base.SetTransform = galSetTransform;
	icontext->base.Clear = galClear;
	icontext->base.Flush = galFlush;

	MatrixIdentity(&icontext->transform);
	UpdateRasterState(icontext);
	galCreateVertexShader(icontext);
	galCreateColorPixelShader(icontext);
	galCreateImagePixelShader(icontext);
	galCreateVertexElementState(icontext);
	galCreateStaticVertexBuffer(icontext);
	galCreateStaticConstantBuffer(icontext);
	galCreateSamplerState(icontext);
	galCreateBlendState(icontext);
	(*ppContext) = &icontext->base;

	return GI_SUCCESS;
}
*/

