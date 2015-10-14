#ifndef Vector_H
#define Vector_H
#ifdef __cplusplus
extern "C" {
#endif

#include <math.h>
#include <stdbool.h>

/**
 * A basic type for representing a point in 2-dimensional space.
 */
typedef struct Vector2 {
    float x, y;
}Vector2;

/**
 * A basic type for representing a point in 3-dimensional space.
 */
typedef struct Vector3 {
    float x, y, z;
}Vector3;

/**
 * A type for representing the position, scale, and rotation of an object.
 */
typedef struct tagTvTransform {
  Vector3 position;
  Vector3 scale;
  Vector3 rotation;
}TvTransform;

/**
 * A type for representing a Rectangle.
 */
typedef struct tagRect {
    float x, y;
    float w, h;
}Rect;

/**
 * Checks if the given Rectangle contains the given point.
 * @param r the Rectangle to check for the point within.
 * @param point the point to determine if is in the bounds of the Rect or not.
 * @return TRUE if the Rectangle does contain the point, else FALSE.
 */
bool Rect_contains(Rect r, Vector2 point);
/**
 * Checks if the given Rectangle overlaps the other given Rectangle.
 * @param r1 the first Rectangle.
 * @param r2 the...second...Rectangle.
 * @return TRUE if the Rectangles overlap, else FALSE.
 */
bool Rect_overlaps(Rect* r1, Rect* r2);

/**
 * A basic type for representing a point in 4-dimensional space
 */
typedef struct Vector4 {
  float x, y, z, w;
}Vector4;
typedef Vector4 quaternion;

extern const Vector2 Vector2_zero;
extern const Vector3 Vector3_zero;
extern const Vector4 Vector4_zero;

extern const Vector3 Vector3_forward;
extern const Vector4 Vector4_forward;

/* new */
Vector2 Vector2_new(float x, float y);
Vector3 Vector3_new(float x, float y, float z);
Vector4 Vector4_new(float x, float y, float z, float w);

/* add */
void Vector2_add(Vector2 v1, Vector2 v2, Vector2* result);
void Vector3_add(Vector3 v1, Vector3 v2, Vector3* result);
void Vector4_add(Vector4 v1, Vector4 v2, Vector4* result);

/* sub */
void Vector2_sub(Vector2 v1, Vector2 v2, Vector2* result);
void Vector3_sub(Vector3 v1, Vector3 v2, Vector3* result);
void Vector4_sub(Vector4 v1, Vector4 v2, Vector4* result);

/* normalize */
void Vector2_normalize(Vector2 v, Vector2* result);
void Vector3_normalize(Vector3 v, Vector3* result);

/* scale */
void Vector2_scale(Vector2 *v, float factor);
void Vector3_scale(Vector3 *v, float factor);
void Vector4_scale(Vector4 *v, float factor);

/* cross product */
float Vector2_cross(Vector2* v1, Vector2* v2);
void Vector3_cross(Vector3* v1, Vector3* v2, Vector3* result);
void Vector4_cross(Vector4* v1, Vector4* v2, Vector4* result);

/* dot-product */
float Vector2_dot(Vector2 v1, Vector2 v2);
float Vector3_dot(Vector3 v1, Vector3 v2);
float Vector4_dot(Vector4 v1, Vector4 v2);

/* distance */
float Vector2_distance(Vector2 v1, Vector2 v2);
float Vector3_distance(Vector3 v1, Vector3 v2);
float Vector4_distance(Vector4 v1, Vector4 v2);

/* magnitude */
float Vector2_mag(Vector2* v);
float Vector3_mag(Vector3* v);

/**
 * Get the unit diRection Vector from v1 to v2.
 * @param v1 the "start" Vector.
 * @param v2 the "destination" Vector.
 * @return a unit Vector representing the diRection of v1 to v2.
 */
void Vector3_diRection(Vector3 v1, Vector3 v2, Vector3* result);

/**
 * Linearly interpolates between the given Vectors.
 * @param v1 the first Vector to interpolate from.
 * @param v2 the Vector to interpolate to.
 * @param t the time (0-1)
 * @return the new Vector 
 */
Vector2 Vector2_lerp(Vector2 v1, Vector2 v2, float t);

/**
 * Linearly interpolates between the given 3-D Vectors.
 * @param v1 the "start" Vector.
 * @param v2 the "destination" Vector.
 * @param t the time (0-1)
 * @return the new Vector
 */
Vector3 Vector3_lerp(Vector3 v1, Vector3 v2, float t);

/**
 * Linearly interpolates between the given 4-D Vectors.
 * @param v1 the "start" Vector.
 * @param v2 the "destination" Vector.
 * @param t the time (0-1)
 * @return the new Vector
 */
Vector4 Vector4_lerp(Vector4 v1, Vector4 v2, float t);

#ifdef __cplusplus
}
#endif
#endif

