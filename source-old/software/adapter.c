#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/sysinfo.h>
#include <linux/types.h>
#include <linux/limits.h>
#include <linux/fb.h>
#include <graphics.h>
#include <device.h>
#include "framebuffer.h"

/* Summary */
static FBAdapter fbadapter;

/* Summary */
/* Retrieves all available adapter information. */
static int fbGetAdapterInfo(IAdapter *pAdapter, AdapterInfo *info)
{
	FBADAPTER(pAdapter);

	if ((info == NULL))
	{
		return -1;
	}

	struct sysinfo sinfo;
	if (sysinfo(&sinfo))
	{
		return -1;
	}

	info->type = GI_ADAPTER_TYPE_CPU;
	info->vram_total = sinfo.totalram;
	info->vram_available = sinfo.freeram;

	return 0;
}

/* Summary */
int FramebufferAdapterInit(void)
{
	memset(&fbadapter, 0, sizeof(fbadapter));
	fbadapter.base.GetAdapterInfo = fbGetAdapterInfo;
	fbadapter.base.EnumDisplays = fbEnumDisplays;
	fbadapter.base.CreateGraphicsDevice = fbCreateGraphicsDevice;
	RegisterAdapter(&fbadapter.base);

	return 1;
}

