#include <stdio.h>
#include <stdint.h>
#include <drm/drm.h>
#include <drm/drm_mode.h>
#include "graphics.h"
#include "device.h"
#include "private.h"

/* Include Static Functions */
#include "state.c"
#include "shaders.c"
#include "surface.c"
#include "display.c"
#include "drawing.c"
#include "buffer.c"
#include "device.c"

/* Creates a new Gallium based graphics context. */ 
//static int galCreateDeviceContext(IAdapter *pAdapter, IDevice **ppDevice)
int CreateDevice(AdapterPtr pAdapter, DevicePtr *ppDevice)
{
	IAdapter *iadapter;
	if ((iadapter = (IAdapter*)pAdapter) == NULL)
	{
		return GI_ERROR;
	}

	if ((iadapter->GetPipeline == NULL))
	{
		printf("GetPipeline::Unimplemented!\n");
		return GI_UNIMPLEMENTED;
	}

	struct pipe_screen *screen = NULL;
	if (iadapter->GetPipeline(iadapter, (void**)&screen) != GI_SUCCESS)
	{
		printf("CreatePipeline::failed!\n");
		return GI_ERROR;
	}

	struct pipe_context *context = NULL;
	if ((context = ICreateContext(screen, (void*)pAdapter)) == NULL)
	{
		printf("context_create::Failed!\n");
		return GI_ERROR;
	}

	GIDevice *idevice = NULL;
	if ((idevice = (GIDevice*)calloc(1, sizeof(GIDevice))) == NULL)
	{
		return GI_ERROR;
	}

	idevice->screen = screen;
	idevice->context = context;
	idevice->handle = iadapter->fd;
	galCreateVertexShader(idevice);
	galCreateColorPixelShader(idevice);
	(*ppDevice) = &idevice->base;

	return GI_SUCCESS;
}



