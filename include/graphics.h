#ifndef __GRAPHICS_FRAMEWORK__
#define __GRAPHICS_FRAMEWORK__

/* Includes */
#include "types.h"
#include "format.h"
#include "rect.h"

/* C++ Interface */
#ifdef __cplusplus
extern "C" {
#endif

/* Error Codes */
#define GI_SUCCESS           0
#define GI_ERROR            -1
#define GI_NOMEMORY         -2
#define GI_UNIMPLEMENTED    -3


/* Surface Info data */
typedef struct __SurfaceInfo
{
	unsigned width;
	unsigned height;
	unsigned pitch;
	unsigned handle;
	unsigned depth;
	unsigned bpp;

} SURFACEINFO;

/* Display info object */
typedef struct __DisplayInfo
{
	unsigned crtc;
	unsigned width;
	unsigned height;
	unsigned handle;
	unsigned gamma;
	unsigned id;
	int fd;

} DISPLAYINFO;

/* Display mode object */
typedef struct __DisplayMode
{
	int width;
	int height;
	int depth;
	int orientation;
	int handle;
	int bpp;
	int id;

} DisplayMode;

/* Display gamma data */
typedef struct __GammaRange
{
	unsigned short red[256];
	unsigned short green[256];
	unsigned short blue[256];

} GAMMA_RANGE;

typedef struct IDriver *HDRIVER;
typedef struct IDevice *HDEVICE;

typedef struct IDisplay *HDISPLAY;
typedef struct __surface *HSURFACE;


typedef struct __cursor* HCURSOR;

/* Open a graphics device */
HDEVICE OpenDevice(const char *devname);

/* Open a graphics device with file node. */
HDEVICE OpenDeviceFromFD(int fd);

/* Returns the default compositor device. */
HDEVICE GetDefaultDevice(void);

unsigned int GetDeviceToken(HDEVICE handle);

int DeviceAuth(HDEVICE handle, unsigned int token);

int GetDeviceFD(HDEVICE handle);

/* Release a graphics device. */
int ReleaseDevice(HDEVICE device);


int GetDisplayCount(HDEVICE device);



HDISPLAY OpenDisplay(HDEVICE device, int index);

/* Get the Surface handle for the Display. */
HSURFACE GetDisplaySurface(HDISPLAY display);

int GetDisplayInfo(HDISPLAY handle, DISPLAYINFO *info);

int GetDisplayBounds(HDISPLAY handle, RECT *bounds);

/* Returns a Boolean value indicating whether a display is connected or online. */
int IsDisplayConnected(HDISPLAY display);

int IsDisplayAttached(HDISPLAY handle);

int GetPropertyCount(HDISPLAY display);
int GetProperty(HDISPLAY display, int index);

int AttachDisplay(HDISPLAY handle);
int DetachDisplay(HDISPLAY handle);
int GetGammaRange(HDISPLAY display, GAMMA_RANGE *data);
int SetGammaRange(HDISPLAY display, GAMMA_RANGE *data);


int WaitDisplayEvent(HDISPLAY display);
int WaitVblank(HDISPLAY display);



HDISPLAY GetMainDisplay(HDEVICE device);

/* Release a display handle. */
int ReleaseDisplay(HDISPLAY display);

int GetDisplayFD(HDISPLAY handle);




HCURSOR CreateCursor(HDEVICE device);

int ShowCursor(HDISPLAY display, HCURSOR cursor);

int MoveCursor(HDISPLAY display, int x, int y);

int HideCursor(HDISPLAY display);



/* Creates a new surface/buffer object. */
HSURFACE CreateSurface(HDEVICE device, unsigned width, unsigned height, unsigned bpp);

/* Creates a new surface/buffer object from a shared handle. */
HSURFACE SurfaceFromHandle(HDEVICE device, int width, int height, int bpp, unsigned int handle);

/* Get a pointer to the data contained in the surface/buffer. */
void* MapSurface(HSURFACE surface);

unsigned int GetSharedHandle(HSURFACE surface);

unsigned int GetSurfaceToken(HSURFACE surface);





#ifdef __cplusplus
}
#endif

#endif
