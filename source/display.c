#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "graphics.h"
#include "device.h"
#include "private.h"
#include "utils.h"

/* Retrieves the time in milliseconds */
static inline unsigned int GetTime(void)
{
	struct timespec time_sp;
	if (clock_gettime(CLOCK_MONOTONIC, &time_sp)) {
		return 0;
	}
	return (unsigned int)((time_sp.tv_sec * 1000UL) + (time_sp.tv_nsec / 1000000UL));
}

int GetDisplayCount(HDEVICE handle)
{
	struct IDevice *device;
	handle = GetDevice(handle);
	if (!(device = (struct IDevice*)handle)) {
		return -1;
	}

	struct drm_mode_card_res req;
	memset(&req, 0, sizeof(req));
	req.count_connectors = 0;
	if (ioctl(device->fd, DRM_IOCTL_MODE_GETRESOURCES, &req)) {
		return -1;
	}

	return (int)(req.count_connectors);
}

static int GetConnectorModes(HDISPLAY display)
{
	RETURN_IF_NULL(display);

	struct drm_mode_get_connector req;
	memset(&req, 0, sizeof(req));
	req.count_modes = MAX_MODES;
	req.modes_ptr = U64PTR(display->modes);
	req.count_props = MAX_PROPS;
	req.props_ptr = U64PTR(display->props);
	req.prop_values_ptr = U64PTR(display->props_values);
	req.connector_id = display->id;
	if (ioctl(display->fd, DRM_IOCTL_MODE_GETCONNECTOR, &req)) {
		return 0;
	}

	display->mod_count = req.count_modes;
	display->count_props = req.count_props;
	return 1;
}

static unsigned int CreateFramebuffer(HDISPLAY display, unsigned int bpp)
{
	RETURN_IF_NULL(display);

	struct drm_get_cap cap;
	memset(&cap, 0, sizeof(cap));
	cap.capability = DRM_CAP_DUMB_BUFFER;
	if (ioctl(display->fd, DRM_IOCTL_GET_CAP, &cap) || (!cap.value)) {
		return 0;
	}

	if (display->mod_count <= 0) {
		return 0;
	}

	struct drm_mode_create_dumb req1;
	memset(&req1, 0, sizeof(req1));
	req1.width = display->modes[0].hdisplay;
	req1.height = display->modes[0].vdisplay;
	req1.bpp = bpp;
	if (ioctl(display->fd, DRM_IOCTL_MODE_CREATE_DUMB, &req1)) {
		return 0;
	}

	struct drm_mode_fb_cmd req2;
	memset(&req2, 0, sizeof(req2));
	req2.width = req1.width;
	req2.height = req1.height;
	req2.pitch = req1.pitch;
	req2.handle = req1.handle;
	req2.depth = 24;
	req2.bpp = req1.bpp;
	if (ioctl(display->fd, DRM_IOCTL_MODE_ADDFB, &req2)) {
		return 0;
	}

	return (req2.fb_id);
}

/* Is encoder attached to connector? */
static int IsEncoderAttached(struct IDevice *device, unsigned int encoder)
{
	int index = 0;
	struct drm_mode_get_connector req;
	while (index < device->count_displays)
	{
		memset(&req, 0, sizeof(req));
		req.connector_id = device->connectors[index];
		if (!ioctl(device->fd, DRM_IOCTL_MODE_GETCONNECTOR, &req)) {
			if ((req.encoder_id == encoder))
				return 1;
		}
		index++;
	}
	return 0;
}

/* Is crtc attached to encoder? */
static int IsCrtcAttached(struct IDevice *device, unsigned int crtc)
{
	int index = 0;
	struct drm_mode_get_encoder req;
	while (index < device->count_encoders)
	{
		memset(&req, 0, sizeof(req));
		req.encoder_id = device->encoders[index];
		if (!ioctl(device->fd, DRM_IOCTL_MODE_GETENCODER, &req)) {
			if ((req.crtc_id == crtc) && IsEncoderAttached(device, req.encoder_id)) {
				return 1;
			}
		}
		index++;
	}
	return 0;
}

/* Is framebuffer attached to crtc? */
static int IsFramebufferAttached(struct IDevice *device, unsigned int fbo)
{
	int index = 0;
	struct drm_mode_crtc req;
	while (index < device->count_crtcs)
	{
		memset(&req, 0, sizeof(req));
		req.crtc_id = device->crtcs[index];
		if (!ioctl(device->fd,  DRM_IOCTL_MODE_GETCRTC, &req)) {
			if ((req.fb_id == fbo)) {
				return 1;
			}
		}
		index++;
	}
	return 0;
}

/* Gets a unattached crtc for first initialisation. */
static unsigned int GetUnattachedCrtc(struct IDevice *device)
{
	int index = 0;
	unsigned int crtc;
	while (index < device->count_crtcs)  {
		crtc = device->crtcs[index];
		if (!IsCrtcAttached(device, crtc)) {
			return crtc;
		}
		index++;
	}
	return 0;
}

/* Gets the current attached crtc. */
static unsigned int GetAttachedCrtc(HDISPLAY display)
{
	RETURN_IF_NULL(display);

	struct drm_mode_get_connector req1;
	memset(&req1, 0, sizeof(req1));
	req1.connector_id = display->id;
	if (ioctl(display->fd, DRM_IOCTL_MODE_GETCONNECTOR, &req1)) {
		return 0;
	}

	struct drm_mode_get_encoder req2;
	memset(&req2, 0, sizeof(req2));
	req2.encoder_id = req1.encoder_id;
	if (ioctl(display->fd, DRM_IOCTL_MODE_GETENCODER, &req2)) {
		return 0;
	}

	return (req2.crtc_id);
}

/* Graphics Infrastructure function. */
unsigned int GetCrtcID(HDISPLAY display)
{
	unsigned int crtc;
	RETURN_IF_NULL(display);
	if ((crtc = GetAttachedCrtc(display)) <= 0) {
		return GetUnattachedCrtc(display->device);
	}
	return crtc;
}

HDISPLAY OpenDisplay(HDEVICE handle, int index)
{
	struct IDevice *device;
	handle = GetDevice(handle);
	if (!(device = (struct IDevice*)handle)) {
		return NULL;
	}

	if ((index < 0) || (index >= device->count_displays)) {
		return NULL;
	}

	struct drm_mode_get_connector req;
	memset(&req, 0, sizeof(req));
	req.connector_id = device->connectors[index];
	if (ioctl(device->fd, DRM_IOCTL_MODE_GETCONNECTOR, &req))
	{
		/*return (HDISPLAY)0;*/
	}

	IDisplay *display = NULL;
	if (!(display = (IDisplay*)calloc(1, sizeof(IDisplay))))
	{
		return NULL;
	}

	display->fd = device->fd;
	display->id = req.connector_id;
	display->crtc = GetAttachedCrtc(display);
	display->count_props = req.count_props;
	display->device = device;
	GetConnectorModes(display);
	return (HDISPLAY)display;
}

HDISPLAY OpenDisplayFromID(HDEVICE handle, unsigned int id)
{
	struct IDevice *device;
	handle = GetDevice(handle);
	if (!(device = (struct IDevice*)handle))
	{
		return NULL;
	}

	struct drm_mode_get_connector connector = { 0 };
	connector.connector_id = (uint32_t)id;
	if (ioctl(device->fd, DRM_IOCTL_MODE_GETCONNECTOR, &connector))
	{
		/*return NULL;*/
	}

	struct drm_mode_get_encoder encoder = { 0 };
	encoder.encoder_id = connector.encoder_id;
	if (ioctl(device->fd, DRM_IOCTL_MODE_GETENCODER, &encoder))
	{
		/*return NULL;*/
	}

	IDisplay *display = NULL;
	if (!(display = (IDisplay*)calloc(1, sizeof(IDisplay))))
	{
		return NULL;
	}

	display->crtc = encoder.crtc_id;
	display->id = connector.connector_id;
	display->device = device;
	display->fd = device->fd;
	return (HDISPLAY)display;
}


/* Get the Surface handle for the Display. */
HSURFACE GetDisplaySurface(HDISPLAY display)
{
	RETURN_IF_NULL(display);

	struct drm_mode_crtc crtc;
	memset(&crtc, 0, sizeof(crtc));
	crtc.crtc_id = GetAttachedCrtc(display);
	if (ioctl(display->fd, DRM_IOCTL_MODE_GETCRTC, &crtc)) {
		return NULL;
	}

	struct drm_mode_fb_cmd fbo = { crtc.fb_id, 0, 0 };
	if (ioctl(display->fd, DRM_IOCTL_MODE_GETFB, &fbo)) {
		return NULL;
	}

	struct IResource *res = NULL;
	if (!(res = (struct IResource*)calloc(1, sizeof(struct IResource)))) {
		return NULL;
	}

	res->bpp = fbo.bpp;
	res->depth = fbo.depth;
	res->width = fbo.width;
	res->height = fbo.height;
	res->stride = fbo.pitch;
	res->size = (fbo.pitch * fbo.height);
	res->handle = fbo.handle;
	res->fd = display->fd;
	return (HSURFACE)res;
}

int WaitDisplayEvent(HDISPLAY display)
{
	RETURN_IF_NULL(display);
	struct drm_event_vblank ev;
	if (read(display->fd, &ev, sizeof(ev)) <= 0) {
		return -1;
	}

	printf("event: %d\n", ev.base.type);
	return 1;
}

int WaitVblank(HDISPLAY display)
{
	RETURN_IF_NULL(display);

	unsigned int start = GetTime();

	union drm_wait_vblank req;
	memset(&req, 0, sizeof(req));
	req.request.type = (_DRM_VBLANK_RELATIVE /*| _DRM_VBLANK_EVENT*/);
//	args.request.type |= ((display->crtc) << _DRM_VBLANK_HIGH_CRTC_SHIFT) &
//		_DRM_VBLANK_HIGH_CRTC_MASK;
	req.request.sequence = 0;
	req.request.signal = 0;
	if (ioctl(display->fd, DRM_IOCTL_WAIT_VBLANK, &req)) {
		printf("error:%d\n", errno);
		return 0;
	}

//	printf("seq: %d\n", req.reply.sequence);

	uint32_t msecs;
	msecs = (req.reply.tval_sec * 1000UL) + (req.reply.tval_usec / 1000UL);
//	printf("msec: %d\n", (msecs - start));
	return 1;
}

int GetDisplayFD(HDISPLAY handle)
{
	IDisplay *display;
	if (!(display = (IDisplay*)handle)) {
		return -1;
	}
	return (display->fd);
}

int GetDisplayInfo(HDISPLAY display, DISPLAYINFO *info)
{
	RETURN_IF_NULL(display);
	RETURN_IF_NULL(info);

	struct drm_mode_crtc crtc;
	memset(&crtc, 0, sizeof(crtc));
	crtc.crtc_id = GetAttachedCrtc(display);
	if (ioctl(display->fd, DRM_IOCTL_MODE_GETCRTC, &crtc)) {
		return 0;
	}

	info->id = display->id;
	info->crtc = crtc.crtc_id;
	info->width = crtc.mode.hdisplay;
	info->height = crtc.mode.vdisplay;
	info->gamma = crtc.gamma_size;
	info->fd = display->fd;
	return 1;
}

int GetDisplayBounds(HDISPLAY handle, RECT *bounds)
{
	RETURN_IF_NULL(handle);
	RETURN_IF_NULL(bounds);

	struct drm_mode_crtc crtc;
	memset(&crtc, 0, sizeof(crtc));
	crtc.crtc_id = handle->crtc;
	if (ioctl(handle->fd, DRM_IOCTL_MODE_GETCRTC, &crtc)) {
		return 0;
	}

	bounds->x = (int)crtc.x;
	bounds->y = (int)crtc.y;
	bounds->width = (int)crtc.mode.hdisplay;
	bounds->height = (int)crtc.mode.vdisplay;
	return 1;
}

/* Returns a Boolean value indicating */
/* whether a display is connected or online. */
int IsDisplayConnected(HDISPLAY handle)
{
	IDisplay *display;
	if (!(display = (IDisplay*)handle)) {
		return 0;
	}

	struct drm_mode_get_connector req;
	memset(&req, 0, sizeof(req));
	req.connector_id = display->id;
	if (ioctl(display->fd, DRM_IOCTL_MODE_GETCONNECTOR, &req)) {
		return 0;
	}

	return (req.connection == 1);
}

int IsDisplayAttached(HDISPLAY handle)
{
	return (GetAttachedCrtc(handle) > 0);
}

int AttachDisplay(HDISPLAY handle)
{
	RETURN_IF_NULL(handle);
	if (!IsDisplayConnected(handle)) {
		return 0;
	}

	if (IsDisplayAttached(handle)) {
		return 1;
	}

	if (handle->mod_count <= 0) {
		return 0;
	}

	unsigned int crtc = 0;
	if ((crtc = GetUnattachedCrtc(handle->device)) <= 0) {
		return 0;
	}

	unsigned int fbo = 0;
	if ((fbo = CreateFramebuffer(handle, 32)) <= 0) {
		return 0;
	}

	struct drm_mode_crtc req;
	memset(&req, 0, sizeof(req));
	req.set_connectors_ptr = U64PTR(&handle->id);
	req.count_connectors = 1;
	req.mode_valid = 1;
	req.crtc_id = crtc;
	req.fb_id = fbo;
	memcpy(&req.mode, &handle->modes[0], sizeof(struct drm_mode_modeinfo));
	if (ioctl(handle->fd, DRM_IOCTL_MODE_SETCRTC, &req)) {
		printf("err: %d\n", errno);
		return 0;
	}

	handle->crtc = crtc;
	return 1;
}

int DetachDisplay(HDISPLAY handle)
{
	RETURN_IF_NULL(handle);
	if (!IsDisplayAttached(handle)) {
		return 0;
	}

	struct drm_mode_crtc req;
	memset(&req, 0, sizeof(req));
	req.set_connectors_ptr = U64PTR(&handle->id);
	req.count_connectors = 1;
	req.mode_valid = 0;
	req.crtc_id = 0;

	return 1;
}

int GetGammaRange(HDISPLAY display, GAMMA_RANGE *data)
{
	RETURN_IF_NULL(display);
	RETURN_IF_NULL(data);

	struct drm_mode_crtc_lut req;
	memset(&req, 0, sizeof(req));
	req.crtc_id = GetAttachedCrtc(display);
	req.gamma_size = 256;
	req.red = U64PTR(data->red);
	req.green = U64PTR(data->green);
	req.blue = U64PTR(data->blue);
	if (ioctl(display->fd, DRM_IOCTL_MODE_GETGAMMA, &req)) {
		printf("error: %d\n", errno);
		return 0;
	}

	return 1;
}

int SetGammaRange(HDISPLAY display, GAMMA_RANGE *data)
{
	RETURN_IF_NULL(display);
	RETURN_IF_NULL(data);

	struct drm_mode_crtc_lut req;
	memset(&req, 0, sizeof(req));
	req.crtc_id = GetAttachedCrtc(display);
	req.gamma_size = 256;
	req.red = U64PTR(data->red);
	req.green = U64PTR(data->green);
	req.blue = U64PTR(data->blue);
	if (ioctl(display->fd, DRM_IOCTL_MODE_SETGAMMA, &req)) {
		printf("error: %d\n", errno);
		return 0;
	}

	return 1;
}

int FlushDisplay(HDISPLAY handle)
{
	RETURN_IF_NULL(handle);
	if (!IsDisplayAttached(handle)) {
		return 0;
	}

	struct drm_mode_fb_dirty_cmd req;
	memset(&req, 0, sizeof(req));

	return 1;
}

int DumpDisplay(HDISPLAY display)
{
	IDisplay *idisplay;
	if ((idisplay = (IDisplay*)display) == NULL)
	{
		return 0;
	}

	struct drm_mode_crtc crtc = { .crtc_id = idisplay->crtc };
	if (ioctl(idisplay->fd, DRM_IOCTL_MODE_GETCRTC, &crtc))
	{
		return 0;
	}

	printf("x: %d\n", crtc.x);
	printf("y: %d\n", crtc.y);
	printf("m: %d\n", crtc.mode_valid);
	printf("h: %d\n", crtc.mode.hdisplay);
	printf("v: %d\n", crtc.mode.vdisplay);

	return 0;
}

int DumpCRTC(HDEVICE handle)
{
	struct IDevice *device;
	if (!(device = (struct IDevice*)handle)) {
		return -1;
	}

	uint32_t crtcs[8];
	struct drm_mode_card_res res = { 0 };
	res.crtc_id_ptr = U64PTR(&crtcs);
	res.count_crtcs = 8;
	if (ioctl(device->fd, DRM_IOCTL_MODE_GETRESOURCES, &res)) {
		return -1;
	}

	int index;
	for (index = 0; index < res.count_crtcs; index++) {
		printf("crtc: %d\n", crtcs[index]);
	}

	return 0;
}

/* Release a display handle. */
int ReleaseDisplay(HDISPLAY display)
{
	IDisplay *idisplay;
	if (!(idisplay = (IDisplay*)display)) {
		return GI_ERROR;
	}

	free(idisplay);
	return GI_SUCCESS;
}

HDISPLAY GetMainDisplay(HDEVICE device)
{
	int index = 0;
	HDISPLAY display = NULL;
	while ((index < MAX_DISPLAYS)) {
		if (!(display = OpenDisplay(device, index))) {
			index++;
			continue;
		}

		if (IsDisplayConnected(display)) {
			break;
		}

		ReleaseDisplay(display);
		display = NULL;
		index++;
	}

	return display;
}


