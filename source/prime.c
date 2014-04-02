#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <asm/fcntl.h>
#include "graphics.h"
#include "device.h"
#include "private.h"

int GetFDFromBuffer(struct IDevice *device, unsigned int handle)
{
	RETURN_IF_NULL(device);
	printf("GetFDFromBuffer...\n");
	struct drm_prime_handle req;
	memset(&req, 0, sizeof(req));
	req.handle = handle;
	req.flags = (DRM_CLOEXEC);
	if (ioctl(device->fd, DRM_IOCTL_PRIME_HANDLE_TO_FD, &req)) {
		return -1;
	}

	return (req.fd);
}

int GetBufferFromFD(struct IDevice *device, int fd)
{
	struct drm_prime_handle req;
	memset(&req, 0, sizeof(req));
	req.fd = fd;
	if (ioctl(device->fd, DRM_IOCTL_PRIME_FD_TO_HANDLE, &req)) {
		return -1;
	}

	return (req.handle);
}
