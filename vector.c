#include "vector.h"

/******************************************************************************
 * zero Vectors
******************************************************************************/
const Vector2 Vector2_zero = {0, 0};
const Vector3 Vector3_zero = {0, 0, 0};
const Vector4 Vector4_zero = {0, 0, 0, 0};

/******************************************************************************
 * forward Vectors- point in the positive Z direction
******************************************************************************/
const Vector3 Vector3_forward = {0.0f, 0.0f, 1.0f};
const Vector4 Vector4_forward = {0.0f, 0.0f, 1.0f, 1.0f};

/******************************************************************************
 * new
******************************************************************************/
Vector2 Vector2_new(float x, float y) {
	Vector2 ret = {x, y};
	return ret;
}
Vector3 Vector3_new(float x, float y, float z) {
	Vector3 ret = {x, y, z};
	return ret;
}
Vector4 Vector4_new(float x, float y, float z, float w) {
	Vector4 ret = {x, y, z, w};
	return ret;
}
/******************************************************************************
 * add
******************************************************************************/
void Vector2_add(Vector2 v1, Vector2 v2, Vector2* result) {
	result->x = v1.x + v2.x;
	result->y = v1.y + v2.y;
}
void Vector3_add(Vector3 v1, Vector3 v2, Vector3* result) {
	result->x = v1.x + v2.x;
	result->y = v1.y + v2.y;
	result->z = v1.z + v2.z;
}
void Vector4_add(Vector4 v1, Vector4 v2, Vector4* result) {
	result->x = v1.x + v2.x;
	result->y = v1.y + v2.y;
	result->z = v1.z + v2.z;
	result->w = v1.w + v2.w;
}
/******************************************************************************
 * subtract
******************************************************************************/
void Vector2_sub(Vector2 v1, Vector2 v2, Vector2* result) {
	result->x = v1.x - v2.x;
	result->y = v1.y - v2.y;
}
void Vector3_sub(Vector3 v1, Vector3 v2, Vector3* result) {
	result->x = v1.x - v2.x;
	result->y = v1.y - v2.y;
	result->z = v1.z - v2.z;
}
void Vector4_sub(Vector4 v1, Vector4 v2, Vector4* result) {
	result->x = v1.x - v2.x;
	result->y = v1.y - v2.y;
	result->z = v1.z - v2.z;
	result->w = v1.w - v2.w;
}
/******************************************************************************
 * scale
******************************************************************************/
void Vector2_scale(Vector2* v, float factor) {
	v->x *= factor;
	v->y *= factor;
}
void Vector3_scale(Vector3* v, float factor) {
	v->x *= factor;
	v->y *= factor;
	v->z *= factor;
}
void Vector4_scale(Vector4* v, float factor) {
	v->x *= factor;
	v->y *= factor;
	v->z *= factor;
	v->w *= factor;
}
/******************************************************************************
 * dot product
******************************************************************************/
float Vector2_dot(Vector2 v1, Vector2 v2) { return v1.x * v2.x + v1.y * v2.y; }
float Vector3_dot(Vector3 v1, Vector3 v2) {
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}
float Vector4_dot(Vector4 v1, Vector4 v2) {
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z + v1.w * v2.w;
}
/******************************************************************************
 * normalize
******************************************************************************/
void Vector2_normalize(Vector2 v, Vector2* result) {
	float mag = Vector2_mag(&v);
	/* assert(mag != 0.0f); */
	if (mag == 0.0f) {
		return;
	}
	result->x = v.x / mag;
	result->y = v.y / mag;
}
void Vector3_normalize(Vector3 v, Vector3* result) {
	float mag = Vector3_mag(&v);
	/* assert(mag != 0.0f); */
	if (mag == 0.0f) {
		return;
	}
	result->x = v.x / mag;
	result->y = v.y / mag;
	result->z = v.z / mag;
}
/******************************************************************************
 * magnitude
******************************************************************************/
float Vector2_mag(Vector2* v) { return (float)sqrt(v->x * v->x + v->y * v->y); }
float Vector3_mag(Vector3* v) {
	return (float)sqrt(v->x * v->x + v->y * v->y + v->z * v->z);
}
/******************************************************************************
 * cross product
******************************************************************************/
float Vector2_cross(Vector2* v1, Vector2* v2) {
	return (v1->x * v2->y) - (v1->y * v2->x);
}
void Vector3_cross(Vector3* v1, Vector3* v2, Vector3* result) {
	result->x = (v1->y * v2->z) - (v1->z * v2->y);
	result->y = (v1->z * v2->x) - (v1->x * v2->z);
	result->z = (v1->x * v2->y) - (v1->y * v2->x);
}
void Vector4_cross(Vector4* v1, Vector4* v2, Vector4* result) { /* TODO: */
}
/******************************************************************************
 * distance
******************************************************************************/
float Vector2_distance(Vector2 v1, Vector2 v2) {
	return (float)sqrt((v2.x * v2.x) - (v1.x * v1.x) + (v2.y * v2.y) -
			   (v1.y * v1.y));
}
float Vector3_distance(Vector3 v1, Vector3 v2) {
	return (float)sqrt((v2.x * v2.x) - (v1.x * v1.x) + (v2.y * v2.y) -
			   (v1.y * v1.y) + (v2.z * v2.z) - (v1.z * v1.z));
}
float Vector4_distance(Vector4 v1, Vector4 v2) {
	return (float)sqrt((v2.x * v2.x) - (v1.x * v1.x) + (v2.y * v2.y) -
			   (v1.y * v1.y) + (v2.z * v2.z) - (v1.z * v1.z) +
			   (v2.w * v2.w) - (v1.w * v1.w));
}
/******************************************************************************
 * LERP
******************************************************************************/
Vector2 Vector2_lerp(Vector2 v1, Vector2 v2, float t) {
	/* TODO: */
	Vector2 ret = {0.0f, 0.0f};
	return ret;
}
Vector3 Vector3_lerp(Vector3 v1, Vector3 v2, float t) {
	/* TODO: */
	Vector3 ret = {0.0f, 0.0f, 0.0f};
	return ret;
}
Vector4 Vector4_lerp(Vector4 v1, Vector4 v2, float t) {
	/* TODO: */
	Vector4 ret = {0.0f, 0.0f, 0.0f, 0.0f};
	return ret;
}
/******************************************************************************
 * direction
******************************************************************************/
void Vector3_direction(Vector3 v1, Vector3 v2, Vector3* result) {
	result->x = v1.x - v2.x;
	result->y = v1.y - v2.y;
	result->z = v1.z - v2.z;
	Vector3_normalize(*result, result);
}
bool rect_contains(Rect r, Vector2 point) {
	if ((point.x >= r.x) && (point.x <= (r.x + r.w)) && (point.y >= r.y) &&
	    (point.y <= (r.y + r.h))) {
		return true;
	}
	return false;
}
bool rect_overlaps(Rect* r1, Rect* r2) {
	if ((r1->x < (r2->x + r2->w)) && ((r1->x + r1->w) > r2->x) &&
	    (r1->y < (r2->y + r2->h)) && ((r1->y + r1->h) > r2->y)) {
		return true;
	}
	return false;
}
