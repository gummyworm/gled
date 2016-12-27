/*****************************************************************************/
/* matrix.h                                                                  */
/* This file contains definitions for the matrix type(s) as well as various  */
/* functions to manipulate them.                                             */
/*                                                                           */
/* Bryce Wilson                                                              */
/* created April, 2013                                                       */
/*****************************************************************************/
#ifndef _MATRIX_H
#define _MATRIX_H
#ifdef __cplusplus
extern "C" {
#endif

#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "vector.h"

#define PI 3.1415926536
#define MATRIX_STACK_SIZE 256

#ifdef _WIN32
#pragma pack(4)
#endif
typedef struct Mat4x4 {
	float a0, b0, c0, d0;
	float a1, b1, c1, d1;
	float a2, b2, c2, d2;
	float a3, b3, c3, d3;
} Mat4x4;

void mat4x4_push(Mat4x4* mat);
void mat4x4_pop(Mat4x4* mat);

void mat4x4_load_identity(Mat4x4* mat);
void mat4x4_scale(Mat4x4* mat, float x, float y, float z);
void mat4x4_translate(Mat4x4* mat, float x, float y, float z);
void mat4x4_rotate(Mat4x4* mat, float angle, float x, float y, float z);

float mat4x4_det(Mat4x4* mat);
bool mat4x4_inverse(Mat4x4 mat, Mat4x4* out);

Mat4x4 mat4x4_multiply(Mat4x4 mat1, Mat4x4 mat2);
Vector4 mat4x4_multiply_vec4x1(Mat4x4 mat, Vector4 vec);
Vector3 mat4x4_multiply_vec3x1(Mat4x4 mat, Vector3 vec);
void mat4x4_perspective(Mat4x4* mat, float fov, float aspect, float zNear,
			float zFar);
void mat4x4_orthographic(Mat4x4* mat, float left, float right, float top,
			 float bottom, float nearZ, float farZ);

float* mat4x4_to_array(Mat4x4* mat);

extern const Mat4x4 Mat4x4Identity;

#ifdef __cplusplus
}
#endif
#endif
