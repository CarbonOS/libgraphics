#include <stdio.h>
#include <dlfcn.h>
#include <string.h>
#include <limits.h>
#include <library.h>
#include <settings.h>
#include "private.h"

/// <summary>
/// 
/// </summary>
void LibraryInit(void)
{
	AdapterListInit();

	//FramebufferAdapterInit();

	return;
}

/* TODO */
int AdapterInit(const char *filename)
{
	void *vdl = NULL;

	pfnAdapterInit init;

	if (!(vdl = dlopen(filename, RTLD_NOW|RTLD_GLOBAL)))
	{
		return 0;
	}

	if (!(init = (pfnAdapterInit)(dlsym(vdl, "GraphicsAdapterInit"))))
	{
		dlclose(vdl);
		return 0;
	}

	if ((init()))
		return 0;

	return 0;
}

/* TODO */
int AdapterListInit(void)
{
	char *line;
	FILE *handle;
	char filename[PATH_MAX] = { 0 };

	if ((handle = fopen(GRAPHICS_ICD, "r")) == NULL)
	{
		return -1;
	}

	while ((fgets(filename, PATH_MAX, handle)))
	{
		if (!(line = strtok(filename, "\n")))
			continue;

		if ((line[0] == '#') || (line[0] == '\n'))
			continue;

		AdapterInit(line);
		continue;
	}

	fclose(handle);

	return 0;
}

/// <summary>
/// 
/// </summary>
void LibraryExit(void)
{
	//ExitAdapterList();

	return;
}


