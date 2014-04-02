#include <stdio.h>
#include <string.h>
#include <graphics/graphics.h>
#include <settings.h>

int main (int argc, char *argv[])
{
	AdapterPtr adapter;
	DevicePtr device;

	if (DefaultAdapter(&adapter) != GI_SUCCESS)
	{
		printf("DefaultAdapter() failed!\n");

		return -1;
	}

	if (CreateDevice(adapter, &device) != GI_SUCCESS)
	{
		printf("CreateDevice() failed!\n");

		return -1;
	}

	ReleaseDevice(device);

	printf("OK\n");

	return 0;
}

