#ifndef __GRAPHICS_VIEW__
#define __GRAPHICS_VIEW__

#include <stdio.h>
#include <stdint.h>

/* C++ Interface */
#ifdef __cplusplus
extern "C" {
#endif

/* View Handle */
typedef struct IView *HVIEW;

/* Register the System Compositor. */
int RegisterCompositor(HDEVICE handle);

HVIEW CreateView(HDEVICE handle, int type, int width, int height);

int SetViewSurface(HVIEW view, HSURFACE surface);

int SetViewCursor(HVIEW view, HCURSOR cursor);

int GetViewSurfaceInfo(HVIEW view, SURFACEINFO *info);
int GetViewBounds(HVIEW view, RECT *bounds);

int SendViewMessage(HVIEW view, const TASKMSG *msg);

int MoveView(HVIEW view, int x, int y);

#ifdef __cplusplus
}
#endif
#endif

