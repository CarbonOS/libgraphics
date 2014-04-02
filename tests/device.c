#include <stdio.h>
#include <graphics/graphics.h>


int main (int argc, char *argv[])
{
	HDEVICE device;
	HDISPLAY display;
	HCURSOR cursor;
	HSURFACE buffer;
	HSWAPCHAIN swap;


	if (!(device = OpenDevice(NULL)))
	{
		printf("OpenDevice failed!\n");
	}

	if (!(display = GetMainDisplay(device)))
	{
		printf("GetMainDisplay failed!\n");
	}

	DumpDisplay(display);

	if (!(cursor = CreateCursor(device)))
	{
		printf("CreateCursor failed!\n");
	}

	if (!(buffer = GetDisplaySurface(display)))
	{
		printf("GetDisplaySurface failed!\n");
	}
/*
	if (!(swap = CreateSwapChain(device, 1920, 1080, 32)))
	{
		printf("CreateSwapChain failed!\n");
	}

	SwapChainFlip(swap, display);
	SwapChainFlip(swap, display);
	SwapChainFlip(swap, display);
	SwapChainFlip(swap, display);
*/
	void *data;
	if ((data = MapSurface((HSURFACE)buffer)) != NULL)
	{
		memset(data, 100, 64*64*4);
	}

	ShowCursor(display, cursor);
	MoveCursor(display, 300, 100);

	printf("Displays: %d\n", GetDisplayCount(device));
/*
	printf("ID: %d\n", GetDisplayID(device, 0));
*/


//	while (WaitDisplayEvent(display))
	{

	}

	printf("OK\n");

	return 0;
}

