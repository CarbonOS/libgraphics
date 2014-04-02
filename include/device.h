#ifndef __GRAPHICS_DEVICE__
#define __GRAPHICS_DEVICE__

#include <stdio.h>
#include <stdint.h>

/* C++ Interface */
#ifdef __cplusplus
extern "C" {
#endif

/* The Driver object */
typedef struct IDriver
{
	int fd;
	void *private;
	HDEVICE device;
	const char *(*GetVendor)(struct IDriver *driver);
	void *(*GetPipeInstance)(struct IDriver *driver);
	int (*Close)(struct IDriver *driver);

} *HDRIVER;


#ifdef __cplusplus
}
#endif
#endif

