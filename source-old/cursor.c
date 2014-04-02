#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <drm/drm.h>
#include <drm/drm_mode.h>
#include <sys/mman.h>
#include <system/io.h>
#include "graphics.h"
#include "device.h"
#include "private.h"
#include <errno.h>

int CreateCursor(AdapterPtr pAdapter, CursorPtr *ppCursor)
{
	IAdapter *iadapter;
	if ((iadapter = (IAdapter*)pAdapter) == NULL)
	{
		return GI_ERROR;
	}

	IBuffer *ibuffer;
	if (CreateBuffer(iadapter, &ibuffer, 64, 64, 32) != GI_SUCCESS)
	{
		return GI_ERROR;
	}

//	AddFramebuffer(ibuffer);

//	uint32_t name;
//	GetBufferName(ibuffer, &name);
//	OpenBufferFromName(iadapter, NULL, name);

	(*ppCursor) = ibuffer;
	return GI_SUCCESS;
}

int DestroyCursor(CursorPtr pCursor)
{
	IBuffer *ibuffer;
	if ((ibuffer = (IBuffer*)pCursor) == NULL)
	{
		return GI_ERROR;
	}

	return GI_SUCCESS;
}

/* ... */
int SetDisplayCursor(DisplayPtr pDisplay, CursorPtr pCursor)
{
	IDisplay *idisplay = NULL;
	if ((idisplay = (IDisplay*)pDisplay) == NULL)
	{
		return GI_ERROR;
	}

	IBuffer *ibuffer = NULL;
	if ((ibuffer = (IBuffer*)pCursor) == NULL)
	{
		return GI_ERROR;
	}

	struct drm_mode_cursor arg;
	arg.flags = DRM_MODE_CURSOR_BO;
	arg.crtc_id = idisplay->crtc;
	arg.width = ibuffer->width;
	arg.height = ibuffer->height;
	arg.handle = ibuffer->handle;

	if (IoControl(idisplay->fd, DRM_IOCTL_MODE_CURSOR, &arg))
	{
		return GI_ERROR;
	}

	return GI_SUCCESS;
}

/* ... */
int MoveCursor(DisplayPtr pDisplay, int x, int y)
{
	IDisplay *idisplay = NULL;
	if ((idisplay = (IDisplay*)pDisplay) == NULL)
	{
		return GI_ERROR;
	}

	struct drm_mode_cursor args;
	args.flags = DRM_MODE_CURSOR_MOVE;
	args.crtc_id = idisplay->crtc;
	args.x = (uint32_t)x;
	args.y = (uint32_t)y;

	if (IoControl(idisplay->fd, DRM_IOCTL_MODE_CURSOR, &args))
	{
		return GI_ERROR;
	}

	return GI_SUCCESS;
}

int HideCursor(DisplayPtr pDisplay)
{
	IDisplay *idisplay = NULL;
	if ((idisplay = (IDisplay*)pDisplay) == NULL)
	{
		return GI_ERROR;
	}

	struct drm_mode_cursor args;
	args.flags = DRM_MODE_CURSOR_BO;
	args.crtc_id = idisplay->crtc;
	args.width = 0;
	args.height = 0;
	args.handle = 0;

	if (IoControl(idisplay->fd, DRM_IOCTL_MODE_CURSOR, &args))
	{
		return GI_ERROR;
	}

	return GI_SUCCESS;
}


