#include <stdio.h>
#include <stdlib.h>
#include <graphics/graphics.h>


int main (int argc, char *argv[])
{
	HDEVICE device;
	HDISPLAY display;
	HCURSOR cursor;
	HSURFACE buffer;
	GAMMA_RANGE gamma;
	DISPLAYINFO info;

	if (!(device = OpenDevice("/dev/dri/card1")))
	{
		printf("OpenDevice failed!\n");
	}

	if (!(display = GetMainDisplay(device)))
	{
		printf("GetMainDisplay failed!\n");
	}

	printf("Attach Display...\n");
	AttachDisplay(display);
	printf("Attached\n");

	if (!(cursor = CreateDefaultCursor(device)))
	{
		printf("CreateCursor failed!\n");
	}

	if (!(buffer = GetDisplaySurface(display)))
	{
		printf("GetDisplaySurface failed!\n");
	}
/*

*/

	void *surface_ptr = NULL;
	if ((surface_ptr = MapSurface(buffer))) {

		printf("mapped\n");
		memset(surface_ptr, 255, 1024*1024*4);
	}

	if (GetDisplayInfo(display, &info)) 
	{
		printf("gamma: %d\n", info.gamma);
	}

	memset(&gamma, 0, sizeof(gamma));
	//if (!GetGammaRange(display, &gamma))
	{
	//	printf("GetGammaRange failed!\n");
	}

	int ii = 0;
	for (ii; ii < 256; ii++) {

	//	printf("r: %d\n", gamma.red[ii]);
	//	printf("g: %d\n", gamma.green[ii]);
	//	printf("b: %d\n", gamma.blue[ii]);

	//	gamma.red[ii] = 10000;
	//	gamma.green[ii] = 10000;
	//	gamma.blue[ii] = 10000;
	}

	//SetGammaRange(display, &gamma);


	ShowCursor(display, cursor);
	MoveCursor(display, 500, 200);
//	ShowCursor(display, cursor);
	printf("Displays: %d\n", GetDisplayCount(device));
/*
	printf("ID: %d\n", GetDisplayID(device, 0));
*/


//	while (WaitDisplayEvent(display))
	{

	}

	int index = 0;
	while (index < 15) {


		sleep(1);
		index++;
	}
	printf("OK\n");

	return 0;
}

