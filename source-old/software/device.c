#include <stdio.h>
#include <graphics.h>
#include <device.h>
#include "framebuffer.h"

/* Summary */
static int fbReleaseDevice(IDevice *pDevice)
{
	FBDevice *fbd = NULL;
	if ((fbd = (FBDevice*)pDevice) == NULL)
		return GI_ERROR;

	free(fbd);
	fbd = NULL;
	return GI_SUCCESS;
}

/* Summary */
int fbCreateGraphicsDevice(IAdapter *pAdapter, IDevice **ppDevice)
{
	FBAdapter *fba = NULL;
	if ((fba = (FBAdapter*)pAdapter) == NULL)
		return GI_ERROR;

	if (ppDevice == NULL)
		return GI_ERROR;

	FBDevice *device = NULL;
	if ((device = (FBDevice*)calloc(1, sizeof(FBDevice))) == NULL)
	{
		return GI_ERROR;
	}

	device->adapter = fba;
	device->handle = fba->handle;
	device->base.CreateSurface = NULL;
	device->base.CreateSurfaceFromHandle = NULL;
	device->base.CreateContext = fbCreateContext;
	device->base.Release = fbReleaseDevice;
	(*ppDevice) = &device->base;

	return GI_SUCCESS;
}

