#ifndef __GRAPHICS_PRIVATE__
#define __GRAPHICS_PRIVATE__

/* cpp Interface */
#ifdef __cplusplus
extern "C" {
#endif

#ifndef ADAPTER_DEVICES
#define ADAPTER_DEVICES "/dev/dri/card%d"
#endif

#ifndef ADAPTER_DRIVERS
#define ADAPTER_DRIVERS "/System/Graphics"
#endif

#define MAX_ADAPTERS 8

#define U64PTR(x) ((uint64_t)(unsigned long)(x))

//int AdapterListInit(void);
//int ExitAdapterList(void);
//int FramebufferAdapterInit(void);

/* */
int drmAdapterInit(IAdapter *pAdapter, int handle);

/* Only for Gallium based Adapters. */
int spGetPipeline(IAdapter *pAdapter, void **ppPipeline);

typedef int (*OpenAdapter)(IAdapter *pAdapter);

int LoadDriver(IAdapter *iadapter);

#ifdef __cplusplus
}
#endif

#endif
