#ifndef __DRM_ADAPTER_PRIVATE__
#define __DRM_ADAPTER_PRIVATE__

/* DRM Private Display */
typedef struct __drmDisplay
{
	IDisplay base;
	IAdapter *adapter;
	uint32_t connector_id;
	uint32_t crtc_id;
	int handle;

} DRMDisplay;

#define U64PTR(x) ((uint64_t)(unsigned long)(x))

#endif

