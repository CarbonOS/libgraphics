#include <stdio.h>
#include <string.h>
#include <graphics/graphics.h>
#include <settings.h>

int main (int argc, char *argv[])
{
	void *ptr;
	AdapterPtr adapter;
	DisplayPtr display;
	SurfacePtr surface;
	ContextPtr context;
	DevicePtr device;
	DisplayInfo info;
	Color4F color;

	if (DefaultAdapter(&adapter) == GI_ERROR)
	{
		printf("DefaultAdapter() failed!\n");

		return -1;
	}

	if (EnumDisplays(adapter, 0, &display) != GI_SUCCESS)
	{
		printf("EnumDisplays() failed!\n");

		return -1;
	}

	if (GetDisplayInfo(display, &info) != GI_SUCCESS)
	{
		printf("GetDisplayInfo() failed!\n");

		return -1;
	}

	if (GetDisplaySurface(display, &surface) != GI_SUCCESS)
	{
		printf("GetDisplaySurface() failed!\n");

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

	if (SetRenderTarget(context, surface) == GI_ERROR)
	{
		printf("SetRenderTarget() failed!\n");

		return -1;
	}

	color.r = 1.0f;

	if (FillRectangleWithColor(context, 0, 0, 1000, 1000, color) == GI_ERROR)
	{
		printf("FillRectangleWithColor() failed!\n");

		return -1;
	}

	printf("OK\n");

	return 0;
}
