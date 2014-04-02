#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "graphics.h"
#include "device.h"
#include "private.h"
#include "utils.h"


int GetPropertyCount(HDISPLAY display)
{
	RETURN_IF_NULL(display);
	return (int)(display->count_props);
}

int GetProperty(HDISPLAY display, int index)
{
	RETURN_IF_NULL(display);
	if ((index < 0) || (index >= display->count_props)) {
		return 0;
	}

	struct drm_mode_get_property req;
	memset(&req, 0, sizeof(req));
	req.prop_id = display->props[index];
	if (ioctl(display->fd, DRM_IOCTL_MODE_GETPROPERTY, &req)) {
		return 0;
	}

	printf("prop: %s\n", req.name);
	return 1;
}

