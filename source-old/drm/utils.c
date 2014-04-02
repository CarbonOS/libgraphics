#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <xf86drm.h>
#include <xf86drmMode.h>
#include <graphics.h>

int drmGetGlobalHandle(int fd, uint32_t crtc_id)
{
	struct drm_mode_crtc crtc;
	struct drm_mode_fb_cmd fbo;
	struct drm_gem_flink flink;

	crtc.crtc_id = crtc_id;
	if (drmIoctl(fd, DRM_IOCTL_MODE_GETCRTC, &crtc))
		return GI_ERROR;

	fbo.fb_id = crtc.fb_id;
	if (drmIoctl(fd, DRM_IOCTL_MODE_GETFB, &fbo))
		return GI_ERROR;

	flink.handle = fbo.handle;
	if (drmIoctl(fd, DRM_IOCTL_GEM_FLINK, &flink))
		return GI_ERROR;

	flink.name;
	return GI_SUCCESS;
}

/* Retrieve the adapter display information. (DRM Version) */
int drmGetDisplayInfo(int fd, uint32_t crtc_id, DisplayInfo *info)
{
	struct drm_mode_crtc crtc;
	struct drm_mode_fb_cmd fbo;

	crtc.crtc_id = crtc_id;
	if (drmIoctl(fd, DRM_IOCTL_MODE_GETCRTC, &crtc))
		return GI_ERROR;

	fbo.fb_id = crtc.fb_id;
	if (drmIoctl(fd, DRM_IOCTL_MODE_GETFB, &fbo))
		return GI_ERROR;

	info->x = (int)crtc.x;
	info->y = (int)crtc.y;
	info->width = (int)fbo.width;
	info->height = (int)fbo.height;
	info->pitch = (int)fbo.pitch;
	info->handle = (int)fbo.handle;
	info->depth = (int)fbo.depth;
	info->bpp = (int)fbo.bpp;
	info->id = (int)0;

	return GI_SUCCESS;
}


