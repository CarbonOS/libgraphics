#include <stdio.h>
#include <settings.h>
#include <graphics/graphics.h>

/* Entry Point */
int main (int argc, char *argv[])
{

	DevicePtr device;
	AdapterPtr adapter;
	DisplayPtr display;
	SurfacePtr surface;
	ContextPtr context;

	setenv("NOUVEAU_MESA_DEBUG", "1");

	if (DefaultAdapter(&adapter) == GI_ERROR)
	{
		printf("DefaultAdapter() failed!\n");

		return -1;
	}

	if (EnumDisplays(adapter, 0, &display) == GI_ERROR)
	{
		printf("EnumDisplays() failed!\n");

		return -1;
	}

	if (CreateDevice(adapter, &device) == GI_ERROR)
	{
		printf("CreateDevice() failed!\n");

		return -1;
	}

	if (CreateContext(device, &context) == GI_ERROR)
	{
		printf("CreateContext() failed!\n");

		return -1;
	}

	if (CreateDisplaySurface(context, display, &surface) == GI_ERROR)
	{
		printf("CreateDisplaySurface() failed!\n");

		return -1;
	}

	ClearRenderTarget(context, surface);

	//SetRenderTarget(context, surface);

	//Clear(context);

	//GetDeviceDesc(device);


	return 0;
}


