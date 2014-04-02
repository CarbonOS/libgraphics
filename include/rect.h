#ifndef __GRAPHICS_RECT_H__
#define __GRAPHICS_RECT_H__

/* C++ Interface */
#ifdef __cplusplus
extern "C" {
#endif

/* Rect Object */
typedef struct __rect
{
	int x;
	int y;
	int width;
	int height;

} RECT;

#define RMAX(a, b) (a > b ? a : b)
#define RMIN(a, b) (a < b ? a : b)

static inline void RECT_EMPTY(RECT *dest)
{
	if ((!dest)) {
		return;
	}

	dest->x = 0;
	dest->y = 0;
	dest->width = 0;
	dest->height = 0;
	return;
}

static inline void RECT_INIT(RECT *dest, int left, int top, int width, int height)
{
	if ((!dest)) {
		return;
	}

	dest->x = (left);
	dest->y = (top);
	dest->width = (width);
	dest->height = (height);
	return;
}

/* Indicates if the width or height are zero. Read only. */
static inline int RECT_ISEMPTY(const RECT *rect)
{
	if (!rect) {
		return 1;
	}
	return ((rect->x == 0) && (rect->y == 0) && (rect->width == 0) && (rect->height == 0));
}

/* Produces a Rectangle structure from */
/* left, top, right, and bottom coordinates. */
static inline int RECT_FROMLTRB(RECT *dest,
	int left, int top, int right, int bottom)
{
	if ((!dest)) {
		return 0;
	}

	dest->x = (left);
	dest->y = (top);
	dest->width = (right - left);
	dest->height = (bottom - top);
	return 1;
}

/* Produces a new Rectangle from the union of 2 existing. */
static inline int RECT_UNION(RECT *dest,
	const RECT *r1, const RECT *r2)
{
	if ((!dest) || (!r1) || (!r2)) {
		return 0;
	}

	int l = RMIN(r1->x, r2->x);
	int t = RMIN(r1->y, r2->y);
	int r = RMAX(r1->x + r1->width, r2->x + r2->width);
	int b = RMAX(r1->y + r1->height, r2->y + r2->height);
	return RECT_FROMLTRB(dest, l, t, r, b);
}

/* Produces a new Rectangle by intersecting 2 existing Rectangles. */ 
static inline int RECT_Intersect(RECT *dest,
	const RECT *r1, const RECT *r2)
{
	if ((!dest) || (!r1) || (!r2)) {
		return 0;
	}

	int l = RMAX(r1->x, r2->x);
	int t = RMAX(r1->y, r2->y);
	int r = RMIN(r1->x + r1->width, r2->x + r2->width);
	int b = RMIN(r1->y + r1->height, r2->y + r2->height);
	return RECT_FROMLTRB(dest, l, t, r, b);
}

static inline int RECT_contains(const RECT *src, int left, int top)
{
	int x = (src->x);
	int y = (src->y);
	int r = (x + src->width);
	int b = (y + src->height);
	int l = (left);
	int t = (top);

	return ((l >= x) && (l < r) && (t >= y) && (t < b));
}

/* Inflates the Rectangle by a specified Size. */
static inline int RECT_inflate(RECT *dest, int size)
{
	dest->x -= size;
	dest->y -= size;
	dest->width += (size * 2);
	dest->height += (size * 2);
	return 1;
}

/* Checks if a Rectangle intersects with this one. */
static inline int RECT_intersects(RECT *a, RECT *b)
{
	if ((!a) || (!b)) {
		return 0;
	}

	return 1;
}

#ifdef __cplusplus
}
#endif
#endif

