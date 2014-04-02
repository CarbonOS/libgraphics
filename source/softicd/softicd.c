#include <stdio.h>
#include <stdlib.h>
#include "graphics.h"
#include "device.h"
#include "private.h"
#include "softicd.h"
#include "rendering.c"
#include "context.c"
#include "effect.c"
#include "target.c"
#include "drawing.c"

/* Software Graphics Extension. */
static const char* GetVendorSP(IExtension *ext)
{
	static char *vendor = "software";
	return vendor;
}

/* Software Graphics Extension. */
static int CloseSP(IExtension *ext)
{
	return 1;
}

/* Software Graphics Extension. */
int SoftGraphicsICD(IExtension *ext)
{
	printf("soft:init\n");
	ext->CreateContext = CreateContextSP;
	ext->ReleaseContext = ReleaseContextSP;
	ext->SetClipping = SetClippingSP;
	ext->SetRenderTarget = SetRenderTargetSP;
	ext->FillRectangle = FillRectangleSP;
	ext->CreateRenderTarget = CreateRenderTargetSP;
	ext->CreateDisplayRenderTarget = CreateDisplayRenderTargetSP;
	ext->ReleaseRenderTarget = ReleaseRenderTargetSP;
	ext->CreateEffect = CreateEffectSP;
	ext->ReleaseEffect = ReleaseEffectSP;
	ext->GetVendor = GetVendorSP;
	ext->Close = CloseSP;
	return GI_SUCCESS;
}

