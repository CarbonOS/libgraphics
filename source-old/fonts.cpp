#include <stdio.h>
#include <limits.h>
#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_OUTLINE_H
#include FT_SIZES_H
#include FT_TRUETYPE_TABLES_H
#include FT_TYPE1_TABLES_H
#include FT_BITMAP_H
#include <freetype/ftglyph.h>
#include "graphics.h"
#include "device.h"
#include "private.h"
#include <vector>

/* Locals */
static int ft_init = 0;
static FT_Library gLibrary;
static std::vector<FT_Face> font_list;

/* Freetype Library Init. */
bool InitFreetype(void)
{
	if ((ft_init > 0))
		return true;
	if (FT_Init_FreeType(&gLibrary))
		return false;
	ft_init = 1;
	return true;
}

/* Creates a new Font object. */
int CreateFont2D(FontPtr *ppFont, const char *filename)
{
	FT_Face face;
	if (!InitFreetype())
		return GI_ERROR;

	if ((ppFont == NULL) || (filename == NULL))
		return GI_ERROR;

	if (FT_New_Face(gLibrary, filename, 0, &face))
		return GI_ERROR;

	struct IFont2D *font = NULL;
	if ((font = (struct IFont2D*)calloc(1, sizeof(struct IFont2D))) == NULL)
		return GI_ERROR;

	font->type = 1;
	font->handle = (void*)face;
	FT_Set_Char_Size(face, 16 << 6,16 << 6, 96, 96);
	(*ppFont) = (FontPtr)font;

	return GI_SUCCESS;
}

/* */
int GetGlyph(struct IFont2D *font, char letter)
{
	FT_Face face;
	FT_UInt index;
	FT_Glyph glyph;
	FT_Outline outline;
	short startIndex = 0;
	short endIndex = 0;

	if ((face = (FT_Face)font->handle) == NULL)
		return GI_ERROR;
 
	index = FT_Get_Char_Index(face, letter);
	if (FT_Load_Glyph(face, index, FT_LOAD_DEFAULT))
		return GI_ERROR;

	outline = face->glyph->outline;
	if ((outline.n_points == 0))
		return GI_ERROR;

	for (int i = 0; i < outline.n_contours; i++)
	{

		endIndex = outline.contours[i];
//		contourLength =  (endIndex - startIndex) + 1;
		startIndex = endIndex + 1;
	}

	printf("points:%d\n", outline.n_points);
	printf("contours:%d\n", outline.n_contours);
	return GI_SUCCESS;
}




