#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "graphics.h"
#include "device.h"
#include "private.h"

int GetPlaneCount(HDEVICE handle)
{
	struct IDevice *device;
	if (!(device = (struct IDevice*)handle)) {
		return -1;
	}

	struct drm_mode_get_plane_res res;
	memset(&res, 0, sizeof(res));
	if (ioctl(device->fd, DRM_IOCTL_MODE_GETPLANERESOURCES, &res)) {
		return -1;
	}

	return ((int)res.count_planes);
}

static inline int GetPlaneID(HDEVICE handle, int index)
{
	struct IDevice *device;
	if (!(device = (struct IDevice*)handle)) {
		return -1;
	}

	uint32_t pids[8];
	struct drm_mode_get_plane_res res;
	memset(&res, 0, sizeof(res));
	res.plane_id_ptr = U64PTR(&pids);
	res.count_planes = 8;
	if (ioctl(device->fd, DRM_IOCTL_MODE_GETPLANERESOURCES, &res)) {
		return -1;
	}

	/* Array bounds check. */
	if ((index < 0) || (index >= res.count_planes)) {
		return -1;
	}

	return ((int)pids[index]);
}
