#ifndef __GRAPHICS_PRIVATE__
#define __GRAPHICS_PRIVATE__

/* cpp Interface */
#ifdef __cplusplus
extern "C" {
#endif

/* Includes */
#include <stdint.h>
#include <drm/drm.h>
#include <drm/drm_mode.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <assert.h>
#include "list.h"

/* Limits */
#define MAX_CRTCS      8
#define MAX_DISPLAYS   8
#define MAX_ENCODERS   8
#define MAX_MODES      32
#define MAX_PROPS      16

#define U64PTR(x) ((uint64_t)(unsigned long)(x))

#ifndef PRIVATE 
#define PRIVATE __attribute__ ((visibility("hidden")))
#endif

#ifndef RETURN_IF_NULL
#define RETURN_IF_NULL(ptr) \
if (__builtin_expect(!(ptr), 0)) { \
	return 0; \
}
#endif

/* The Device object */
typedef struct IDevice
{
	int fd;
	struct IDriver driver;
	struct list_head node;
	uint32_t encoders[MAX_ENCODERS];
	uint32_t connectors[MAX_DISPLAYS];
	uint32_t crtcs[MAX_CRTCS];
	uint32_t count_crtcs;
	uint32_t count_displays;
	uint32_t count_encoders;
	uint32_t magic;
	dev_t dev;

} *HDEVICE;

/* Resource Object */
typedef struct IResource
{
	int fd;
	uint32_t fb;
	uint32_t bpp;
	unsigned depth;
	unsigned width;
	unsigned height;
	unsigned stride;
	unsigned size;
	unsigned handle;
	unsigned shared;
	uint32_t map_count;
	uint64_t map;

	struct IDevice *device;

} *HRESOURCE;

/* The Buffer Object */
typedef struct __IBuffer
{
	int fd;
	uint32_t fb;
	uint32_t bpp;
	unsigned depth;
	uint32_t width;
	uint32_t height;
	uint32_t stride;
	uint32_t size;
	uint32_t handle;
	uint32_t shared;
	uint32_t map_count;
	uint64_t map;

	struct IDevice *device;

} IBuffer;

/* The Display/Output object */
typedef struct IDisplay
{
	int fd;
	uint32_t id;
	uint32_t fb;
	uint32_t crtc;
	uint32_t count_props;
	uint32_t props[MAX_PROPS];
	uint64_t props_values[MAX_PROPS];
	struct IDevice *device;
	struct drm_mode_modeinfo modes[MAX_MODES];
	int mod_count;

} IDisplay, *HDISPLAY;

/* The SwapChain object */
typedef struct __ISwapChain
{
	int fd;
	uint32_t width;
	uint32_t height;
	uint32_t front;
	uint32_t count;
	IBuffer buffer[2];

} ISwapChain;

/* Private Function */
PRIVATE HDEVICE GetDevice(HDEVICE device);

/* Private Function */
PRIVATE int LoadDriver(struct IDevice *idevice);

/* Private Function */
//PRIVATE int CreateBuffer(struct IDevice *idevice, IBuffer **pbuffer, unsigned int width, unsigned int height, unsigned int bpp);
PRIVATE int CreateResource(struct IDevice *device, struct IResource *res, unsigned int width, unsigned int height, unsigned int bpp);

/* Private Function */
PRIVATE int CreateBufferFromHandle(struct IDevice *idevice, IBuffer **pbuffer, unsigned int width, unsigned int height, unsigned int bpp, unsigned int name);

/* Private Function */
PRIVATE int MapBuffer(IBuffer *ibuffer, void **ptr);

/* Private Function */
PRIVATE int UnmapBuffer(IBuffer *ibuffer, void *ptr);

/* Private Function */
PRIVATE unsigned int GetBufferHandle(IBuffer *ibuffer);
PRIVATE unsigned int GetResourceHandle(struct IResource *res);

/* Private Function */
PRIVATE int Viewcall(int id, int cmd, void *args);

/* Private Function */
PRIVATE __attribute__ ((constructor)) void LibraryInit(void);

/* Private Function */
PRIVATE __attribute__ ((destructor)) void LibraryExit(void);

#ifdef __cplusplus
}
#endif
#endif

