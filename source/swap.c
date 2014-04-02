#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <drm/drm.h>
#include <drm/drm_mode.h>
#include "graphics.h"
#include "device.h"
#include "private.h"

static int CreateSwapBuffer(struct IDevice *device, IBuffer *buffer, unsigned int width, unsigned int height, unsigned int bpp)
{
	assert((width > 0));
	assert((height > 0));
	assert((device != NULL));
	assert((buffer != NULL));

	struct drm_get_cap cap;
	memset(&cap, 0, sizeof(cap));	
	cap.capability = DRM_CAP_DUMB_BUFFER;
	if (ioctl(device->fd, DRM_IOCTL_GET_CAP, &cap) || (!cap.value)) {
		return GI_ERROR;
	}

	struct drm_mode_create_dumb args;
	memset(&args, 0, sizeof(args));
	args.width = width;
	args.height = height;
	args.bpp = bpp;
	if (ioctl(device->fd, DRM_IOCTL_MODE_CREATE_DUMB, &args)) {
		return GI_ERROR;
	}

	struct drm_mode_fb_cmd fbo = { .width  = width, .height = height, .pitch = args.pitch, .bpp = args.bpp, .depth = 24, .handle = args.handle };
	if (ioctl(device->fd, DRM_IOCTL_MODE_ADDFB, &fbo))
	{
		return GI_ERROR;
	}

	buffer->fb = fbo.fb_id;
	buffer->fd = device->fd;
	buffer->bpp = args.bpp;
	buffer->height = args.height;
	buffer->width = args.width;
	buffer->stride = args.pitch;
	buffer->handle = args.handle;
	buffer->size = args.size;
	buffer->shared = -1;
	return GI_SUCCESS;
}

/* Creates a new Swap Chain Object. */
HSWAPCHAIN CreateSwapChain(HDEVICE device, int width, int height, int bpp)
{
	struct IDevice *idevice;
	if ((idevice = (struct IDevice*)device) == NULL)
	{
		return (HSWAPCHAIN)0;
	}

	if ((width < 16) || (height < 16) || (bpp < 24))
	{
		return (HSWAPCHAIN)0;
	}

	ISwapChain *iswap;
	if ((iswap = (ISwapChain*)calloc(1, sizeof(ISwapChain))) == NULL)
	{
		return (HSWAPCHAIN)0;
	}

	if (CreateSwapBuffer(idevice, &iswap->buffer[0], (uint32_t)width, (uint32_t)height, (uint32_t)bpp) != GI_SUCCESS)
	{
		free(iswap);
		return (HSWAPCHAIN)0;
	}

	if (CreateSwapBuffer(idevice, &iswap->buffer[1], (uint32_t)width, (uint32_t)height, (uint32_t)bpp) != GI_SUCCESS)
	{
		free(iswap);
		return (HSWAPCHAIN)0;
	}

	iswap->front = 0;
	iswap->count = 2;
	iswap->width = (uint32_t)width;
	iswap->height = (uint32_t)height;
	iswap->fd = idevice->fd;
	return (HSWAPCHAIN)iswap;
}

int SwapChainFlip(HSWAPCHAIN swap, HDISPLAY display)
{
	ISwapChain *iswap;
	if (!(iswap = (ISwapChain*)swap)) {
		return GI_ERROR;
	}

	IDisplay *idisplay;
	if (!(idisplay = (IDisplay*)display)) {
		return 0;
	}

	IBuffer *buffer = &iswap->buffer[0];
	struct drm_mode_crtc_page_flip args;
	args.fb_id = buffer->fb;
	args.crtc_id = idisplay->crtc;
	args.flags = DRM_MODE_PAGE_FLIP_EVENT;
	args.user_data = 0;
	args.reserved = 0;
	if (ioctl(iswap->fd, DRM_IOCTL_MODE_PAGE_FLIP, &args))
	{
		return GI_ERROR;
	}

	iswap->front ^= 1;
	printf("font: %d\n", iswap->front);
	return GI_SUCCESS;
}



