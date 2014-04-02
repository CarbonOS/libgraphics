/* Static Device Drawing Source */

/* Draws a line connecting two Poin2F structures. */
static int galDrawLine(IDevice *pContext, Point2F a, Point2F b)
{
	GIDevice *icontext = NULL;
	if ((icontext = (GIDevice*)pContext) == NULL)
	{
		return GI_ERROR;
	}

	struct pipe_context *context;
	if ((context = icontext->context) == NULL)
	{
		return GI_ERROR;
	}

	GIEdge edge[2];

	edge[0].x = a.x;
	edge[0].y = a.y;
	edge[0].r = 1.0f;
	edge[0].g = 1.0f;
	edge[0].b = 1.0f;
	edge[0].a = 1.0f;

	edge[1].x = b.x;
	edge[1].y = b.y;
	edge[1].r = 1.0f;
	edge[1].g = 1.0f;
	edge[1].b = 1.0f;
	edge[1].a = 0.4f;



	pipe_buffer_write(context, icontext->buffer, 0, sizeof(edge), edge);


	struct pipe_draw_info info;
	memset(&info, 0, sizeof(info));
	info.instance_count = 1;
	info.max_index = 0xffffffff;
	info.mode = PIPE_PRIM_LINES;
	info.start = 0;
	info.count = 2;
	info.min_index = 0;
	info.max_index = 0 + 2 - 1;

	SetPixelShader(context, icontext->fs_color);

	context->draw_vbo(context, &info);

	SetPixelShader(context, NULL);

	return GI_SUCCESS;
}

/* Draws a rectangle specified by a Rect4F structure. */
static int galDrawRectangle(IDevice *pContext, Rect4F bounds)
{
	GIDevice *icontext = NULL;
	if ((icontext = (GIDevice*)pContext) == NULL)
	{
		return GI_ERROR;
	}

	struct pipe_context *context;
	if ((context = icontext->context) == NULL)
	{
		return GI_ERROR;
	}

	GIEdge edge[4];
	edge[0].x = (bounds.x);
	edge[0].y = (bounds.y);
	edge[0].r = 1.0f;
	edge[0].g = 0.0f;
	edge[0].b = 0.0f;
	edge[0].a = 1.0f;

	edge[1].x = (bounds.x + bounds.width);
	edge[1].y = (bounds.y);
	edge[1].r = 0.0f;
	edge[1].g = 1.0f;
	edge[1].b = 0.0f;
	edge[1].a = 0.6f;

	edge[2].x = (bounds.x + bounds.width);
	edge[2].y = (bounds.y + bounds.height);
	edge[2].r = 0.0f;
	edge[2].g = 0.0f;
	edge[2].b = 1.0f;
	edge[2].a = 0.6f;

	edge[3].x = (bounds.x);
	edge[3].y = (bounds.y + bounds.height);
	edge[3].r = 1.0f;
	edge[3].g = 0.0f;
	edge[3].b = 0.0f;
	edge[3].a = 0.6f;


	pipe_buffer_write(context, icontext->buffer, 0, sizeof(edge), edge);

	struct pipe_draw_info info;
	memset(&info, 0, sizeof(info));
	info.instance_count = 1;
	info.max_index = 0xffffffff;
	info.mode = PIPE_PRIM_QUADS;
	info.start = 0;
	info.count = 4;
	info.min_index = 0;
	info.max_index = 0 + 2 - 1;

	SetPixelShader(context, icontext->fs_color);

	context->draw_vbo(context, &info);

	SetPixelShader(context, NULL);

	return GI_SUCCESS;
}

static int galDrawImage(IDevice *pContext, ISurface *pSurface, unsigned x, unsigned y, unsigned sx, unsigned sy, unsigned width, unsigned height)
{
	GIDevice *icontext = NULL;
	if ((icontext = (GIDevice*)pContext) == NULL)
	{
		return GI_ERROR;
	}

	GISurface *isurface = NULL;
	if ((isurface = (GISurface*)pSurface) == NULL)
	{
		return GI_ERROR;
	}

	struct pipe_context *context;
	if ((context = icontext->context) == NULL)
	{
		return GI_ERROR;
	}

	struct pipe_sampler_view *view;
	if ((view = isurface->view) == NULL)
	{
		return GI_ERROR;
	}

	GIEdge edge[4];
	edge[0].x = x;
	edge[0].y = y;
	edge[0].r = 0.0f;
	edge[0].g = 0.0f;
	edge[0].b = 0.0f;
	edge[0].a = 0.0f;

	edge[1].x = (x + width);
	edge[1].y = (y);
	edge[1].r = 0.0f;
	edge[1].g = 0.0f;
	edge[1].b = 0.0f;
	edge[1].a = 0.0f;

	edge[2].x = (x + width);
	edge[2].y = (y + height);
	edge[2].r = 0.0f;
	edge[2].g = 0.0f;
	edge[2].b = 0.0f;
	edge[2].a = 0.0f;

	edge[3].x = (x);
	edge[3].y = (y + height);
	edge[3].r = 0.0f;
	edge[3].g = 0.0f;
	edge[3].b = 0.0f;
	edge[3].a = 0.0f;


	pipe_buffer_write(context, icontext->buffer, 0, sizeof(edge), edge);

	struct pipe_draw_info info;
	memset(&info, 0, sizeof(info));
	info.instance_count = 1;
	info.max_index = 0xffffffff;
	info.mode = PIPE_PRIM_QUADS;
	info.start = 0;
	info.count = 4;
	info.min_index = 0;
	info.max_index = 0 + 2 - 1;

	SetPixelShader(context, icontext->fs_image);

	context->set_fragment_sampler_views(context, 1, &view);

	context->draw_vbo(context, &info);

	SetPixelShader(context, NULL);

	return GI_SUCCESS;
}



