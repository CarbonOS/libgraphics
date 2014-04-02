#include <stdio.h>
#include <system/task.h>
#include <graphics/graphics.h>
#include <graphics/rendering.h>
#include <graphics/view.h>

/* Globals */
static HVIEW view;
static HSURFACE surface;
static HCONTEXT context;

int main (int argc, char *argv[])
{
	HDEVICE device;

	/* Get the default graphic device. */
	if (!(device = GetDefaultDevice()))
	{
		printf("OpenDevice failed!\n");
	}

	/* Creates a new surface/buffer object. */
	if (!(surface = CreateSurface(device, 600, 400, 32))) {

		printf("CreateSurface failed!\n");
	}

	/* Creates a new surface view object. */
	if (!(view = CreateView(device, 0, 600, 400))) {
	
		printf("CreateView failed!\n");
	}

	SetViewSurface(view, surface);

	MoveView(view, 200, 200);




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

