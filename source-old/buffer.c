#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <drm/drm.h>
#include <drm/drm_mode.h>
#include <system/io.h>
#include <sys/mman.h>
#include "graphics.h"
#include "device.h"
#include "private.h"

int CreateBuffer(IAdapter *iadapter, IBuffer **ppbuffer, uint32_t width, uint32_t height, uint32_t bpp)
{
	if ((iadapter == NULL))
	{
		return GI_ERROR;
	}

	struct drm_get_cap cap = { DRM_CAP_DUMB_BUFFER, 0 };
	if (IoControl(iadapter->fd, DRM_IOCTL_GET_CAP, &cap) || (!cap.value))
	{
		return GI_ERROR;
	}

	struct drm_mode_create_dumb args = { .width = width, .height = height, .bpp = bpp };
	if (IoControl(iadapter->fd, DRM_IOCTL_MODE_CREATE_DUMB, &args))
	{
		return GI_ERROR;
	}

	IBuffer *ibuffer = NULL;
	if ((ibuffer = (IBuffer*)calloc(1, sizeof(IBuffer))) == NULL)
	{
		return GI_ERROR;
	}

	ibuffer->fd = iadapter->fd;
	ibuffer->height = args.width;
	ibuffer->width = args.height;
	ibuffer->stride = args.pitch;
	ibuffer->handle = args.handle;
	ibuffer->size = args.size;
	ibuffer->shared = -1;
	(*ppbuffer) = ibuffer;

	return GI_SUCCESS;
}

int CreateBufferFromHandle(IAdapter *iadapter, IBuffer **ppbuffer, uint32_t width, uint32_t height, uint32_t stride, uint32_t handle)
{
	if ((iadapter == NULL) || (iadapter->fd < 0))
	{
		return GI_ERROR;
	}

	IBuffer *ibuffer = NULL;
	if ((ibuffer = (IBuffer*)calloc(1, sizeof(IBuffer))) == NULL)
	{
		return GI_ERROR;
	}

	ibuffer->fd = iadapter->fd;
	ibuffer->height = width;
	ibuffer->width = height;
	ibuffer->stride = stride;
	ibuffer->handle = handle;
	ibuffer->size = (stride * height);
	(*ppbuffer) = ibuffer;

	return GI_SUCCESS;
}

/*
Open accesses an existing object and returns a handle for it. If the
object doesn't exist, -ENOENT is returned. The size of the object is
also returned. This handle has all the same capabilities as the
handle used to create the object. In particular, the object is not
destroyed until all handles are closed.
*/
int OpenBufferFromName(IAdapter *iadapter, IBuffer **ppbuffer, uint32_t width, uint32_t height, uint32_t name)
{
	if ((iadapter == NULL) || (iadapter->fd < 0))
	{
		return GI_ERROR;
	}

	struct drm_gem_open req = { .name = name };
	if (IoControl(iadapter->fd, DRM_IOCTL_GEM_OPEN, &req))
	{
		return GI_ERROR;
	}

	IBuffer *ibuffer = NULL;
	if ((ibuffer = (IBuffer*)calloc(1, sizeof(IBuffer))) == NULL)
	{
		return GI_ERROR;
	}

	ibuffer->fd = iadapter->fd;
	ibuffer->height = width;
	ibuffer->width = height;
	ibuffer->stride = (req.size / height);
	ibuffer->handle = (req.handle);
	ibuffer->size = req.size;
	(*ppbuffer) = ibuffer;

	return GI_SUCCESS;
}

int AddFramebuffer(IBuffer *ibuffer)
{
	struct drm_mode_fb_cmd args;
	args.width  = ibuffer->width;
	args.height = ibuffer->height;
	args.pitch  = ibuffer->stride;
	args.bpp    = 32;
	args.depth  = 1;
	args.handle = ibuffer->handle;

	if (IoControl(ibuffer->fd, DRM_IOCTL_MODE_ADDFB, &args))
	{
		return GI_ERROR;
	}

	return GI_SUCCESS;
}

/*
Flink creates a name for the object and returns it to the
application. This name can be used by other applications to gain
access to the same object.
*/
int GetBufferName(IBuffer *ibuffer, uint32_t *name)
{
	if ((ibuffer == NULL) || (!ibuffer->handle))
	{
		return GI_ERROR;
	}

	struct drm_gem_flink req = { .handle = ibuffer->handle };
	if (IoControl(ibuffer->fd, DRM_IOCTL_GEM_FLINK, &req))
	{
		return GI_ERROR;
	}

	(*name) = req.name;
	return GI_SUCCESS;
}

int GetSharedBufferFD(IBuffer *ibuffer, int *handle)
{
	if ((ibuffer == NULL) || (!ibuffer->handle))
	{
		return GI_ERROR;
	}

	struct drm_prime_handle args =
	{
		.handle = ibuffer->handle,
		.flags = 0//O_CLOEXEC
	};

	if (IoControl(ibuffer->fd, DRM_IOCTL_PRIME_HANDLE_TO_FD, &args))
	{
		return GI_ERROR;
	}

	ibuffer->shared = args.fd;
	(*handle) = args.fd;
	return GI_SUCCESS;
}

int MapBuffer(IBuffer *ibuffer, void **ptr)
{
	if ((ibuffer == NULL) || (!ibuffer->handle))
	{
		return GI_ERROR;
	}

	struct drm_mode_map_dumb req = { .handle = ibuffer->handle };
	if (IoControl(ibuffer->fd, DRM_IOCTL_MODE_MAP_DUMB, &req))
	{
		printf("IOCTL_MODE_MAP_DUMB::failed!\n");
		return GI_ERROR;
	}

	void *data = NULL;
	if ((data = MapMemory64(ibuffer->size, PROT_READ|PROT_WRITE, MAP_SHARED, ibuffer->fd, req.offset)) == MAP_FAILED)
	{
		printf("MapMemory64::failed!\n");
		return GI_ERROR;
	}

	(*ptr) = data;
	return GI_SUCCESS;
}

int UnmapBuffer(IBuffer *ibuffer)
{
/*
	if ((ibuffer == NULL) || (ibuffer->map == NULL))
	{
		return GI_ERROR;
	}
*/
//	munmap(ibuffer->map, ibuffer->size);
//	ibuffer->map = NULL;
	return GI_SUCCESS;
}


