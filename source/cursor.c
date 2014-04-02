#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "graphics.h"
#include "device.h"
#include "private.h"
#include "cursor.h"

/* Creates a new Cursor Object. */
HCURSOR CreateCursor(HDEVICE handle)
{
	struct IDevice *device;
	device = GetDevice(handle);
	RETURN_IF_NULL(device);

	struct IResource *res = NULL;
	if (!(res = (struct IResource*)calloc(1, sizeof(struct IResource)))) {
		return NULL;
	}

	if (CreateResource(device, res, 64, 64, 32) != GI_SUCCESS) {
		free(res);	
		return NULL;
	}

	return (HCURSOR)res;
}

/* Moves the pointer shape to a specified */
/* point relative to the display origin. */
int MoveCursor(HDISPLAY display, int x, int y)
{
	RETURN_IF_NULL(display);

	struct drm_mode_cursor req;
	memset(&req, 0, sizeof(req));
	req.flags = DRM_MODE_CURSOR_MOVE;
	req.crtc_id = display->crtc;
	req.x = (uint32_t)x;
	req.y = (uint32_t)y;
	if (ioctl(display->fd, DRM_IOCTL_MODE_CURSOR, &req)) {
		return GI_ERROR;
	}

	return GI_SUCCESS;
}

int HideCursor(HDISPLAY display)
{
	RETURN_IF_NULL(display);

	struct drm_mode_cursor req;
	memset(&req, 0, sizeof(req));
	req.flags = DRM_MODE_CURSOR_BO;
	req.crtc_id = display->crtc;
	req.width = 0;
	req.height = 0;
	req.handle = 0;
	if (ioctl(display->fd, DRM_IOCTL_MODE_CURSOR, &req)) {
		return GI_ERROR;
	}

	return GI_SUCCESS;
}

int ShowCursor(HDISPLAY display, HCURSOR cursor)
{
	RETURN_IF_NULL(display);
	RETURN_IF_NULL(cursor);

	struct IResource *res = NULL;
	if (!(res = (struct IResource*)cursor)) {
		return GI_ERROR;
	}

	struct drm_mode_cursor req;
	memset(&req, 0, sizeof(req));
	req.flags = DRM_MODE_CURSOR_BO;
	req.crtc_id = display->crtc;
	req.width = res->width;
	req.height = res->height;
	req.handle = res->handle;
	if (ioctl(display->fd, DRM_IOCTL_MODE_CURSOR, &req)) {
		return GI_ERROR;
	}

	return GI_SUCCESS;
}

/* Creates a new Cursor Object. */
HCURSOR CreateDefaultCursor(HDEVICE handle)
{
	HCURSOR cursor;
	if (!(cursor = CreateCursor(handle))) {
		return NULL;
	}

	void *data = NULL;
	if ((data = MapSurface((HSURFACE)cursor)) != NULL) {
		memcpy(data, DEFAULTCURSOR, (64*64*4));
	}

	return cursor;
}

