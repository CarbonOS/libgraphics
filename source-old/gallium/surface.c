#include <stdio.h>
#include <graphics.h>
#include <device.h>
#include "private.h"

/* Summary */
static int galReleaseSurface(ISurface *pSurface)
{
	GISurface *isurface = NULL;
	if ((isurface = (GISurface*)pSurface) == NULL)
	{
		return GI_ERROR;
	}

	return GI_SUCCESS;
}

static int galMapSurface(ISurface *pSurface, void **ptr)
{
	GISurface *isurface = NULL;
	if ((isurface = (GISurface*)pSurface) == NULL)
	{
		return GI_ERROR;
	}

	struct pipe_resource *resource;
	if ((resource = isurface->resource) == NULL)
	{
		return GI_ERROR;
	}
/*
	struct pipe_context *context = NULL;
	if ((context = isurface->resource->context) == NULL)
	{
		return GI_ERROR;
	}
*/
	return GI_SUCCESS;
}



/* Summary */
static int galCreateSurface(IDevice *pDevice, ISurface **ppSurface, int width, int height, int format)
{
	GIDevice *idevice = NULL;
	if ((idevice = (GIDevice*)pDevice) == NULL)
		return GI_ERROR;

	struct pipe_screen *screen = NULL;
	if ((screen = idevice->screen) == NULL)
	{
		return GI_ERROR;
	}

	struct pipe_context *context = NULL;
	if ((context = idevice->context) == NULL)
	{
		return GI_ERROR;
	}

	/* Create the Resource */
	struct pipe_resource rtempl;
	memset(&rtempl, 0, sizeof(rtempl));
	rtempl.target = PIPE_TEXTURE_RECT;
	rtempl.bind = PIPE_BIND_RENDER_TARGET|PIPE_BIND_SAMPLER_VIEW;
	rtempl.format = PIPE_FORMAT_R8G8B8A8_UNORM;
	rtempl.width0 = width;
	rtempl.height0 = height;
	rtempl.nr_samples = 1;
	rtempl.array_size = 1;
	rtempl.depth0 = 1;

	struct pipe_resource *resource; // texture
	if ((resource = screen->resource_create(screen, &rtempl)) == NULL)
	{
		printf("resource_create::failed!\n");
		return GI_ERROR;
	}

	/* Create the Render Target */
	struct pipe_surface stempl;
	memset(&stempl, 0, sizeof(stempl));
	stempl.format = resource->format;
	stempl.usage = PIPE_BIND_RENDER_TARGET;
	stempl.u.tex.level = 0;
	stempl.u.tex.first_layer = 0;
	stempl.u.tex.last_layer = 0;

	struct pipe_surface *surface; // render target
	if ((surface = context->create_surface(context, resource, &stempl)) == NULL)
	{
		printf("create_surface::failed!\n");
		screen->resource_destroy(screen, resource);
		return GI_ERROR;
	}

	/* Create the Render Target View */
	struct pipe_sampler_view vtempl;
	memset(&vtempl, 0, sizeof(vtempl));
	vtempl.format = resource->format;
	vtempl.texture = resource;
	vtempl.swizzle_r = 0;
	vtempl.swizzle_g = 1;
	vtempl.swizzle_b = 2;
	vtempl.swizzle_a = 3;

	struct pipe_sampler_view *view; // sample view
	if ((view = context->create_sampler_view(context, resource, &vtempl)) == NULL)
	{
		printf("create_sampler_view::failed!\n");
		context->surface_destroy(context, surface);
		screen->resource_destroy(screen, resource);
		return GI_ERROR;
	}

	GISurface *isurface = NULL;
	if ((isurface = (GISurface*)calloc(1, sizeof(GISurface))) == NULL)
	{
		printf("calloc::failed!\n");
		context->sampler_view_destroy(context, view);
		context->surface_destroy(context, surface);
		screen->resource_destroy(screen, resource);
		return GI_ERROR;
	}

	isurface->view = view;
	isurface->surface = surface;
	isurface->resource = resource;
//	isurface->base.Release = galReleaseSurface;
	isurface->base.height = height;
	isurface->base.width = width;
	(*ppSurface) = &isurface->base;

	return GI_SUCCESS;
}

/* Summary */
static int galCreateDisplaySurface(IDevice *pDevice, DisplayInfo *info, ISurface **ppSurface)
{
	GIDevice *idevice = NULL;
	if ((idevice = (GIDevice*)pDevice) == NULL)
	{
		return GI_ERROR;
	}

	if ((info == NULL) || (ppSurface == NULL))
	{
		return GI_ERROR;
	}

	struct pipe_screen *screen = NULL;
	if ((screen = idevice->screen) == NULL)
	{
		return GI_ERROR;
	}

	struct pipe_context *context = NULL;
	if ((context = idevice->context) == NULL)
	{
		return GI_ERROR;
	}


	if ((screen->resource_from_handle == NULL))
	{
		printf("resource_from_handle::Unimplemented!\n");
		return GI_UNIMPLEMENTED;
	}

	/* Create the Resource */
	struct pipe_resource templ;
	memset(&templ, 0, sizeof(templ));
	templ.target = PIPE_TEXTURE_RECT;
	templ.bind = PIPE_BIND_RENDER_TARGET|PIPE_BIND_DISPLAY_TARGET;
	templ.format = PIPE_FORMAT_B8G8R8X8_UNORM;
	templ.width0 = info->width;
	templ.height0 = info->height;
	templ.nr_samples = 1;
	templ.array_size = 1;
	templ.depth0 = 1;

	struct winsys_handle whandle;
	whandle.handle = info->handle;
	whandle.stride = info->pitch;
	whandle.type = 0;

	struct pipe_resource *resource; // texture
	if ((resource = screen->resource_from_handle(screen, &templ, &whandle)) == NULL)
	{
		printf("resource_from_handle::failed!\n");
		return GI_ERROR;
	}

	/* Create the Render Target */
	struct pipe_surface stempl;
	memset(&stempl, 0, sizeof(stempl));
	stempl.format = resource->format;
	stempl.usage = PIPE_BIND_RENDER_TARGET;
	stempl.u.tex.level = 0;
	stempl.u.tex.first_layer = 0;
	stempl.u.tex.last_layer = 0;

	struct pipe_surface *surface; // render target
	if ((surface = context->create_surface(context, resource, &stempl)) == NULL)
	{
		printf("create_surface::failed!\n");
		screen->resource_destroy(screen, resource);
		return GI_ERROR;
	}

	GISurface *isurface = NULL;
	if ((isurface = (GISurface*)calloc(1, sizeof(GISurface))) == NULL)
	{
		return GI_ERROR;
	}

	isurface->view = NULL;
	isurface->surface = surface;
	isurface->resource = resource;
//	isurface->base.Release = galReleaseSurface;
	isurface->base.height = info->height;
	isurface->base.width = info->width;
	(*ppSurface) = &isurface->base;

	return GI_SUCCESS;
}

