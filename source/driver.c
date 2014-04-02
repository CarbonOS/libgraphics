#include <stdio.h>
#include <dlfcn.h>
#include <string.h>
#include <dirent.h>
#include "graphics.h"
#include "device.h"
#include "private.h"

#ifndef DRIVERS_DIR
#define DRIVERS_DIR "/System/Graphics"
#endif

typedef int (*OpenGraphicsICD)(HDRIVER driver);

static int TryOpenDriver(struct IDevice *device, const char *name)
{
	void *vdl = NULL;
	char path[PATH_MAX];
	snprintf(path, PATH_MAX, "%s/%s", DRIVERS_DIR, name);
	if (!(vdl = dlopen(path, RTLD_NOW|RTLD_GLOBAL)))
	{
		return GI_ERROR;
	}

	OpenGraphicsICD entry = NULL;
	if (!(entry = (OpenGraphicsICD)(dlsym(vdl, "OpenGraphicsICD"))))
	{
		dlclose(vdl);
		return GI_ERROR;
	}

	if (entry(&device->driver) != GI_SUCCESS)
	{
		dlclose(vdl);
		return GI_ERROR;
	}

	return GI_SUCCESS;
}

/* Loads a Graphics ICD driver. */
int LoadDriver(struct IDevice *device)
{
	DIR *drivers = NULL;
	if (!(drivers = opendir(DRIVERS_DIR))) {
		return GI_ERROR;
	}

	struct dirent *entry;
	while ((entry = readdir(drivers)))
	{
		if (strlen(entry->d_name) <= 3) {
			continue;
		}
		if (TryOpenDriver(device, entry->d_name) == GI_SUCCESS) {
			break;
		}
	}

	closedir(drivers);
	return GI_SUCCESS;
}

