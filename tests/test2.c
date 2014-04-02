#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <graphics/graphics.h>
#include <cairo/cairo.h>

/* Entry Point */
int main (int argc, char *argv[])
{
	void *data;
	void *data2;
	AdapterPtr adapter;
	DevicePtr device;
	ContextPtr context;
	SurfacePtr surface1;
	SurfacePtr surface2;
	SurfacePtr backbuffer;
	DisplayPtr display;
	int count = 0;
	void *handle;

	cairo_surface_t *input;
	cairo_surface_t *output;

	Matrix4x4F matrix;

	Color4F c;
	c.r = 0.0f;
	c.g = 0.0f;
	c.b = 0.0f;
	c.a = 1.0f;

	Color4F c2;
	c2.r = 1.0f;
	c2.g = 1.0f;
	c2.b = 1.0f;
	c2.a = 1.0f;


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

	if (CreateDevice(adapter, &device) != GI_SUCCESS)
	{
		printf("CreateDevice() failed!\n");

		return -1;
	}

	if (CreateContext(device, &context) != GI_SUCCESS)
	{
		printf("CreateContext() failed!\n");

		return -1;
	}

	if (GetDisplaySurface(display, &surface1) != GI_SUCCESS)
//	if (CreateSurface(device, &surface1, 800, 600, 0) != GI_SUCCESS)
	{
		printf("CreateSurface() failed!\n");

		return -1;
	}


	if ((input = cairo_image_surface_create_from_png("/home/mediacore/images/background.png")) == NULL)
	{
		printf("cairo_image_surface_create_from_png failed!\n");

		return -1;
	}

	if (CreateSurface(device, &surface2, cairo_image_surface_get_width(input), cairo_image_surface_get_height(input), 0) != GI_SUCCESS)
	{
		printf("CreateSurface() failed!\n");

		return -1;
	}

	GetSurfaceHandle(surface1, handle);
	printf("0x%08x\n", (uint32_t)handle);

	if (MapSurface(surface2, &data2) == GI_ERROR)
	{
		printf("MapSurface() failed!\n");

		return 0;
	}

	memcpy(data2, cairo_image_surface_get_data(input), (cairo_image_surface_get_height(input) * cairo_image_surface_get_stride(input)));


	Point2F a, b, p;
	a.x = -200.0f;
	a.y = -200.0f;
	b.x = 200.0f;
	b.y = 200.0f;
	p.x = 0.0f;
	p.y = 0.0f;

	Rect4F rect;
	rect.x = -400;
	rect.y = -300;
	rect.width = 800;
	rect.height = 600;

	SetLineWidth(context, 8.0f);

	SetRenderTarget(context, surface2);

	//Clear(context, c2);

	DrawLineF(context, a, b);


	SetRenderTarget(context, surface1);

	Clear(context, c);






	DrawLineF(context, a, b);

	MatrixIdentity(&matrix);


	while (count < 360)
	{
		MatrixIdentity(&matrix);

		MatrixRotateZ(&matrix, (float)count);

		MatrixRotateY(&matrix, (float)count);

		MatrixSetCenter(&matrix, 1920, 1080);

		MatrixSetPerspective(&matrix, 2000.0f);

		WaitForVBlank(display);

		Clear(context, c);

		ResetTransform(context);

		DrawImageUnscaled(context, surface2, 100, 100);

		SetTransform(context, &matrix);

		DrawImage(context, surface2, 100, 100, 0, 0, 100, 100);



		DrawLineF(context, a, b);

		usleep(10000);

		count+=10;
	}




/*
	if (MapSurface(surface1, &data) == GI_ERROR)
	{
		printf("MapSurface() failed!\n");

		return 0;
	}

	unsigned char *buffer = (unsigned char *)data;

	if ((output = cairo_image_surface_create_for_data(buffer, CAIRO_FORMAT_ARGB32, 800, 600, 800 * 4)) == NULL)
	{
		printf("cairo_image_surface_create failed!\n");
	}

	cairo_surface_write_to_png(output, "test2.png");
//	cairo_surface_destroy(output);
*/

	ReleaseSurface(surface2);
	ReleaseSurface(surface1);
	ReleaseContext(context);
	ReleaseDevice(device);

	return 0;
}

