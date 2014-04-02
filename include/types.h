#ifndef __GRAPHIC_TYPES__
#define __GRAPHIC_TYPES__

/* C++ Interface */
#ifdef __cplusplus
extern "C" {
#endif

/* PointF Object */
/* A 2D point that consists of two */
/* single-precision floating-point values (x, y). */
typedef struct IPointF
{
	float x;
	float y;

} POINTF __attribute__ ((aligned(16)));

/* RectF Object */
/* Represents a rectangle defined by */
/* the coordinates */
typedef struct IRectF
{
	float x;
	float y;
	float width;
	float height;

} RECTF __attribute__ ((aligned(32)));

/* Color Object */
/* Describes the red, green, blue, */
/* and alpha components of a color, */
/* expressed as floating-point values */
typedef struct __COLOR
{
	float r;
	float g;
	float b;
	float a;

} COLOR __attribute__ ((aligned(32)));


/* Vector4F Object */
/* Describes the red, green, blue, */
/* and alpha components of a color, */
/* expressed as floating-point values */
typedef struct __Vector4F
{
	float x;
	float y;
	float z;
	float w;

} Vector4F __attribute__ ((aligned(32)));

/* Matrix Object */
typedef struct IMatrix4x4F
{
	float m11;
	float m12;
	float m13;
	float m14;
	float m21;
	float m22;
	float m23;
	float m24;
	float m31;
	float m32;
	float m33;
	float m34;
	float m41;
	float m42;
	float m43;
	float m44;

} MATRIX4X4;


#ifdef __cplusplus
}
#endif

#endif
