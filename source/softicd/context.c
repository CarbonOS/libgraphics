/* Software Context */

/* Creates a new Software Context object. */
static IContext* CreateContextSP(IExtension *ext)
{
	IContextSP *context = NULL;
	if (!(context = (IContextSP*)calloc(1, sizeof(IContextSP))))
	{
		return NULL;
	}

	context->threads = 4;
	if (RenderingInit(context) <= 0) {
		free(context);
		return NULL;
	}

	context->target = NULL;
	context->base.enable_clip = 0;
	context->base.ext = ext;
	return (&context->base);
}

static int SetClippingSP(IContext *context, RECT *clip)
{
	IContextSP *ctxsp = NULL;
	if (!(ctxsp = (IContextSP*)context)) {
		return 0;
	}

	ctxsp->clip.x = clip->x;
	ctxsp->clip.y = clip->y;
	ctxsp->clip.width = clip->width;
	ctxsp->clip.height = clip->height;
	return 1;
}

static int SetRenderTargetSP(IContext *handle, IRenderTarget *target)
{
	IContextSP *context = NULL;
	if (!(context = (IContextSP*)handle)) {
		return 0;
	}

	IRenderTargetSP *targetsp = NULL;
	if (!(targetsp = (IRenderTargetSP*)target)) {
		return 0;
	}

	context->target = targetsp;
	return 1;
}

/* Destroys a Software Context object. */
static int ReleaseContextSP(IContext *context)
{
	IContextSP *contextsp = NULL;
	if (!(contextsp = (IContextSP*)context))
	{
		return 0;
	}

	free(contextsp);
	return 1;
}

