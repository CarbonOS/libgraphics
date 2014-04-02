#include <stdio.h>
#include <graphics/graphics.h>


int main (int argc, char *argv[])
{
	HCONTEXT context;
	HDISPLAY display;
	HRENDERTARGET target;

	if (!(context = CreateContext(NULL)))
	{
		printf("CreateContext failed!\n");
	}

	if (!(display = GetMainDisplay(NULL)))
	{
		printf("GetMainDisplay failed!\n");
	}

	if (!(target = CreateDisplayRenderTarget(NULL, display)))
	{
		printf("CreateDisplayRenderTarget failed!\n");
	}

	if (SetRenderTarget(context, target) <= 0)
	{
		printf("SetRenderTarget failed!\n");
	}

	COLOR color;
	color.r = 0.0f;
	color.g = 1.0f;
	color.b = 1.0f;
	color.a = 1.0f;

	RECT bounds;
	bounds.x = -99;
	bounds.y = -97;
	bounds.width = 1000;
	bounds.height = 800;

	FillRectangle(context, &color, &bounds);

	printf("OK\n");

	return 0;
}

