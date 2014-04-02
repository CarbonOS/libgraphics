#include <stdio.h>
#include <string.h>
#include "graphics.h"
#include "private.h"
#include <math.h>

/* */
extern "C" int MatrixIdentity(Matrix4x4F *matrix)
{
	if ((matrix == NULL))
	{
		return GI_ERROR;
	}

	matrix->m11 = 1.0f; // M11
	matrix->m12 = 0.0f; // M12
	matrix->m13 = 0.0f; // M13
	matrix->m14 = 0.0f; // M14
	matrix->m21 = 0.0f; // M21
	matrix->m22 = 1.0f; // M22
	matrix->m23 = 0.0f; // M23
	matrix->m24 = 0.0f; // M24
	matrix->m31 = 0.0f; // M31
	matrix->m32 = 0.0f; // M32
	matrix->m33 = 1.0f; // M33
	matrix->m34 = 0.0f; // M34
	matrix->m41 = 0.0f; // M41
	matrix->m42 = 0.0f; // M42
	matrix->m43 = 0.0f; // M43
	matrix->m44 = 1.0f; // M44

	return GI_SUCCESS;
}

extern "C" int MatrixSetCenter(Matrix4x4F *matrix, float width, float height)
{
	if ((matrix == NULL))
	{
		return GI_ERROR;
	}

	Matrix4x4F temp;
	MatrixIdentity(&temp);
	temp.m41 = (width / 2.0f);
	temp.m42 = (height / 2.0f);
	MatrixMultiply(matrix, &temp);

	return GI_SUCCESS;
}

extern "C" int MatrixSetPerspective(Matrix4x4F *matrix, float depth)
{
	if ((matrix == NULL))
		return GI_ERROR;

	Matrix4x4F temp;
//	MatrixIdentity(&temp);


        temp.m11 = 1.0f;
        temp.m12 = 0.0f;
        temp.m13 = 0.0f;
        temp.m14 = 0.0f;
        temp.m21 = 0.0f;
        temp.m22 = 1.0f;
        temp.m23 = 0.0f;
        temp.m24 = 0.0f;
	temp.m31 = 0.0f;
	temp.m32 = 0.0f;
	temp.m33 = 1.0f;
	temp.m34 = -1.0f / depth;
	temp.m41 = 0.0f; // M41
	temp.m42 = 0.0f; // M42
	temp.m43 = 0.0f; // M43
	temp.m44 = 1.0f; // M44

	MatrixMultiply(matrix, &temp);

	return GI_SUCCESS;
}

extern "C" int MatrixRotateX(Matrix4x4F *matrix, float angle)
{
	if ((matrix == NULL))
	{
		return GI_ERROR;
	}

	Matrix4x4F temp;
	MatrixIdentity(&temp);
	double rad = (angle * 0.017453292519943295769236907684886);
	temp.m22 = (float)cos(rad);
	temp.m23 = (float)sin(rad);
	temp.m32 = -temp.m23;
	temp.m33 = temp.m22;
	MatrixMultiply(matrix, &temp);

	return GI_SUCCESS;
}

extern "C" int MatrixRotateY(Matrix4x4F *matrix, float angle)
{
	if ((matrix == NULL))
		return GI_ERROR;

	Matrix4x4F temp;
	MatrixIdentity(&temp);
        double rad = (angle * 0.017453292519943295769236907684886);
        temp.m11 = (float)cos(rad);
        temp.m31 = (float)sin(rad);
        temp.m13 = -temp.m31;
        temp.m33 = temp.m11;
	MatrixMultiply(matrix, &temp);

	return GI_SUCCESS;
}

extern "C" int MatrixRotateZ(Matrix4x4F *matrix, float angle)
{
	if ((matrix == NULL))
		return GI_ERROR;

	Matrix4x4F temp;
	MatrixIdentity(&temp);
	double rad = (angle * 0.017453292519943295769236907684886);
        temp.m11 = (float)cos(rad);
        temp.m12 = (float)sin(rad);
        temp.m21 = -temp.m12;
        temp.m22 = temp.m11;
	MatrixMultiply(matrix, &temp);

	return GI_SUCCESS;
}

extern "C" int MatrixTransformPoint2F(Matrix4x4F *matrix, Point2F *point)
{
	if ((matrix == NULL) || (point == NULL))
	{
		return GI_ERROR;
	}

	float temp[4];

	temp[0] = ((point->x * matrix->m11) + (point->y * matrix->m21) /*+ (0.0f * matrix->m31)*/ + (/*1.0f **/ matrix->m41));
	temp[1] = ((point->x * matrix->m12) + (point->y * matrix->m22) /*+ (0.0f * matrix->m32)*/ + (/*1.0f **/ matrix->m42));
	temp[2] = ((point->x * matrix->m13) + (point->y * matrix->m23) /*+ (0.0f * matrix->m33)*/ + (/*1.0f **/ matrix->m43));
	temp[3] = ((point->x * matrix->m14) + (point->y * matrix->m24) /*+ (0.0f * matrix->m34)*/ + (/*1.0f **/ matrix->m44));

       temp[0] /= temp[3];
       temp[1] /= temp[3];
       temp[2] /= temp[3];

	point->x = temp[0];
	point->y = temp[1];

	return GI_SUCCESS;
}

extern "C" int MatrixMultiply(Matrix4x4F *matrix1, Matrix4x4F *matrix2)
{
	if ((matrix1 == NULL) || (matrix2 == NULL))
		return GI_ERROR;

	Matrix4x4F temp;
	temp.m11 = (((matrix1->m11 * matrix2->m11) + (matrix1->m12 * matrix2->m21)) + (matrix1->m13 * matrix2->m31)) + (matrix1->m14 * matrix2->m41);
	temp.m12 = (((matrix1->m11 * matrix2->m12) + (matrix1->m12 * matrix2->m22)) + (matrix1->m13 * matrix2->m32)) + (matrix1->m14 * matrix2->m42);
	temp.m13 = (((matrix1->m11 * matrix2->m13) + (matrix1->m12 * matrix2->m23)) + (matrix1->m13 * matrix2->m33)) + (matrix1->m14 * matrix2->m43);
	temp.m14 = (((matrix1->m11 * matrix2->m14) + (matrix1->m12 * matrix2->m24)) + (matrix1->m13 * matrix2->m34)) + (matrix1->m14 * matrix2->m44);
	temp.m21 = (((matrix1->m21 * matrix2->m11) + (matrix1->m22 * matrix2->m21)) + (matrix1->m23 * matrix2->m31)) + (matrix1->m24 * matrix2->m41);
	temp.m22 = (((matrix1->m21 * matrix2->m12) + (matrix1->m22 * matrix2->m22)) + (matrix1->m23 * matrix2->m32)) + (matrix1->m24 * matrix2->m42);
	temp.m23 = (((matrix1->m21 * matrix2->m13) + (matrix1->m22 * matrix2->m23)) + (matrix1->m23 * matrix2->m33)) + (matrix1->m24 * matrix2->m43);
	temp.m24 = (((matrix1->m21 * matrix2->m14) + (matrix1->m22 * matrix2->m24)) + (matrix1->m23 * matrix2->m34)) + (matrix1->m24 * matrix2->m44);
	temp.m31 = (((matrix1->m31 * matrix2->m11) + (matrix1->m32 * matrix2->m21)) + (matrix1->m33 * matrix2->m31)) + (matrix1->m34 * matrix2->m41);
	temp.m32 = (((matrix1->m31 * matrix2->m12) + (matrix1->m32 * matrix2->m22)) + (matrix1->m33 * matrix2->m32)) + (matrix1->m34 * matrix2->m42);
	temp.m33 = (((matrix1->m31 * matrix2->m13) + (matrix1->m32 * matrix2->m23)) + (matrix1->m33 * matrix2->m33)) + (matrix1->m34 * matrix2->m43);
	temp.m34 = (((matrix1->m31 * matrix2->m14) + (matrix1->m32 * matrix2->m24)) + (matrix1->m33 * matrix2->m34)) + (matrix1->m34 * matrix2->m44);
	temp.m41 = (((matrix1->m41 * matrix2->m11) + (matrix1->m42 * matrix2->m21)) + (matrix1->m43 * matrix2->m31)) + (matrix1->m44 * matrix2->m41);
	temp.m42 = (((matrix1->m41 * matrix2->m12) + (matrix1->m42 * matrix2->m22)) + (matrix1->m43 * matrix2->m32)) + (matrix1->m44 * matrix2->m42);
	temp.m43 = (((matrix1->m41 * matrix2->m13) + (matrix1->m42 * matrix2->m23)) + (matrix1->m43 * matrix2->m33)) + (matrix1->m44 * matrix2->m43);
	temp.m44 = (((matrix1->m41 * matrix2->m14) + (matrix1->m42 * matrix2->m24)) + (matrix1->m43 * matrix2->m34)) + (matrix1->m44 * matrix2->m44);
	memcpy(matrix1, &temp, sizeof(temp));	
	return GI_SUCCESS;
}





