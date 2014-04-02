#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <drm/drm.h>
#include <drm/drm_mode.h>
#include <system/io.h>
#include "graphics.h"
#include "device.h"
#include "private.h"

static uint32_t drmGetConnectorID(IAdapter *iadapter, int index)
{
	uint32_t cids[8];
	struct drm_mode_card_res res = { 0 };
	res.connector_id_ptr = U64PTR(&cids);
	res.count_connectors = 8;

	if (IoControl(iadapter->fd, DRM_IOCTL_MODE_GETRESOURCES, &res))
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
	struct drm_mode_get_connector conn = { .connector_id = connector_id };
	if (IoControl(pAdapter->fd, DRM_IOCTL_MODE_GETCONNECTOR, &conn))
	{
		return 0;
	}

	struct drm_mode_get_encoder enc = { .encoder_id = conn.encoder_id };
	if (IoControl(pAdapter->fd, DRM_IOCTL_MODE_GETENCODER, &enc))
	{
		return 0;
	}

	return enc.crtc_id;
}

/* Enumerate adapter (video card) outputs (displays). */
int GetDisplayInfo(DisplayPtr pDisplay, DisplayInfo *pInfo)
{
	IDisplay *idisplay;
	if ((idisplay = (IDisplay*)pDisplay) == NULL)
	{
		return GI_ERROR;
	}

	struct drm_mode_crtc crtc = { 0, 0, idisplay->crtc };
	if (IoControl(idisplay->fd, DRM_IOCTL_MODE_GETCRTC, &crtc))
	{
		/*return GI_ERROR*/;
	}

	struct drm_mode_fb_cmd fbo = { crtc.fb_id, 0, 0 };
	if (IoControl(idisplay->fd, DRM_IOCTL_MODE_GETFB, &fbo))
	{
		/*return GI_ERROR*/;
	}

	pInfo->x = (int)crtc.x;
	pInfo->y = (int)crtc.y;
	pInfo->width = (int)crtc.mode.hdisplay;
	pInfo->height = (int)crtc.mode.vdisplay;
	pInfo->pitch = (int)fbo.pitch;
	pInfo->depth = (int)fbo.depth;
	pInfo->bpp = (int)fbo.bpp;
	pInfo->id = fbo.handle;

	return GI_SUCCESS;
}

/* Get the Surface object for the Display. */
int GetDisplaySurface(DisplayPtr pDisplay, SurfacePtr *ppSurface)
{
	IDisplay *idisplay;
	if ((idisplay = (IDisplay*)pDisplay) == NULL)
	{
		return GI_ERROR;
	}

	struct drm_mode_crtc crtc = { 0, 0, idisplay->crtc };
	if (IoControl(idisplay->fd, DRM_IOCTL_MODE_GETCRTC, &crtc))
	{
		return GI_ERROR;
	}

	struct drm_mode_fb_cmd fbo = { crtc.fb_id, 0, 0 };
	if (IoControl(idisplay->fd, DRM_IOCTL_MODE_GETFB, &fbo))
	{
		return GI_ERROR;
	}

	IBuffer *ibuffer = NULL;
	if ((ibuffer = (IBuffer*)calloc(1, sizeof(IBuffer))) == NULL)
	{
		return GI_ERROR;
	}

	ibuffer->fd = idisplay->fd;
	ibuffer->height = fbo.width;
	ibuffer->width = fbo.height;
	ibuffer->stride = fbo.pitch;
	ibuffer->size = (fbo.pitch * fbo.height);
	ibuffer->handle = fbo.handle;
	(*ppSurface) = ibuffer;

	return GI_SUCCESS;
}


/* Enumerate all Properties supported by an connected display. */
int EnumProperties(DisplayPtr pDisplay, int index)
{
	IDisplay *idisplay;
	if ((idisplay = (IDisplay*)pDisplay) == NULL)
	{
		return GI_ERROR;
	}

	return GI_UNIMPLEMENTED;
}

int IsDisplayConnected(DisplayPtr pDisplay)
{
	IDisplay *idisplay;
	if ((idisplay = (IDisplay*)pDisplay) == NULL)
	{
		return 0;
	}

	struct drm_mode_get_connector conn = { .connector_id = idisplay->connector };
	if (IoControl(idisplay->fd, DRM_IOCTL_MODE_GETCONNECTOR, &conn))
	{
		return 0;
	}

	return (conn.connection == 1);
}

/* Release the Display object. */
int ReleaseDisplay(DisplayPtr pDisplay)
{
	IDisplay *idisplay;
	if ((idisplay = (IDisplay*)pDisplay) == NULL)
	{
		return GI_ERROR;
	}

	free(idisplay);
	return GI_SUCCESS;
}

/* Enumerate adapter (video card) outputs (displays). */
int EnumDisplays(AdapterPtr pAdapter, int index, DisplayPtr *ppDisplay)
{
	IAdapter *iadapter;
	if (((iadapter = (IAdapter*)pAdapter) == NULL) || (ppDisplay == NULL))
	{
		return GI_ERROR;
	}

	uint32_t connector_id;
	if (!(connector_id = drmGetConnectorID(iadapter, index)))
	{
		return GI_ERROR;
	}

	uint32_t crtc_link;
	if (!(crtc_link = drmGetConnectorCRTCLink(iadapter, connector_id)))
	{
	}

	IDisplay *idisplay;
	if ((idisplay = (IDisplay*)calloc(1, sizeof(IDisplay))) == NULL)
	{
		return GI_ERROR;
	}

	idisplay->crtc = crtc_link;
	idisplay->connector = connector_id;
	idisplay->adapter = iadapter;
	idisplay->fd = iadapter->fd;
	(*ppDisplay) = idisplay;

	return GI_SUCCESS;
}

/* Get the first connected display object. */
int DefaultDisplay(AdapterPtr pAdapter, DisplayPtr *ppDisplay)
{
	int index = 0;
	DisplayPtr display;

	if ((pAdapter == NULL) || (ppDisplay == NULL))
	{
		return GI_ERROR;
	}

	while (EnumDisplays(pAdapter, index, &display) == GI_SUCCESS)
	{
		if (IsDisplayConnected(display))
		{
			(*ppDisplay) = display;
			return GI_SUCCESS;
		}

		ReleaseDisplay(display);
		index++;
	}

	return GI_ERROR;
}



