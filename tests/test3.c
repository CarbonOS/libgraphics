#include <stdio.h>
#include <string.h>
#include <graphics/graphics.h>
#include <settings.h>

/* Entry Point */
int main(int argc, char *argv[])
{
	DevicePtr device;
	SurfacePtr surface;

	if (CreateDevice(NULL, &device) != GI_SUCCESS)
	{
		printf("CreateDevice() failed!\n");

		return -1;
	}

	if (CreateSurfaceFromHandle(device, &surface, 0x00000001) != GI_SUCCESS)
	{
		printf("CreateSurfaceFromHandle() failed!\n");

		return -1;
	}

	return 0;
}


