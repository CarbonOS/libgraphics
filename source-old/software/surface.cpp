#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include "graphics.h"
#include "device.h"
#include "software.h"
#include "private.h"
#include "format.h"

/* */
static inline int swGetNextSharedKey(void)
{
	int count;
	int index;
	int shm_id;
	key_t last_key = 0;
	struct shmid_ds shm_info;
	struct shmid_ds shm_segment;

	if ((count = shmctl(0, SHM_INFO, &shm_info)) <= 0)
		return -1;

	for (index = 0; index <= count; index++)
	{
		if ((shm_id = shmctl(index, SHM_STAT, &shm_segment)) < 0)
			continue;
		if (shm_segment.shm_perm.__key > last_key)
			last_key = shm_segment.shm_perm.__key;
	}

	return (int)(last_key + 1);
}

int swCreateSurface(IDevice *pDevice, ISurface **ppSurface, int width, int height, int format)
{
	unsigned size;

	if ((ppSurface == NULL))
		return GI_ERROR;

	if ((width <= 0) || (height <= 0))
		return GI_ERROR;

	SWSurface *surface = NULL;
	if ((surface = (SWSurface*)malloc(sizeof(SWSurface))) == NULL)
		return GI_ERROR;

	memset(surface, 0, sizeof(SWSurface));
	surface->size = 0;
	surface->base.width = width;
	surface->base.height = height;
	surface->base.pitch;
	surface->base.format = format;
	surface->base.handle = 0;
	(*ppSurface) = &surface->base;
	return GI_SUCCESS;
}

int swMapSurface(ISurface *pSurface, void **ptr)
{
	if ((pSurface == NULL) || (ptr == NULL))
		return GI_ERROR;

	SWSurface *surface = NULL;
	if ((surface = (SWSurface*)pSurface) == NULL)
		return GI_ERROR;

	return GI_SUCCESS;
}

int swDestroySurface(ISurface *pSurface)
{
	SWSurface *surface = NULL;
	if ((surface = (SWSurface*)pSurface) == NULL)
		return GI_ERROR;

	return GI_SUCCESS;
}





