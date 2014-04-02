
int CreateDisplayRenderTarget(DevicePtr pDevice, DisplayPtr pDisplay, SurfacePtr *ppSurface)
{
	GIDevice *idevice = NULL;
	if ((idevice = (GIDevice*)pDevice) == NULL)
	{
		return GI_ERROR;
	}

	IDisplay *idisplay;
	if ((idisplay = (IDisplay*)pDisplay) == NULL)
	{
		return GI_ERROR;
	}

	struct drm_mode_crtc crtc = { 0, 0, idisplay->crtc };
	if (IoControl(idisplay->fd, DRM_IOCTL_MODE_GETCRTC, &crtc))
	{
		return GI_ERROR;
	}

	struct drm_mode_fb_cmd fbo = { crtc.fb_id, 0, 0 };
	if (IoControl(idisplay->fd, DRM_IOCTL_MODE_GETFB, &fbo))
	{
		return GI_ERROR;
	}

	struct drm_gem_flink req = { .handle = fbo.handle };
	if (IoControl(idisplay->fd, DRM_IOCTL_GEM_FLINK, &req))
	{
		return GI_ERROR;
	}

	struct pipe_screen *screen = NULL;
	if ((screen = idevice->screen) == NULL)
	{
		return GI_ERROR;
	}

	struct pipe_context *context = NULL;
	if ((context = idevice->context) == NULL)
	{
		return GI_ERROR;
	}


	if ((screen->resource_from_handle == NULL))
	{
		printf("resource_from_handle::Unimplemented!\n");
		return GI_UNIMPLEMENTED;
	}

	/* Create the Resource */
	struct pipe_resource rtempl;
	memset(&rtempl, 0, sizeof(rtempl));
	rtempl.target = PIPE_TEXTURE_RECT;
	rtempl.bind = PIPE_BIND_RENDER_TARGET|PIPE_BIND_DISPLAY_TARGET;
	rtempl.format = PIPE_FORMAT_B8G8R8X8_UNORM;
	rtempl.width0 = fbo.width;
	rtempl.height0 = fbo.height;
	rtempl.nr_samples = 1;
	rtempl.array_size = 1;
	rtempl.depth0 = 1;

	struct winsys_handle whandle;
	whandle.handle = req.name;
	whandle.stride = fbo.pitch;
	whandle.type = 0;

	struct pipe_resource *resource; // texture
	if ((resource = screen->resource_from_handle(screen, &rtempl, &whandle)) == NULL)
	{
		printf("resource_from_handle::Failed!\n");
		return GI_ERROR;
	}

	/* Create the Render Target */
	struct pipe_surface stempl;
	memset(&stempl, 0, sizeof(stempl));
	stempl.format = resource->format;
	stempl.usage = PIPE_BIND_RENDER_TARGET;
	stempl.u.tex.level = 0;
	stempl.u.tex.first_layer = 0;
	stempl.u.tex.last_layer = 0;

	struct pipe_surface *surface; // render target
	if ((surface = context->create_surface(context, resource, &stempl)) == NULL)
	{
		printf("create_surface::failed!\n");
		screen->resource_destroy(screen, resource);
		return GI_ERROR;
	}

	GISurface *isurface = NULL;
	if ((isurface = (GISurface*)calloc(1, sizeof(GISurface))) == NULL)
	{
		return GI_ERROR;
	}

	isurface->view = NULL;
	isurface->surface = surface;
	isurface->resource = resource;
	isurface->base.height = fbo.height;
	isurface->base.width = fbo.width;
	(*ppSurface) = &isurface->base;

	return GI_SUCCESS;
}

