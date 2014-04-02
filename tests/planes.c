#include <stdio.h>
#include <graphics/graphics.h>


int main (int argc, char *argv[])
{
	HDEVICE device;
	HDISPLAY display;
	HCURSOR cursor;
	HSURFACE buffer;
	int count;
	int index;
	if (!(device = OpenDevice(NULL)))
	{
		printf("OpenDevice failed!\n");
	}

	//DumpCRTC(device);

/*
	count = GetDisplayCount(device);
	for (index = 0; index < count; index++) {
		OpenDisplay(device, index);
	}
*/
	printf("OK\n");

	return 0;
}

