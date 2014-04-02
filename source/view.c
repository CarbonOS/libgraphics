#include <stdio.h>
#include <stdlib.h>
#include <system/task.h>
#include <linux/winsys.h>
#include "graphics.h"
#include "device.h"
#include "private.h"
#include "utils.h"
#include "view.h"

int RegisterCompositor(HDEVICE handle)
{
	RETURN_IF_NULL(handle);

	struct wm_args args;
	memset(&args, 0, sizeof(args));
	args.param1 = handle->fd;
	return Viewcall(-1, WINCMD_REGISTERMANAGER, &args);
}

HVIEW CreateView(HDEVICE handle, int type, int width, int height)
{
	int viewid = 0;
	struct wm_args args;
	args.param1 = type;
	args.param2 = width;
	args.param3 = height;
	if ((viewid = Viewcall(-1, WINCMD_CREATEWIN, &args)) <= 0) {
		return NULL;
	}

	printf("view: %d\n", viewid);
	return (HVIEW)viewid;
}

int MoveView(HVIEW view, int x, int y)
{
	RETURN_IF_NULL(view);

	struct wm_args args;
	args.param1 = (int)x;
	args.param2 = (int)y;
	if (Viewcall((int)view, WINCMD_MOVEWIN, &args)) {
		return 0;
	}
	return 1;
}

int ResizeView(HVIEW view, int width, int height)
{
	RETURN_IF_NULL(view);

	struct wm_args args;
	args.param1 = (int)width;
	args.param2 = (int)height;
	if (Viewcall((int)view, WINCMD_MOVEWIN, &args)) {
		return 0;
	}
	return 1;
}

int SetViewCursor(HVIEW view, HCURSOR cursor)
{
	RETURN_IF_NULL(view);
	RETURN_IF_NULL(cursor);

	struct IResource *res = NULL;
	if (!(res = (struct IResource*)cursor)) {
		return GI_ERROR;
	}

	struct wm_args args;
	args.param1 = (int)GetResourceHandle(res);
	if (Viewcall((int)view, WINCMD_CREATEWIN, &args)) {
		return 0;
	}
	return 1;
}

int SetViewSurface(HVIEW view, HSURFACE surface)
{
	RETURN_IF_NULL(view);
	RETURN_IF_NULL(surface);

	struct IResource *res = NULL;
	if (!(res = (struct IResource*)surface)) {
		return GI_ERROR;
	}

	struct wm_args args;
	args.param1 = (int)res->width;
	args.param2 = (int)res->height;
	args.param3 = (int)res->stride;
	args.param4 = (int)GetResourceHandle(res);
	args.param5 = (int)res->bpp;
	if (Viewcall((int)view, WINCMD_SETSURFACE, &args)) {
		return 0;
	}

	return 1;
}

HSURFACE GetViewSurface(HDEVICE device, HVIEW view)
{
	int handle;
	RETURN_IF_NULL(view);
	RETURN_IF_NULL(device);

	SURFACEINFO info;
	struct wm_args args;
	args.param1 = (int)(void*)&info;
	if (Viewcall((int)view, WINCMD_GETSURFACE, &args)) {
		return NULL;
	}

	printf("width: %d\n", info.width);
	printf("height: %d\n", info.height);
	printf("handle: %d\n", info.handle);
	printf("stride: %d\n", info.pitch);
	printf("depth: %d\n", info.depth);
	printf("bpp: %d\n", info.bpp);
	return (HSURFACE)NULL;
}

int GetViewSurfaceInfo(HVIEW view, SURFACEINFO *info)
{
	RETURN_IF_NULL(view);
	RETURN_IF_NULL(info);

	struct wm_args args;
	args.param1 = (int)(void*)info;
	if (Viewcall((int)view, WINCMD_GETSURFACE, &args)) {
		return 0;
	}

	return 1;
}

int GetViewBounds(HVIEW view, RECT *bounds)
{
	RETURN_IF_NULL(view);
	RETURN_IF_NULL(bounds);

	struct wm_args args;
	args.param1 = (int)(void*)bounds;
	if (Viewcall((int)view, WINCMD_GETBOUNDS, &args)) {
		return 0;
	}

	return 1;
}

int SendViewMessage(HVIEW view, const TASKMSG *msg)
{
	RETURN_IF_NULL(view);
	RETURN_IF_NULL(msg);

	struct wm_args args;
	args.param1 = (int)(void*)msg;
	if (Viewcall((int)view, WINCMD_SNDMESSAGE, &args)) {
		return 0;
	}

	return 1;
}

int DestroyView(HVIEW view)
{
	RETURN_IF_NULL(view);

	return 1;
}

/* Returns the default compositor device. */
HDEVICE GetDefaultDevice(void)
{
	int fd = -1;
	struct wm_args args;
	memset(&args, 0, sizeof(args));
	if ((fd = Viewcall(-1, WINCMD_GETDEVICE, &args)) < 0) {
		return NULL;
	}
	return OpenDeviceFromFD(fd);
}


