/* Static Context Vertex Buffer */

/* Creates the Static Vertex Buffer for the Device. */
static int galCreateStaticVertexBuffer(GIDevice *idevice)
{
	struct pipe_resource buffer;
	memset(&buffer, 0, sizeof(buffer));
	buffer.target = PIPE_BUFFER;
	buffer.format = PIPE_FORMAT_R8_UNORM;
	buffer.bind = PIPE_BIND_VERTEX_BUFFER;
	buffer.usage = PIPE_USAGE_STATIC;
	buffer.width0 = (8 * 256);
	buffer.height0 = 1;
	buffer.depth0 = 1;
	buffer.array_size = 1;
	buffer.flags = 0;

	idevice->buffer = NULL;
	struct pipe_resource *handle;
	if ((handle = idevice->screen->resource_create(idevice->screen, &buffer)) == NULL)
	{
		printf("resource_create failed!\n");
		return GI_ERROR;
	}

	struct pipe_vertex_buffer vbuffer;
	memset(&vbuffer, 0, sizeof(vbuffer));
	vbuffer.stride = (24);
	vbuffer.buffer_offset = 0;
	vbuffer.buffer = handle;

	idevice->context->set_vertex_buffers(idevice->context, 1, &vbuffer);
	idevice->buffer = handle;
	return GI_SUCCESS;
}

/* Creates the Static Constant Buffer for the Device. */
static int galCreateStaticConstantBuffer(GIDevice *idevice)
{
	struct pipe_resource buffer;
	memset(&buffer, 0, sizeof(buffer));
	buffer.target = PIPE_BUFFER;
	buffer.format = PIPE_FORMAT_R8_UNORM;
	buffer.bind = PIPE_BIND_CONSTANT_BUFFER;
	buffer.usage = PIPE_USAGE_DYNAMIC;
	buffer.width0 = (4 * 128);
	buffer.height0 = 1;
	buffer.depth0 = 1;
	buffer.array_size = 1;
	buffer.flags = 0;

	idevice->constant = NULL;
	struct pipe_resource *handle;
	if ((handle = idevice->screen->resource_create(idevice->screen, &buffer)) == NULL)
	{
		printf("resource_create failed!\n");
		return GI_ERROR;
	}

	pipe_set_constant_buffer(idevice->context, PIPE_SHADER_VERTEX, 0, handle);
	pipe_set_constant_buffer(idevice->context, PIPE_SHADER_FRAGMENT, 0, handle);
	idevice->constant = handle;

	return GI_SUCCESS;
}

/* --------------- */
static inline int SetConstantColor(GIDevice *icontext, float r, float g, float b, float a)
{
	if ((icontext->constant == NULL))
	{
		return GI_ERROR;
	}

	Color4F color;
	color.r = r;
	color.g = g;
	color.b = b;
	color.a = a;

	struct pipe_box box;
	box.x = 4;
	box.y = 0;
	box.z = 0;
	box.width = sizeof(color);
	box.height = 1;
	box.depth = 1;

   
	unsigned usage = PIPE_TRANSFER_WRITE;
	pipe_buffer_write(icontext->context, icontext->constant, 64, sizeof(color), &color);
	icontext->context->transfer_inline_write(icontext->context, icontext->constant, 0, usage, &box, (void*)&color, box.width, 0);

	return GI_SUCCESS;
}


