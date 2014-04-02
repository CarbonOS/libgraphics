#ifndef __FRAMEBUFFER_PRIVATE__
#define __FRAMEBUFFER_PRIVATE__

#define DEVICES "/dev"
#define DEVICES_FB DEVICES "/fb"

/* C++ Interface */
#ifdef __cplusplus
extern "C" {
#endif

/* Includes */
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <state_tracker/sw_winsys.h>
#include <pipe/p_screen.h>
#include <linux/types.h>
#include <linux/limits.h>
#include <linux/fb.h>

/* Private Framebuffer Surface */
typedef struct __FBSurface
{
	ISurface base;
	struct FBDevice *device;
	size_t length;
	void *mapped;
	int handle;
	int state;

} FBSurface;

/* Private Framebuffer Adapter */
typedef struct __FBAdapter
{
	IAdapter base;
	int handle;

} FBAdapter;

/* Private Framebuffer Display */
typedef struct __FBDisplay
{
	IDisplay base;
	FBAdapter *adapter;
	struct fb_fix_screeninfo fix;
	struct fb_var_screeninfo var;
	int handle;

} FBDisplay;

/* Private Framebuffer Device */
typedef struct __FBDevice
{
	IDevice base;
	FBAdapter *adapter;
	int handle;

} FBDevice;

/* Private Framebuffer Context */
typedef struct __FBContext
{
	IContext base;
	ISurface *target;
	FBDevice *device;
	IRasterstate state;
	int handle;

	struct {
		unsigned x;
		unsigned y;
		unsigned width;
		unsigned height;
	} clipping;

} FBContext;

#define FBADAPTER(ptr) \
	FBAdapter *fba = NULL; \
	if ((fba = (FBAdapter*)ptr) == NULL) \
		return GI_ERROR;

#define FBDISPLAY(ptr) \
	FBDisplay *fbd = NULL; \
	if ((fbd = (FBDisplay*)ptr) == NULL) \
		return GI_ERROR;

#define FBSURFACE(ptr) \
	FBSurface *fbs = NULL; \
	if ((fbs = (FBSurface*)ptr) == NULL) \
		return GI_ERROR;

int fbEnumDisplays(IAdapter *pAdapter, int index, IDisplay **ppDisplay);

int fbCreateGraphicsDevice(IAdapter *pAdapter, IDevice **ppDevice);

int fbCreateContext(IDevice *pDevice, IContext **ppContext);

int fbFillRectangleWithColor(IContext *pContext, unsigned x, unsigned y, unsigned width, unsigned height, Color4F color);

#ifdef __cplusplus
}
#endif

#endif
