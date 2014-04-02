#include <stdio.h>
#include <string.h>
#include <limits.h>
#include "graphics.h"
#include "device.h"
#include "private.h"

/* kslkjdl */
int MeasureText(FontPtr pFont, const char *text)
{
	return GI_SUCCESS;
}

/* Draws formatted text in the specified rectangle. */
int DrawText(DevicePtr pContext, FontPtr pFont, const char *text)
{
	IFont *font;
	if ((font = (IFont*)pFont) == NULL)
		return GI_ERROR;

	IDevice *icontext;
	if ((icontext = (IDevice*)pContext) == NULL)
		return GI_ERROR;

	if ((text == NULL) || (strlen(text) <= 0))
		return GI_ERROR;

	int index = 0;
	for (index; index < strlen(text); index++)
	{
		GetGlyph(font, text[index]);
	}

	return GI_SUCCESS;
}

