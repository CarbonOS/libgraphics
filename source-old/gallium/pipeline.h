#ifndef __GRAPHICS_PIPELINE__
#define __GRAPHICS_PIPELINE__

/* Gallium Pipeline Macros */

#define ICreateContext(screen, data) \
	((screen)->context_create(screen, data))

/* Create a rasterizer state object that tells the rasterizer stage how to behave. */
#define CreateRasterizerState(context, state) \
	((context)->create_rasterizer_state(context, state))

/* Create a blend-state object that encapsules blend state for the output-merger stage. */
#define CreateBlendState(context, state) \
	((context)->create_blend_state(context, state))

#define SetBlendState(context, handle) \
	((context)->bind_blend_state(context, handle))


#define IContextDestroyBlendState(context, handle) \
	((context)->delete_blend_state(context, handle))

#define IContextCreateDepthStencilAlphaState(context, state) \
	((context)->create_depth_stencil_alpha_state(context, state))

/* Create a sampler-state object that encapsulates sampling information for a context. */
#define CreateSamplerState(context, state) \
	((context)->create_sampler_state(context, state))

#define SetSamplerState(context, handle) \
	((context)->bind_fragment_sampler_states(context, handle))


/* Sets a vertex shader to the context. */
#define SetVertexShader(context, handle) \
	((context)->bind_vs_state(context, handle))


/* Sets a pixel shader to the context. */
#define SetPixelShader(context, handle) \
	((context)->bind_fs_state(context, handle))

/* Set an array of sampler states to the pixel shader pipeline stage. */
#define SetPixelShaderSamplers(context, count, array) \
	((context)->bind_fragment_sampler_states(context, count, array))

#define SetPixelShaderSamplerViews(context, count, array) \
	((context)->set_fragment_sampler_views(context, count, array))




#define IContextBindDepthStencilAlphaState(context, state) \
	((context)->bind_depth_stencil_alpha_state(context, state))

#define IContextSetFramebufferState(context, state) \
	((context)->set_framebuffer_state(context, state))

#define IContextFlush(context, fence) \
	((context)->flush(context, fence))

#define IContextDestroy(context) \
	((context)->destroy(context))

#define IResourceCreate(screen, templat) \
	((screen)->resource_create(screen, templat))

#define IResourceDestroy(screen, resource) \
	((screen)->resource_destroy(screen, resource))

#define ISurfaceCreate(context, resource, templat) \
	((context)->create_surface(context, resource, templat))

#define ISurfaceDestroy(context, surface) \
	((context)->surface_destroy(context, surface))

#endif
