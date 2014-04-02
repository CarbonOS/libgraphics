#include <stdio.h>
#include "graphics.h"
#include "device.h"
#include "private.h"

/* Release a graphics context. */
int ReleaseDevice(DevicePtr pDevice)
{
	IDevice *idevice;
	if ((idevice = (IDevice*)pDevice) == NULL)
	{
		return GI_ERROR;
	}

	return GI_SUCCESS;
}

/* Creates a graphics device. */
/*
int CreateDevice(AdapterPtr pAdapter, DevicePtr *ppDevice)
{
	IAdapter *iadapter;
	if ((iadapter = (IAdapter*)pAdapter) == NULL)
	{
		return GI_ERROR;
	}

	return GI_SUCCESS;
}
*/
