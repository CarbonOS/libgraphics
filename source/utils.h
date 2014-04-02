#ifndef __GRAPHICS_UTILS__
#define __GRAPHICS_UTILS__

/* Includes */
#include <errno.h>
#include <string.h>

/* Copy image data to the surface. */
static inline int CopyDataToSurface(IBuffer *pbuffer, const void *src, unsigned int length)
{
	void *dst = NULL;
	if (!pbuffer || !src || length <= 0) {
		return 0;
	}

	if ((length > pbuffer->size)) {
		return 0;
	}

	if (MapBuffer(pbuffer, &dst) != GI_SUCCESS) {
		return 0;
	}

	memcpy(dst, src, length);
	UnmapBuffer(pbuffer, dst); 
	return 1;
}

static inline int IOControl(int fd, unsigned long request, void *arg)
{
	int ret;
	do {
		ret = ioctl(fd, request, arg);
	} while (ret && (errno == EINTR || errno == EAGAIN));
	return ret;
}

#endif

