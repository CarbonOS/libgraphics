#include <stdio.h>
#include "graphics.h"
#include "device.h"
#include "private.h"

/* Creates a new Context object. */
HRENDERTARGET CreateRenderTarget(HDEVICE handle, unsigned int width, unsigned int height)
{
	IDevice *device;
	handle = GetDevice(handle);
	if (!(device = (IDevice*)handle)) {
		return NULL;
	}

	if (!device->extension.CreateRenderTarget) {
		return NULL;
	}

	return (HRENDERTARGET)device->extension.CreateRenderTarget(&device->extension, width, height, 0);
}

/* Creates a new Display RenderTarget object. */
HRENDERTARGET CreateDisplayRenderTarget(HDEVICE handle, HDISPLAY display)
{
	IDevice *device;
	handle = GetDevice(handle);
	if (!(device = (IDevice*)handle)) {
		return NULL;
	}

	if (!device->extension.CreateDisplayRenderTarget) {
		return NULL;
	}

	return (HRENDERTARGET)device->extension.CreateDisplayRenderTarget(&device->extension, (IDisplay*)display);
}

/* Releases a Context object. */
int ReleaseRenderTarget(HRENDERTARGET handle)
{
	IRenderTarget *target;
	if (!(target = (IRenderTarget*)handle)) {
		return 0;
	}

	if (!target->ext->ReleaseRenderTarget) {
		return 0;
	}

	return target->ext->ReleaseRenderTarget(target);
}

