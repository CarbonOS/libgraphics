#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <asm/fcntl.h>
#include "graphics.h"
#include "device.h"
#include "private.h"

/* Internal Buffer */
int CreateResource(struct IDevice *device, struct IResource *res, unsigned int width, unsigned int height, unsigned int bpp)
{
	assert((res != NULL));
	assert((device != NULL));
	assert((width > 0));
	assert((height > 0));
	assert((bpp > 0));

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

	res->fb = 0;
	res->fd = device->fd;
	res->bpp = args.bpp;
	res->device = device;
	res->height = args.height;
	res->width = args.width;
	res->stride = args.pitch;
	res->handle = args.handle;
	res->size = args.size;
	res->shared = -1;
	return GI_SUCCESS;
}

/* Internal Buffer */
int CreateSharedResource(struct IDevice *device, struct IResource *res, unsigned int width, unsigned int height, unsigned int bpp)
{
	if (CreateResource(device, res, width, height, bpp) != GI_SUCCESS) {
		return GI_ERROR;
	}

	struct drm_gem_flink req;
	memset(&req, 0, sizeof(req));
	req.handle = res->handle;
	if (ioctl(device->fd, DRM_IOCTL_GEM_FLINK, &req)) {
		return GI_ERROR;
	}

	res->shared = (req.name);
	return GI_SUCCESS;
}

/* Internal Buffer */
int CreateBufferFromHandle(struct IDevice *idevice, IBuffer **pbuffer, unsigned int width, unsigned int height, unsigned int bpp, unsigned int name)
{
	if (!(idevice) || !(pbuffer)) {
		return GI_ERROR;
	}

	struct drm_gem_open args;
	memset(&args, 0, sizeof(args));
	args.name = name;
	if ((ioctl(idevice->fd, DRM_IOCTL_GEM_OPEN, &args)) || (args.handle == 0))
	{
		//printf("errno: %d\n", errno);
		return GI_ERROR;
	}


	IBuffer *ibuffer = NULL;
	if (!(ibuffer = (IBuffer*)calloc(1, sizeof(IBuffer)))) {
		return GI_ERROR;
	}

	ibuffer->fb = 0;
	ibuffer->bpp = bpp;
	ibuffer->fd = idevice->fd;
	ibuffer->device = idevice;
	ibuffer->height = height;
	ibuffer->width = width;
	ibuffer->stride = (width * (bpp / 8));
	ibuffer->size = (ibuffer->height * ibuffer->stride);
	ibuffer->handle = args.handle;
	ibuffer->shared = name;
	(*pbuffer) = ibuffer;
	return GI_SUCCESS;
}

unsigned int GetResourceHandle(struct IResource *res)
{
	RETURN_IF_NULL(res);
	RETURN_IF_NULL(res->handle);

	struct drm_gem_flink req;
	memset(&req, 0, sizeof(req));
	req.handle = res->handle;
	if (ioctl(res->fd, DRM_IOCTL_GEM_FLINK, &req)) {
		return -1;
	}

	return (req.name);
}

int GetResourceBufferFD(struct IResource *res)
{
	RETURN_IF_NULL(res);
	RETURN_IF_NULL(res->handle);

	struct drm_prime_handle req;
	memset(&req, 0, sizeof(req));
	req.handle = res->handle;
	req.flags = (DRM_CLOEXEC);
	if (ioctl(res->fd, DRM_IOCTL_PRIME_HANDLE_TO_FD, &req)) {
		return -1;
	}

	return (req.fd);
}

unsigned int GetBufferHandle(IBuffer *ibuffer)
{
	if ((!ibuffer) || (!ibuffer->handle)) {
		return -1;
	}

	struct drm_gem_flink req;
	memset(&req, 0, sizeof(req));
	req.handle = ibuffer->handle;
	if (ioctl(ibuffer->fd, DRM_IOCTL_GEM_FLINK, &req)) {
		return -1;
	}

	return (req.name);
}

int MapBuffer(IBuffer *ibuffer, void **ptr)
{
	if ((!ibuffer) || (!ibuffer->handle)) {
		return GI_ERROR;
	}

	struct drm_mode_map_dumb req;
	memset(&req, 0, sizeof(req));
	req.handle = ibuffer->handle;
	if (ioctl(ibuffer->fd, DRM_IOCTL_MODE_MAP_DUMB, &req)) {
		return GI_ERROR;
	}

	void *data = NULL;
	if (!(data = mmap64(NULL, ibuffer->size, PROT_READ|PROT_WRITE, MAP_SHARED, ibuffer->fd, req.offset))) {
		return GI_ERROR;
	}

	(*ptr) = data;
	return GI_SUCCESS;
}

int UnmapBuffer(IBuffer *ibuffer, void *ptr)
{
	RETURN_IF_NULL(ibuffer);
	munmap(ptr, ibuffer->size);
	return GI_SUCCESS;
}
