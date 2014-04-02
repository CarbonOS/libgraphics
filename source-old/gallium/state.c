/* Creates the Static Vertex Elements State for the Device. */
static int galCreateVertexElementState(GIDevice *icontext)
{
	void *handle;
	struct pipe_vertex_element elements[2];
	memset(&elements, 0, sizeof(elements));
	elements[0].src_offset = 0;
	elements[0].src_format = PIPE_FORMAT_R32G32_FLOAT;
	elements[1].src_offset = 8;
	elements[1].src_format = PIPE_FORMAT_R32G32B32A32_FLOAT;

	icontext->ve_state = NULL;
	if ((handle = icontext->context->create_vertex_elements_state(icontext->context, 2, elements)) == NULL)
	{
		printf("create_vertex_elements_state failed!\n");
		return GI_ERROR;
	}

	icontext->context->bind_vertex_elements_state(icontext->context, handle);
	icontext->ve_state = handle;

	return GI_SUCCESS;
}

/* Creates the Static Blending State for the Device. */
static int galCreateBlendState(GIDevice *idevice)
{
	void *handle;
	struct pipe_blend_state blend;
	memset(&blend, 0, sizeof(blend));
	blend.rt[0].blend_enable = 1;
	blend.rt[0].colormask = PIPE_MASK_RGBA;
	blend.rt[0].rgb_func = PIPE_BLEND_ADD;
	blend.rt[0].rgb_src_factor = PIPE_BLENDFACTOR_SRC_ALPHA;
	blend.rt[0].rgb_dst_factor = PIPE_BLENDFACTOR_INV_SRC_ALPHA;
	blend.rt[0].alpha_func = PIPE_BLEND_ADD;
	blend.rt[0].alpha_src_factor = PIPE_BLENDFACTOR_ONE;
	blend.rt[0].alpha_dst_factor = PIPE_BLENDFACTOR_ONE;
	blend.independent_blend_enable = 0;

	idevice->bl_state = NULL;
	if ((handle = CreateBlendState(idevice->context, &blend)) == NULL)
	{
		printf("create_blend_state::failed!\n");
		return GI_ERROR;
	}

	SetBlendState(idevice->context, handle);
	idevice->bl_state = handle;
	return GI_SUCCESS;
}

/* Creates the Static Sampler State for the Device. */
static int galCreateSamplerState(GIDevice *idevice)
{
	void *handle;
	struct pipe_sampler_state sampler;
	memset(&sampler, 0, sizeof(sampler));
	sampler.wrap_s = PIPE_TEX_WRAP_CLAMP_TO_EDGE;
	sampler.wrap_t = PIPE_TEX_WRAP_CLAMP_TO_EDGE;
	sampler.wrap_r = PIPE_TEX_WRAP_REPEAT;
	sampler.min_mip_filter = PIPE_TEX_MIPFILTER_NONE;
	sampler.min_img_filter = PIPE_TEX_FILTER_LINEAR;
	sampler.mag_img_filter = PIPE_TEX_FILTER_LINEAR;
	sampler.compare_mode = PIPE_TEX_COMPARE_NONE;
	sampler.compare_func = PIPE_FUNC_ALWAYS;
	sampler.normalized_coords = 0;
	sampler.max_anisotropy = 0;

	idevice->sp_state = NULL;
	if ((handle = CreateSamplerState(idevice->context, &sampler)) == NULL)
	{
		printf("create_sampler_state::failed!\n");
		return GI_ERROR;
	}

	SetPixelShaderSamplers(idevice->context, 1, &handle);
	idevice->sp_state = handle;
	return GI_SUCCESS;
}



