#include <stdio.h>
#include <string.h>
#include <graphics/graphics.h>


int main (int argc, char *argv[])
{
	Matrix4x4F m;

	Point2F pos;
	pos.x = 400;
	pos.y = 200;

	MatrixIdentity(&m);

//	MatrixRotateX(&m, 45.0f);

//	MatrixRotateY(&m, 45.0f);

	MatrixTransformPoint2F(&m, &pos);

        float w = 400;
        float h = 400;

        float x = (pos.x / w /** (1.0f)*/);
        float y = (pos.y / h /** (1.0f)*/);

	printf("%f, %f\n", pos.x, pos.y);
	printf("%f, %f\n", x, y);

	return 0;
}

