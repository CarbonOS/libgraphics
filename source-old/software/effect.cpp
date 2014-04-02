#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "format.h"
#include "graphics.h"
#include "device.h"
#include "software.h"
#include "private.h"
#include "utils.h"

/* Summary */
/* Creates an effect with source code. */
int swCreateEffect(IContext *pContext, IEffect **ppEffect, const char *source)
{
	if ((pContext) || (ppEffect))
		return GI_ERROR;

	if ((source == NULL) || (strlen(source) <= 0))
		return GI_ERROR;

	SWContext *context = NULL;
	if ((context = (SWContext*)pContext) == NULL)
		return GI_ERROR;

	SWEffect *effect = NULL;
	if ((effect = (SWEffect*)malloc(sizeof(SWEffect))) == NULL)
		return GI_ERROR;

	memset(effect, 0, sizeof(SWEffect));
	(*ppEffect) = &effect->base;
	return GI_SUCCESS;
}

/* Destroys the effect. */
int swDestroyEffect(IEffect *pEffect)
{
	SWEffect *effect = NULL;
	if ((effect = (SWEffect*)pEffect) == NULL)
		return GI_ERROR;

	free(effect);

	return GI_SUCCESS;
}

