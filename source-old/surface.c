#include <stdio.h>
#include <stdlib.h>
#include "graphics.h"
#include "device.h"
#include "private.h"

/* Get a pointer to the data contained in the surface/buffer. */
int MapSurface(SurfacePtr pSurface, void **ptr)
{
	IBuffer *ibuffer = NULL;
	if ((ibuffer = (IBuffer*)pSurface) == NULL)
	{
		printf("IBuffer::invalid\n");
		return GI_ERROR;
	}

	void *data = NULL;
	if (MapBuffer(ibuffer, &data) != GI_SUCCESS)
	{
		printf("MapBuffer::failed\n");
		return GI_ERROR;
	}

	(*ptr) = data;
	return GI_SUCCESS;
}


