#include <stdio.h>
#include <system/task.h>
#include <graphics/graphics.h>
#include <graphics/view.h>

int main (int argc, char *argv[])
{
	HDEVICE device;
	HVIEW view;

	if (!(device = OpenDevice(NULL)))
	{
		printf("OpenDevice failed!\n");
	}

	if (!(view = CreateView(NULL, 0, 400, 300))) {
	
		printf("CreateView failed!\n");
	}

	printf("pid: %d\n", getpid());

	RECT bounds;
	GetViewBounds(view, &bounds);
	printf("x: %d\n", bounds.x);
	printf("y: %d\n", bounds.y);
	printf("w: %d\n", bounds.width);
	printf("h: %d\n", bounds.height);

//	if (!(device = GetDeviceFromView(view))) {
//		printf("GetDeviceFromView failed!\n");
//	}

	GetViewSurface(device, view);

	printf("getmessage\n");

//	ReleaseDevice(device);

	TASKMSG msg;
	memset(&msg, 0, sizeof(msg));
	while ((msg.type != TASKMSG_EXIT))
	{
		if (TaskWaitMessage(&msg, -1)) {
			continue;
		}

		printf("msg: %d\n", msg.type);
		continue;
	}

	printf("OK\n");

	return 0;
}

