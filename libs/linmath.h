#ifndef GLC_LINMATH_H
#define GLC_LINMATH_H

#include <string.h>
#include <math.h>
#include <stdio.h>

#define GLC_PI 3.14159265358979323846f

#define GLC_RAD(degrees) ((degrees) * GLC_PI / 180.0f)

void mat4Multiply(float matrix[16], const float lhs[16], const float rhs[16])
{
	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 4; ++j)
		{
			matrix[i * 4 + j] =
					lhs[j + 0 * 4] * rhs[i * 4 + 0] +
					lhs[j + 1 * 4] * rhs[i * 4 + 1] +
					lhs[j + 2 * 4] * rhs[i * 4 + 2] +
					lhs[j + 3 * 4] * rhs[i * 4 + 3];
		}
	}
}

void mat4Perspective(float matrix[16], float fov, float aspect, float zNear, float zFar)
{
	fov = GLC_RAD(fov);

	const float range = tanf(fov * 0.5f) * zNear;

	matrix[0] = (zNear * 2.0f) / (range * aspect + range * aspect);
	matrix[1] = 0.0f;
	matrix[2] = 0.0f;
	matrix[3] = 0.0f;

	matrix[4] = 0.0f;
	matrix[5] = zNear / range;
	matrix[6] = 0.0f;
	matrix[7] = 0.0f;

	matrix[8] = 0.0f;
	matrix[9] = 0.0f;
	matrix[10] = -(zFar + zNear) / (zFar - zNear);
	matrix[11] = -1.0f;

	matrix[12] = 0.0f;
	matrix[13] = 0.0f;
	matrix[14] = -(zFar * zNear * 2.0f) / (zFar - zNear);
	matrix[15] = 0.0f;
}

void mat4Translation(float matrix[16], float x, float y, float z)
{
	const float translation[16] = {
			1.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			x, y, z, 1.0f,
	};

	memcpy(matrix, translation, sizeof(translation));
}

void mat4Scaling(float matrix[16], float sx, float sy, float sz)
{
	const float translation[16] = {
			sx, 0.0f, 0.0f, 0.0f,
			0.0f, sy, 0.0f, 0.0f,
			0.0f, 0.0f, sz, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f,
	};

	memcpy(matrix, translation, sizeof(translation));
}

void mat4Rotation(float matrix[16], float angle, float x, float y, float z)
{
	const float s = sinf(angle), c = cosf(angle);
	const float oc = 1.0f - c;

	matrix[0] = x * x * oc + c;
	matrix[1] = x * y * oc - z * s;
	matrix[2] = x * z * oc + y * s;
	matrix[3] = 0.0f;

	matrix[4] = y * x * oc + z * s;
	matrix[5] = y * y * oc + c;
	matrix[6] = y * z * oc - x * s;
	matrix[7] = 0.0f;

	matrix[8] = x * z * oc - y * s;
	matrix[9] = y * z * oc + x * s;
	matrix[10] = z * z * oc + c;
	matrix[11] = 0.0f;

	matrix[12] = 0.0f;
	matrix[13] = 0.0f;
	matrix[14] = 0.0f;
	matrix[15] = 1.0f;
}

#endif