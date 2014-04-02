#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <drm/drm.h>
#include <drm/drm_mode.h>
#include <system/io.h>
#include "graphics.h"
#include "device.h"
#include "drm_private.h"

/* Include Static Functions */
#include "surface.c"
#include "cursor.c"

static uint32_t drmGetConnectorID(IAdapter *pAdapter, int index)
{
	uint32_t cids[8];
	struct drm_mode_card_res res = { 0 };
	res.count_connectors = 8;
	res.connector_id_ptr = U64PTR(&cids);

	if (IoControl(pAdapter->handle, DRM_IOCTL_MODE_GETRESOURCES, &res))
	{
		return 0;
	}

	if ((index < 0) || (index >= res.count_connectors))
	{
		return 0;
	}

	return cids[index];
}

static uint32_t drmGetConnectorCRTCLink(IAdapter *pAdapter, uint32_t connector_id)
{
	struct drm_mode_crtc crtc = { 0 };
	struct drm_mode_get_connector conn = { 0 };
	struct drm_mode_get_encoder enc = { 0 };

	conn.connector_id = connector_id;
	if (IoControl(pAdapter->handle, DRM_IOCTL_MODE_GETCONNECTOR, &conn))
	{
		return 0;
	}

	enc.encoder_id = conn.encoder_id;
	if (IoControl(pAdapter->handle, DRM_IOCTL_MODE_GETENCODER, &enc))
	{
		return 0;
	}

	return enc.crtc_id;
}

/* Retrieve the adapter display information. (DRM Version) */
static int drmGetDisplayInfo(IDisplay *pDisplay, DisplayInfo *info)
{
	struct drm_mode_crtc crtc = { 0 };
	struct drm_mode_fb_cmd fbo = { 0 };

	DRMDisplay *display;
	if ((display = (DRMDisplay*)pDisplay) == NULL)
	{
		return GI_ERROR;
	}

	crtc.crtc_id = display->crtc_id;
	if (IoControl(display->handle, DRM_IOCTL_MODE_GETCRTC, &crtc))
	{
		/*return GI_ERROR*/;
	}

	fbo.fb_id = crtc.fb_id;
	if (IoControl(display->handle, DRM_IOCTL_MODE_GETFB, &fbo))
	{
		/*return GI_ERROR*/;
	}

//	flink.handle = fbo.handle;
//	if (drmIoctl(ndsp->handle, DRM_IOCTL_GEM_FLINK, &flink))
//	{
//		/*return GI_ERROR*/;
//	}

	info->x = (int)crtc.x;
	info->y = (int)crtc.y;
	info->width = (int)crtc.mode.hdisplay;
	info->height = (int)crtc.mode.vdisplay;
	info->pitch = (int)fbo.pitch;
	info->depth = (int)fbo.depth;
	info->bpp = (int)fbo.bpp;
	info->id = fbo.handle;

	return GI_SUCCESS;
}

static int drmMoveCursor(IDisplay *pDisplay, int x, int y)
{
	DRMDisplay *display = NULL;
	if ((display = (DRMDisplay*)pDisplay) == NULL)
	{
		return GI_ERROR;
	}

	struct drm_mode_cursor arg;
	arg.flags = DRM_MODE_CURSOR_MOVE;
	arg.crtc_id = display->crtc_id;
	arg.x = x;
	arg.y = y;

	if (IoControl(display->handle, DRM_IOCTL_MODE_CURSOR, &arg))
	{
		return GI_ERROR;
	}

	return GI_SUCCESS;
}

/* Halt a thread until the next vertical blank occurs. */
static int drmWaitForVBlank(IDisplay *pDisplay)
{
	DRMDisplay *display = NULL;
	if ((display = (DRMDisplay*)pDisplay) == NULL)
	{
		return GI_ERROR;
	}
/*
	drmVBlank vbl;
	vbl.request.type = DRM_VBLANK_RELATIVE;
	vbl.request.sequence = 0;
	vbl.request.signal = 0;

	if (drmWaitVBlank(display->handle, &vbl))
	{
		return GI_ERROR;
	}
*/
	return GI_SUCCESS;
}

static int drmGetDisplayAdapter(IDisplay *pDisplay, IAdapter **ppAdapter)
{
	DRMDisplay *display = NULL;
	if ((display = (DRMDisplay*)pDisplay) == NULL)
	{
		return GI_ERROR;
	}

	if ((ppAdapter == NULL) || (display->adapter == NULL))
	{
		return GI_ERROR;
	}

	(*ppAdapter) = display->adapter;
	return GI_SUCCESS;
}

/* Enumerate all Properties supported by an connected display. */
static int drmEnumProperties(IDisplay *pDisplay, int index)
{
	DRMDisplay *display = NULL;
	if ((display = (DRMDisplay*)pDisplay) == NULL)
	{
		return GI_ERROR;
	}

	struct drm_mode_get_connector conn = { 0 };
	conn.connector_id = display->connector_id;
	if (IoControl(display->handle, DRM_IOCTL_MODE_GETCONNECTOR, &conn))
	{
		return GI_ERROR;
	}

	printf("prop: %d\n", conn.count_props);
	return GI_SUCCESS;
}

/* Release the Display object. */
static int drmReleaseDisplay(IDisplay *pDisplay)
{
	DRMDisplay *display = NULL;
	if ((display = (DRMDisplay*)pDisplay) == NULL)
	{
		return GI_ERROR;
	}

	free(display);
	return GI_SUCCESS;
}

/* Enumerate adapter (video card) outputs (displays). */
static int drmEnumDisplays(IAdapter *pAdapter, int index, IDisplay **ppDisplay)
{
	if ((pAdapter == NULL) || (ppDisplay == NULL))
	{
		return GI_ERROR;
	}

	uint32_t connector_id;
	if (!(connector_id = drmGetConnectorID(pAdapter, index)))
	{
		return GI_ERROR;
	}

	uint32_t crtc_link;
	if (!(crtc_link = drmGetConnectorCRTCLink(pAdapter, connector_id)))
	{
	}

	DRMDisplay *display;
	if ((display = (DRMDisplay*)calloc(1, sizeof(DRMDisplay))) == NULL)
	{
		return GI_ERROR;
	}

	display->adapter = pAdapter;
	display->handle = pAdapter->handle;
	display->base.GetDisplayInfo = drmGetDisplayInfo;
	display->base.GetAdapter = drmGetDisplayAdapter;
	display->base.EnumProperties = drmEnumProperties;
	display->base.MoveCursor = drmMoveCursor;
	display->connector_id = connector_id;
	display->crtc_id = crtc_link;
	(*ppDisplay) = &display->base;

	return GI_SUCCESS;
}

/* */
int drmAdapterInit(IAdapter *pAdapter, int handle)
{
	if ((pAdapter == NULL) || (handle < 0))
	{
		return GI_ERROR;
	}

	struct drm_version version = { 0 };
	if (IoControl(handle, DRM_IOCTL_VERSION, &version))
	{
		return GI_ERROR;
	}

	printf("drm: %d.%d\n", version.version_major, version.version_minor);
	pAdapter->EnumDisplays = drmEnumDisplays;
	return GI_SUCCESS;
}

