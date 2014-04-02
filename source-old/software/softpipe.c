#include <stdio.h>
#include <stdlib.h>
#include <state_tracker/sw_winsys.h>
#include <pipe/p_screen.h>
#include <sp_public.h>
#include "graphics.h"
#include "device.h"

static void softpipe_destroy(struct sw_winsys *winsys)
{
	return;
}


static boolean softpipe_is_format_supported(
	struct sw_winsys *ws,
	unsigned tex_usage,
	enum pipe_format format)
{
	return FALSE;
}


static struct sw_displaytarget * softpipe_displaytarget_create(
	struct sw_winsys *winsys,
	unsigned tex_usage,
	enum pipe_format format,
	unsigned width,
	unsigned height,
	unsigned alignment,
	unsigned *stride)
{
	return NULL;
}

static struct sw_displaytarget *softpipe_displaytarget_from_handle(
	struct sw_winsys *winsys,
	const struct pipe_resource *templet,
	struct winsys_handle *whandle,
	unsigned *stride)
{
	return NULL;
}

static boolean softpipe_displaytarget_get_handle(
	struct sw_winsys *winsys,
	struct sw_displaytarget *dt,
	struct winsys_handle *whandle)
{
	return FALSE;
}

static void * softpipe_displaytarget_map(
	struct sw_winsys *ws,
	struct sw_displaytarget *dt,
	unsigned flags)
{
	return NULL;
}

static void softpipe_displaytarget_unmap(
	struct sw_winsys *ws,
	struct sw_displaytarget *dt)
{
	return;
}

static void softpipe_displaytarget_display(
	struct sw_winsys *winsys,
	struct sw_displaytarget *dt,
	void *context_private)
{
	return;
}

static void softpipe_displaytarget_destroy(
	struct sw_winsys *winsys,
	struct sw_displaytarget *dt)
{
	return;
}

static struct sw_winsys winsys =
{
	.destroy = softpipe_destroy,
	.is_displaytarget_format_supported = softpipe_is_format_supported,
	.displaytarget_create = softpipe_displaytarget_create,
	.displaytarget_from_handle = softpipe_displaytarget_from_handle,
	.displaytarget_get_handle = softpipe_displaytarget_get_handle,
	.displaytarget_map = softpipe_displaytarget_map,
	.displaytarget_unmap = softpipe_displaytarget_unmap,
	.displaytarget_display = softpipe_displaytarget_display,
	.displaytarget_destroy = softpipe_displaytarget_destroy,
};

/* Only for Gallium based Adapters. */
int spGetPipeline(IAdapter *pAdapter, void **ppPipeline)
{
	struct pipe_screen *pipe;

	if ((pipe = softpipe_create_screen(&winsys)) == NULL)
	{
		return GI_ERROR;
	}

	(*ppPipeline) = (void*)pipe;

	return GI_SUCCESS;
}

