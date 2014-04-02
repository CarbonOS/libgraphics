#include <stdio.h>
#include <errno.h>
#include "graphics.h"
#include "device.h"
#include "private.h"
#include "utils.h"
#include "image.c"

/* Creates a new surface/buffer object. */
HSURFACE CreateSurface(HDEVICE handle, unsigned width, unsigned height, unsigned bpp)
{
	struct IDevice *device;
	device = GetDevice(handle);
	RETURN_IF_NULL(device);

	struct IResource *res = NULL;
	if (!(res = (struct IResource*)calloc(1, sizeof(struct IResource)))) {
		return NULL;
	}

	if (CreateResource(device, res, width, height, bpp) != GI_SUCCESS) {
		free(res);	
		return NULL;
	}

	return (HSURFACE)res;
}

/* Creates a new surface/buffer object from image file. */
HSURFACE CreateSurfaceFromFile(HDEVICE device, const char *filename)
{
	int width;
	int height;
	int components;
	struct IDevice *idevice;
	unsigned char *data;
	device = GetDevice(device);
	if (!(idevice = (struct IDevice*)device) || (!filename))
	{
		printf("Invalid Argument!\n");
		return NULL;
	}

	/* Load Image from Filename. */
	data = stbi_load(filename, &width, &height, &components, 0);
	if ((data == NULL))
	{
		printf("Invalid Image File!\n");
		return NULL;
	}

	IBuffer *ibuffer = NULL;
//	if (CreateBuffer(idevice, &ibuffer, width, height, (components * 8)) != GI_SUCCESS)
//	{
//		printf("CreateBuffer Failed!\n");
//		stbi_image_free(data);
//		return NULL;
//	}

//	CopyDataToSurface(ibuffer, data, ibuffer->size);
	stbi_image_free(data);
	return (HSURFACE)ibuffer;
}

unsigned int GetSharedHandle(HSURFACE surface)
{
	IBuffer *ibuffer = NULL;
	if (!(ibuffer = (IBuffer*)surface)) {
		return -1;
	}

	unsigned int handle = 0;
	if ((handle = GetBufferHandle(ibuffer)) <= 0)
	{
		printf("Invalid Buffer handle!\n");
		return -1;
	}

	return handle;
}

int GetSurfaceInfo(HSURFACE surface, SURFACEINFO *info)
{
	struct IResource *res = NULL;
	if (!(res = (struct IResource*)surface) || !info) {
		return GI_ERROR;
	}
	info->width = res->width;
	info->height = res->height;
	info->pitch = res->stride;
	info->handle = res->handle;
	info->depth = res->depth;
	info->bpp = res->bpp;
	return GI_SUCCESS;
}

/* Get a pointer to the data contained in the surface/buffer. */
void* MapSurface(HSURFACE surface)
{
	IBuffer *ibuffer = NULL;
	if (!(ibuffer = (IBuffer*)surface))
	{
		printf("Buffer::invalid\n");
		return NULL;
	}

	void *data = NULL;
	if (MapBuffer(ibuffer, &data) != GI_SUCCESS)
	{
		printf("MapBuffer::failed\n");
		return NULL;
	}

	return data;
}



