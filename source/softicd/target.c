/* Software RenderTarget */

/* Creates a new RenderTarget object. */
static IRenderTarget* CreateRenderTargetSP(IExtension *ext, unsigned int width, unsigned int height, unsigned int format)
{
	assert((width > 0));
	assert((height > 0));
	assert((ext != NULL));

	struct drm_get_cap cap;
	memset(&cap, 0, sizeof(cap));	
	cap.capability = DRM_CAP_DUMB_BUFFER;
	if (ioctl(ext->fd, DRM_IOCTL_GET_CAP, &cap) || (!cap.value))
	{
		return NULL;
	}

	struct drm_mode_create_dumb args;
	memset(&args, 0, sizeof(args));	
	args.width = width;
	args.height = height;
	args.bpp = 32;
	if (ioctl(ext->fd, DRM_IOCTL_MODE_CREATE_DUMB, &args))
	{
		return NULL;
	}

	IRenderTargetSP *target = NULL;
	if (!(target = (IRenderTargetSP*)calloc(1, sizeof(IRenderTargetSP))))
	{
		return NULL;
	}

	target->fd = ext->fd;
	target->base.bpp = args.bpp;
	target->base.height = args.height;
	target->base.width = args.width;
	target->base.stride = args.pitch;
	target->base.handle = args.handle;
	target->base.size = args.size;
	target->base.ext = ext;
	return (&target->base);
}

/* Creates a new Display based RenderTarget object. */
static IRenderTarget* CreateDisplayRenderTargetSP(IExtension *ext, IDisplay *display)
{
	assert((ext != NULL));
	assert((display != NULL));

	struct drm_mode_crtc crtc;
	memset(&crtc, 0, sizeof(crtc));
	crtc.crtc_id = display->crtc;
	if (ioctl(ext->fd, DRM_IOCTL_MODE_GETCRTC, &crtc))
	{
		return NULL;
	}

	struct drm_mode_fb_cmd fbo;
	memset(&fbo, 0, sizeof(fbo));
	fbo.fb_id = crtc.fb_id;
	if (ioctl(ext->fd, DRM_IOCTL_MODE_GETFB, &fbo))
	{
		return NULL;
	}

	IRenderTargetSP *target = NULL;
	if (!(target = (IRenderTargetSP*)calloc(1, sizeof(IRenderTargetSP))))
	{
		return NULL;
	}

	target->fd = ext->fd;
	target->base.bpp = fbo.bpp;
	target->base.width = fbo.width;
	target->base.height = fbo.height;
	target->base.stride = fbo.pitch;
	target->base.handle = fbo.handle;
	target->base.size = (fbo.pitch * fbo.height);
	target->base.ext = ext;
	return (&target->base);
}

static void* MapRenderTargetSP(IRenderTarget *handle)
{
	IRenderTargetSP *target = NULL;
	if (!(target = (IRenderTargetSP*)handle))
	{
		return NULL;
	}

	struct drm_mode_map_dumb req;
	memset(&req, 0, sizeof(req));
	req.handle = target->base.handle;
	if (ioctl(target->fd, DRM_IOCTL_MODE_MAP_DUMB, &req))
	{
		return NULL;
	}

	void *ptr = NULL;
	if (!(ptr = mmap64(NULL, target->base.size, PROT_READ|PROT_WRITE, MAP_SHARED, target->fd, req.offset)))
	{
		return NULL;
	}

	return ptr;
}

static int ReleaseRenderTargetSP(IRenderTarget *target)
{
	IRenderTarget *targetsp = NULL;
	if (!(targetsp = (IRenderTarget*)target))
	{
		return 0;
	}

	free(targetsp);
	return 1;
}

