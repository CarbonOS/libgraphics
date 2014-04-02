#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <linux/types.h>
#include <linux/limits.h>
#include <linux/fb.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <graphics.h>
#include <device.h>
#include "framebuffer.h"

/* Summary */
/* Retrieve the display information. */
static int fbGetDisplayInfo(IDisplay *pDisplay, DisplayInfo *info)
{
	FBDISPLAY(pDisplay);

	if ((!fbd->adapter) || (!info))
		return GI_ERROR;

	info->x = (int)fbd->var.xoffset;
	info->y = (int)fbd->var.yoffset;
	info->width = (int)fbd->var.xres;
	info->height = (int)fbd->var.yres;
	info->pitch = (int)fbd->fix.line_length;
	info->bpp = (int)fbd->var.bits_per_pixel;
	info->handle = (int)fbd->handle;
	info->id = (int)0;

	return GI_SUCCESS;
}

/* Summary */
static int fbMapDisplaySurface(ISurface *pSurface, void **ptr)
{
	void *data = NULL;
	FBSurface *fbs = NULL;

	if (!(fbs = (FBSurface*)pSurface) || (!ptr)) 
		return GI_ERROR;

	if ((fbs->state == 1) && (fbs->mapped != NULL))
	{
		(*ptr) = fbs->mapped;
		return GI_SUCCESS;
	}

	if ((fbs->handle < 0) || (fbs->length <= 0))
	{
		fbs->state = 0;
		fbs->mapped = NULL;
		return GI_ERROR;
	}

	if ((data = mmap(NULL, fbs->length, PROT_READ|PROT_WRITE, MAP_SHARED, fbs->handle, 0)) == MAP_FAILED)
	{
		fbs->state = 0;
		fbs->mapped = NULL;
		return GI_ERROR;
	}

	fbs->state = 1;
	fbs->mapped = data;
	(*ptr) = data;
	return GI_SUCCESS;
}

/* Summary */
static int fbUnmapDisplaySurface(ISurface *pSurface)
{
	FBSurface *fbs = NULL;

	if (!(fbs = (FBSurface*)pSurface)) 
		return GI_ERROR;

	if ((fbs->state == 0) || (fbs->mapped == NULL))
		return GI_ERROR;

	munmap(fbs->mapped, fbs->length);
	fbs->mapped = NULL;
	fbs->state = 0;

	return GI_SUCCESS;
}

/* Summary */
static int fbReleaseDisplaySurface(ISurface *pSurface)
{
	FBSurface *fbs = NULL;

	if (!(fbs = (FBSurface*)pSurface)) 
		return GI_ERROR;

	fbUnmapDisplaySurface(pSurface);
	free(fbs);

	return GI_SUCCESS;
}

/* Summary */
/* Retrieve the display surface (front buffer). */
static int fbGetDisplaySurface(IDisplay *pDisplay, ISurface **ppSurface)
{
	FBDISPLAY(pDisplay);

	if ((!fbd->adapter) || (!ppSurface))
		return GI_ERROR;

	FBSurface *surface = NULL;
	if ((surface = (FBSurface*)calloc(1, sizeof(FBSurface))) == NULL)
	{
		return GI_ERROR;
	}

	void *data = NULL;
	size_t length = (fbd->var.yres * fbd->fix.line_length);
	if ((data = mmap(NULL, length, PROT_READ|PROT_WRITE, MAP_SHARED, fbd->handle, 0)) == MAP_FAILED)
	{
		free(surface);
		return GI_ERROR;
	}

	surface->state = 0;
	surface->mapped = data;
	surface->length = length;
	surface->handle = fbd->handle;
	surface->base.width = (int)fbd->var.xres;
	surface->base.height = (int)fbd->var.yres;
	surface->base.pitch = (int)fbd->fix.line_length;
	surface->base.bpp = (int)fbd->var.bits_per_pixel;
	surface->base.format = 0;
	surface->base.Map = fbMapDisplaySurface;
	surface->base.Unmap = fbUnmapDisplaySurface;
	surface->base.Release = fbReleaseDisplaySurface;
	(*ppSurface) = &surface->base;

	return GI_SUCCESS;
}



/* Summary */
static int fbDisplayGetAdapter(IDisplay *pDisplay, IAdapter **ppAdapter)
{
	FBDISPLAY(pDisplay);

	if ((!fbd->adapter) || (!ppAdapter))
		return GI_ERROR;

	(*ppAdapter) = &fbd->adapter->base;
	return GI_SUCCESS;
}

/* Summary */
static int fbReleaseDisplay(IDisplay *pDisplay)
{
	FBDISPLAY(pDisplay);
	close(fbd->handle);
	free(fbd);
	return GI_SUCCESS;
}

/* Summary */
/* Enumerate adapter (video card) outputs (displays). */
int fbEnumDisplays(IAdapter *pAdapter, int index, IDisplay **ppDisplay)
{
	FBADAPTER(pAdapter);

	if ((index < 0) || (!ppDisplay))
		return GI_ERROR;

	int handle = -1;
	char filename[PATH_MAX] = { 0 };
	sprintf(filename, "%s%d", DEVICES_FB, index);
	if ((handle = open(filename, O_RDWR)) < 0)
	{
		return GI_ERROR;
	}

	FBDisplay *display = NULL;
	if ((display = (FBDisplay*)calloc(1, sizeof(FBDisplay))) == NULL)
	{
		return GI_ERROR;
	}

	if (ioctl(handle, FBIOGET_FSCREENINFO, &display->fix) < 0)
	{
		close(handle);
		free(display);
		return GI_ERROR;
	}

	if (ioctl(handle, FBIOGET_VSCREENINFO, &display->var) < 0)
	{
		close(handle);
		free(display);
		return GI_ERROR;
	}

	display->adapter = fba;
	display->handle = handle;
	display->base.SetCursor = NULL;
	display->base.SetSurface = NULL;
	display->base.WaitForVBlank = NULL;
	display->base.GetDisplayInfo = fbGetDisplayInfo;
	display->base.GetSurface = fbGetDisplaySurface;
	display->base.GetAdapter = fbDisplayGetAdapter;
	display->base.Release = fbReleaseDisplay;
	(*ppDisplay) = &display->base;

	return GI_SUCCESS;
}
