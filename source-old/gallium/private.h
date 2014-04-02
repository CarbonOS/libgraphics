#ifndef __Gallium_Private_h__
#define __Gallium_Private_h__

/* Includes */
#include <stdio.h>
#include <stdint.h>
#include <pthread.h>
#include <state_tracker/drm_driver.h>
#include <pipe/p_screen.h>
#include <pipe/p_context.h>
#include <pipe/p_defines.h>
#include <pipe/p_state.h>
#include <tgsi/tgsi_build.h>
#include <tgsi/tgsi_text.h>
#include <util/u_inlines.h>
#include <util/u_memory.h>
#include <util/u_debug.h>
#include <util/u_draw.h>
#include "pipeline.h"

/* Gallium Device */
typedef struct __giDevice
{
	IDevice base;
	struct pipe_screen *screen;
	struct pipe_context *context;
	struct pipe_resource *buffer;
	struct pipe_resource *constant;
	Matrix4x4F transform;
	void *rs_state;
	void *bl_state;
	void *sp_state;
	void *vs_state;
	void *fs_color;
	void *fs_image;
	void *ve_state;
	int handle;

} GIDevice;

/* Gallium Context */
/*
typedef struct __giContext
{
	IContext base;
	struct pipe_context *context;
	struct pipe_resource *buffer;
	struct pipe_resource *constant;
	Matrix4x4F transform;
	void *rs_state;
	void *bl_state;
	void *sp_state;
	void *vs_state;
	void *fs_color;
	void *fs_image;
	void *ve_state;
	int handle;

} GIContext;
*/

/* Gallium Surface */
typedef struct __giSurface
{
	ISurface base;
	struct pipe_resource *resource;
	struct pipe_surface *surface;
	struct pipe_sampler_view *view;
	GIDevice *device;
	int handle;

} GISurface;

typedef struct _giEdge
{
	float x;
	float y;
	float r;
	float g;
	float b;
	float a;

} GIEdge;

typedef struct _giQuad
{
	GIEdge a;
	GIEdge b;
	GIEdge c;
	GIEdge d;

} GIQuad;

#endif
