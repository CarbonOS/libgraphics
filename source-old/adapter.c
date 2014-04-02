#include <stdio.h>
#include <stdlib.h>
#include <system/core.h>
#include <system/io.h>
#include "graphics.h"
#include "device.h"
#include "private.h"

/* Locals */
static int adapter_count = 0;
static IAdapter* adapters[MAX_ADAPTERS];

/*
static int OpenDevice(int index)
{
	char filename[PATH_MAX];
	snprintf(filename, PATH_MAX, ADAPTER_DEVICES, index);
	return OpenFile(filename, O_RDWR, 0);
}
*/
/* Get the first installed adapter */
int DefaultAdapter(AdapterPtr *ppAdapter)
{
	if (EnumAdapters(0, ppAdapter) != GI_SUCCESS)
	{
		return GI_ERROR;
	}

	return GI_SUCCESS;
}

/* Enumerates the installed adapters (video cards). */
int EnumAdapters(int index, AdapterPtr *ppAdapter)
{
	int handle = -1;
	char filename[PATH_MAX];

	snprintf(filename, PATH_MAX, ADAPTER_DEVICES, index);
	if ((handle = OpenFile(filename, O_RDWR, 0)) < 0)
	{
		return GI_ERROR;
	}

	IAdapter *iadapter;
	if ((iadapter = (IAdapter*)calloc(1, sizeof(IAdapter))) == NULL)
	{
		close(handle);
		return GI_ERROR;
	}

	iadapter->fd = handle;
	iadapter->CreateDeviceContext = NULL;
	iadapter->GetPipeline = spGetPipeline;
	LoadDriver(iadapter);
	(*ppAdapter) = iadapter;

	return GI_SUCCESS;
}


