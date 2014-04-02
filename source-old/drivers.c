#include <stdio.h>
#include <dlfcn.h>
#include <dirent.h>
#include <system/core.h>
#include <system/io.h>
#include "graphics.h"
#include "device.h"
#include "private.h"

int TryOpenAdapter(IAdapter *adapter, const char *name)
{
	char path[PATH_MAX];
	snprintf(path, PATH_MAX, "%s/%s", ADAPTER_DRIVERS, name);

	void *vdl = NULL;
	if (!(vdl = dlopen(path, RTLD_NOW|RTLD_GLOBAL)))
	{
		return GI_ERROR;
	}

	OpenAdapter entry;
	if (!(entry = (OpenAdapter)(dlsym(vdl, "OpenAdapterICD"))))
	{
		dlclose(vdl);
		return GI_ERROR;
	}

	if (entry(adapter) != GI_SUCCESS)
	{
		dlclose(vdl);
		return GI_ERROR;
	}

	return GI_SUCCESS;
}

int LoadDriver(IAdapter *iadapter)
{
	DIR *drivers;
	if ((drivers = opendir(ADAPTER_DRIVERS)) == NULL)
	{
		return GI_ERROR;
	}

	struct dirent *entry;
	while ((entry = readdir(drivers)) != NULL)
	{
		if (strlen(entry->d_name) <= 3)
			continue;

		if (TryOpenAdapter(iadapter, entry->d_name) == GI_SUCCESS)
		{
			break;
		}
	}

	return GI_SUCCESS;
}

