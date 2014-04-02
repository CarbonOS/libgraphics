#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <drm/drm.h>
#include <drm/drm_mode.h>
#include <linux/limits.h>
#include "graphics.h"
#include "device.h"
#include "private.h"
#include <errno.h>
#include "utils.h"

#ifndef DEVICE_PATH
#define DEVICE_PATH "/dev/dri/card%d"
#endif

#ifndef DEFAULT_DEVICE
#define DEFAULT_DEVICE "/dev/dri/card0"
#endif

/* Locals */
static DEFINE_LIST(devlist);

static int GetResources(struct IDevice *device)
{
	struct drm_mode_card_res res;
	memset(&res, 0, sizeof(res));

	res.crtc_id_ptr = U64PTR(device->crtcs);
	res.count_crtcs = MAX_CRTCS;

	res.connector_id_ptr = U64PTR(device->connectors);
	res.count_connectors = MAX_DISPLAYS;

	res.encoder_id_ptr = U64PTR(device->encoders);
	res.count_encoders = MAX_ENCODERS;

	if (ioctl(device->fd, DRM_IOCTL_MODE_GETRESOURCES, &res)) {
		return -1;
	}

	device->count_crtcs = res.count_crtcs;
	device->count_displays = res.count_connectors;
	device->count_encoders = res.count_encoders;
	return 0;
}

/* Open a graphics device */
HDEVICE OpenDevice(const char *devname)
{
	int handle = -1;
	const char *filename;
	filename = (devname == NULL ? DEFAULT_DEVICE : devname);
	if ((handle = open(filename, O_RDWR, 0)) < 0) {
		return NULL;
	}

	struct drm_version version = { 0 };
	if (ioctl(handle, DRM_IOCTL_VERSION, &version)) {
		close(handle);
		return NULL;
	}

	struct stat data;
	data.st_rdev = 0;
	if (fstat(handle, &data)) {
	}

	struct drm_auth auth;
	if (ioctl(handle, DRM_IOCTL_GET_MAGIC, &auth)) {
		close(handle);
		return NULL;
	}

	struct IDevice *device = NULL;
	if (!(device = (struct IDevice*)calloc(1, sizeof(struct IDevice)))) {
		close(handle);
		return NULL;
	}

	device->fd = handle;
	device->dev = data.st_rdev;
	device->magic = auth.magic;
	device->driver.device = device;
	device->driver.fd = handle;
	IListAdd(&device->node, &devlist);
	GetResources(device);
	LoadDriver(device);
	return (HDEVICE)device;
}

/* Open a graphics device with file node. */
HDEVICE OpenDeviceFromFD(int fd)
{
	struct drm_version version = { 0 };
	if (ioctl(fd, DRM_IOCTL_VERSION, &version)) {
		return NULL;
	}

	struct stat data;
	data.st_rdev = 0;
	if (fstat(fd, &data)) {
	}

	struct drm_auth auth;
	if (ioctl(fd, DRM_IOCTL_GET_MAGIC, &auth)) {
		return NULL;
	}

	struct IDevice *device = NULL;
	if (!(device = (struct IDevice*)calloc(1, sizeof(struct IDevice)))) {
		return NULL;
	}

	device->fd = fd;
	device->dev = data.st_rdev;
	device->magic = auth.magic;
	device->driver.device = device;
	device->driver.fd = fd;
	IListAdd(&device->node, &devlist);
	GetResources(device);
	LoadDriver(device);
	return (HDEVICE)device;
}

int DeviceAuth(HDEVICE handle, unsigned int token)
{
	struct IDevice *device;
	if (!(device = (struct IDevice*)handle)) {
		return -1;
	}

	struct drm_auth auth;
	auth.magic = token;
	if (ioctl(device->fd, DRM_IOCTL_AUTH_MAGIC, &auth)) {
		printf("err:%d\n", errno);
		return 0;
	}

	return 1;
}

HDEVICE OpenDeviceFromDEV(dev_t dev)
{
	int index = 0;
	int handle = -1;
	struct stat data;
	char filename[PATH_MAX];
	while ((index <= 16)) {

		snprintf(filename, PATH_MAX, DEVICE_PATH, index);
		if ((handle = open(filename, O_RDWR, 0)) < 0) {
			continue;
		}

		if (fstat(handle, &data)) {
			close(handle);
			continue;
		}

		if (data.st_rdev == dev) {
			return OpenDeviceFromFD(handle);
		}

		close(handle);
		index++;
	}
	return NULL;
}

int GetDeviceFD(HDEVICE handle)
{
	handle = GetDevice(handle);
	if (!((struct IDevice*)handle)) {
		return -1;
	}
	return (handle->fd);
}

/* Simple Private Helper Function. */
void* GetPipeInstance(HDEVICE handle)
{
	handle = GetDevice(handle);
	if (!((struct IDevice*)handle)) {
		return NULL;
	}
	if (!handle->driver.GetPipeInstance) {
		return NULL;
	}
	return handle->driver.GetPipeInstance(&handle->driver);
}

/* Simple Private Helper Function. */
unsigned int GetDeviceToken(HDEVICE handle)
{
	struct IDevice *device;
	if (!(device = (struct IDevice*)handle)) {
		return -1;
	}
	return device->magic;
}

/* Simple Private Helper Function. */
HDEVICE GetDevice(HDEVICE device)
{
	static HDEVICE instance = NULL;
	if (device != NULL) {
		return device;
	}
	if (instance == NULL) {
		instance = OpenDevice(NULL);
	}
	return instance;
}

/* Release a graphics device. */
int ReleaseDevice(HDEVICE device)
{
	RETURN_IF_NULL(device);

	if (device->driver.Close) {
		device->driver.Close(&device->driver);
	}

	close(device->fd);
	free(device);
	return GI_SUCCESS;
}

