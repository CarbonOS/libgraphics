#include <stdio.h>
#include <string.h>
#include <settings.h>
#include <graphics/graphics.h>
#include <cairo/cairo.h>



int main (int argc, char *argv[])
{
	void *data;
	AdapterPtr adapter;
	DevicePtr device;
//	ContextPtr context;
	SurfacePtr surface;
	SurfaceInfo info;
	cairo_surface_t *output;
	DisplayPtr display;
	FontPtr font;
	int count = 0;

	Matrix4x4F matrix;

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

//	if (CreateDevice(adapter, &device) != GI_SUCCESS)
	{
		printf("CreateDevice() failed!\n");

		return -1;
	}
/*
	if (CreateContext(device, &context) != GI_SUCCESS)
	{
		printf("CreateContext() failed!\n");

		return -1;
	}
*/

	if (GetDisplaySurface(display, &surface) != GI_SUCCESS)
//	if (CreateSurface(device, &surface, 800, 600, 0) != GI_SUCCESS)
	{
		printf("CreateSurface() failed!\n");

		return -1;
	}

	GetSurfaceInfo(surface, &info);

//	SetRenderTarget(context, surface);

	Color4F c;
	c.r = 1.0f;
	c.g = 1.0f;
	c.b = 1.0f;
	c.a = 1.0f;



	Rect4F bounds;

	bounds.x = 0;
	bounds.y = 0;
	//FillRectangleWithColor(context, bounds, c);

	bounds.x = 200;
	bounds.y = 100;
	//FillRectangleWithColor(context, bounds, c);

//	Clear(context, c);


	Point2F a, b;
	a.x = -100.0f;
	a.y = -100.0f;
	b.x = 100.0f;
	b.y = 100.0f;


	printf("%d, %d, %d\n",  1 << 17, 0x00000800, 32 * 4);



	Rect4F rect;
	rect.x = 200;
	rect.y = 200;
	rect.width = 600;
	rect.height = 400;


//	DrawLineF(context, a, b);

//	SetLineWidth(context, 8.0f);

	MatrixIdentity(&matrix);
	//MatrixSetCenter(&matrix, info.width, info.height);
	//MatrixSetPerspective(&matrix, info.width, info.height, 1.0f, 1000.0f);

	//while (count < 200)
	{
		MatrixRotateY(&matrix, 40.0f);

		//MatrixRotateZ(&matrix, 15.0f);

//		SetTransform(context, &matrix);

//		Clear(context, c);

//		DrawLineF(context, a, b);

//		DrawRectangleF(context, rect);

		usleep(100000);

		count++;
	}




union { float f; unsigned int i; } u;
u.f = 100.0f;

	printf("0x%08x\n", u.i);

/*
	if (MapSurface(surface, &data) == GI_ERROR)
	{
		printf("MapSurface() failed!\n");

		return 0;
	}

	unsigned char *buffer = (unsigned char *)data;

	if ((output = cairo_image_surface_create_for_data(buffer, CAIRO_FORMAT_ARGB32, 800, 600, 800 * 4)) == NULL)
	{
		printf("cairo_image_surface_create failed!\n");
	}

	cairo_surface_write_to_png(output, "test1.png");
*/

	ReleaseSurface(surface);
//	ReleaseContext(context);
//	ReleaseDevice(device);

	return 0;
}

