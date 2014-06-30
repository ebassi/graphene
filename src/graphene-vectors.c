/* graphene-vectors.c: Assorted vectors
 *
 * Copyright © 2014  Emmanuele Bassi
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include "graphene-private.h"
#include "graphene-vectors-private.h"
#include "graphene-alloc-private.h"

/**
 * SECTION:graphene-vectors
 * @Title: Vectors
 * @Short_Description: Vectors in 2, 3, and 4 dimensions
 *
 * Graphene has three vector types, distinguished by their length:
 *
 *  1. #graphene_vec2_t, which holds 2 components x and y
 *  2. #graphene_vec3_t, which holds 3 components x, y, and z
 *  3. #graphene_vec4_t, which holds 4 components x, y, z, and w
 *
 * Each vector type should be treated as an opaque data type.
 */

/* vec2 {{{ */

/**
 * graphene_vec2_alloc: (constructor)
 *
 * Allocates a new #graphene_vec2_t structure.
 *
 * The contents of the returned structure are undefined.
 *
 * Use graphene_vec2_init() to initialize the vector.
 *
 * Returns: (transfer full): the newly allocated #graphene_vec2_t
 *   structure. Use graphene_vec2_free() to free the resources allocated
 *   by this function.
 *
 * Since: 1.0
 */
graphene_vec2_t *
graphene_vec2_alloc (void)
{
  return graphene_alloc (sizeof (graphene_vec2_t), 1, 16);
}

/**
 * graphene_vec2_free:
 * @v: a #graphene_vec2_t
 *
 * Frees the resources allocated by @v
 *
 * Since: 1.0
 */
void
graphene_vec2_free (graphene_vec2_t *v)
{
  graphene_free (v);
}

/**
 * graphene_vec2_init:
 * @v: a #graphene_vec2_t
 * @x: the X field of the vector
 * @y: the Y field of the vector
 *
 * Initializes a #graphene_vec2_t using the given values.
 *
 * This function can be called multiple times.
 *
 * Returns: (transfer none): the initialized vector
 *
 * Since: 1.0
 */
graphene_vec2_t *
graphene_vec2_init (graphene_vec2_t *v,
                    float            x,
                    float            y)
{
  v->value = graphene_simd4f_init (x, y, 0.f, 0.f);

  return v;
}

/**
 * graphene_vec2_init_from_vec2:
 * @v: a #graphene_vec2_t
 * @src: a #graphene_vec2_t
 *
 * Copies the contents of @src into @v.
 *
 * Returns: (transfer none): the initialized vector
 *
 * Since: 1.0
 */
graphene_vec2_t *
graphene_vec2_init_from_vec2 (graphene_vec2_t       *v,
                              const graphene_vec2_t *src)
{
  v->value = src->value;

  return v;
}

/**
 * graphene_vec2_init_from_float:
 * @v: a #graphene_vec2_t
 * @src: (array fixed-size=2): an array of floating point values
 *   with at least two elements
 *
 * Initializes @v with the contents of the given array.
 *
 * Returns: (transfer none): the initialized vector
 *
 * Since: 1.0
 */
graphene_vec2_t *
graphene_vec2_init_from_float (graphene_vec2_t *v,
                               const float     *src)
{
  v->value = graphene_simd4f_init_2f (src);

  return v;
}

/**
 * graphene_vec2_get_x:
 * @v: a #graphene_vec2_t
 *
 * Retrieves the X component of the #graphene_vec2_t.
 *
 * Returns: the value of the X component
 *
 * Since: 1.0
 */
float
graphene_vec2_get_x (const graphene_vec2_t *v)
{
  return graphene_simd4f_get_x (v->value);
}

/**
 * graphene_vec2_get_y:
 * @v: a #graphene_vec2_t
 *
 * Retrieves the Y component of the #graphene_vec2_t.
 *
 * Returns: the value of the Y component
 *
 * Since: 1.0
 */
float
graphene_vec2_get_y (const graphene_vec2_t *v)
{
  return graphene_simd4f_get_y (v->value);
}

/**
 * graphene_vec2_to_float:
 * @v: a #graphene_vec2_t
 * @dest: (out caller-allocates) (array fixed-size=2): return location
 *   for an array of floating point values with at least 2 elements
 *
 * Stores the components of @v into an array.
 *
 * Since: 1.0
 */
void
graphene_vec2_to_float (const graphene_vec2_t *v,
                        float                 *dest)
{
  graphene_simd4f_dup_2f (v->value, dest);
}

/**
 * graphene_vec2_add:
 * @a: a #graphene_vec2_t
 * @b: a #graphene_vec2_t
 * @res: (out caller-allocates): return location for the result
 *
 * Adds each component of the two passed vectors and places
 * each result into the components of @res.
 *
 * Since: 1.0
 */
void
graphene_vec2_add (const graphene_vec2_t *a,
                   const graphene_vec2_t *b,
                   graphene_vec2_t       *res)
{
  res->value = graphene_simd4f_add (a->value, b->value);
}

/**
 * graphene_vec2_subtract:
 * @a: a #graphene_vec2_t
 * @b: a #graphene_vec2_t
 * @res: (out caller-allocates): return location for the result
 *
 * Subtracts each component of the two passed vectors and places
 * each result into the components of @res.
 *
 * Since: 1.0
 */
void
graphene_vec2_subtract (const graphene_vec2_t *a,
                        const graphene_vec2_t *b,
                        graphene_vec2_t       *res)
{
  res->value = graphene_simd4f_sub (a->value, b->value);
}

/**
 * graphene_vec2_multiply:
 * @a: a #graphene_vec2_t
 * @b: a #graphene_vec2_t
 * @res: (out caller-allocates): return location for the result
 *
 * Multiplies each component of the two passed vectors and places
 * each result into the components of @res.
 *
 * Since: 1.0
 */
void
graphene_vec2_multiply (const graphene_vec2_t *a,
                        const graphene_vec2_t *b,
                        graphene_vec2_t       *res)
{
  res->value = graphene_simd4f_mul (a->value, b->value);
}

/**
 * graphene_vec2_divide:
 * @a: a #graphene_vec2_t
 * @b: a #graphene_vec2_t
 * @res: (out caller-allocates): return location for the result
 *
 * Divides each component of the two passed vectors and places
 * each result into the components of @res.
 *
 * Since: 1.0
 */
void
graphene_vec2_divide (const graphene_vec2_t *a,
                      const graphene_vec2_t *b,
                      graphene_vec2_t       *res)
{
  res->value = graphene_simd4f_div (a->value, b->value);
}

/**
 * graphene_vec2_dot:
 * @a: a #graphene_vec2_t
 * @b: a #graphene_vec2_t
 *
 * Computes the dot product of the two given vectors.
 *
 * Returns: the dot product of the vectors
 *
 * Since: 1.0
 */
float
graphene_vec2_dot (const graphene_vec2_t *a,
                   const graphene_vec2_t *b)
{
  return graphene_simd4f_get_x (graphene_simd4f_dot2 (a->value, b->value));
}

/**
 * graphene_vec2_length:
 * @v: a #graphene_vec2_t
 *
 * Computes the length of the given vector.
 *
 * Returns: the length of the vector
 *
 * Since: 1.0
 */
float
graphene_vec2_length (const graphene_vec2_t *v)
{
  return graphene_simd4f_get_x (graphene_simd4f_length2 (v->value));
}

/**
 * graphene_vec2_normalize:
 * @v: a #graphene_vec2_t
 * @res: (out caller-allocates): return location for the
 *   normalized vector
 *
 * Computes the normalized vector for the given vector @v.
 *
 * Since: 1.0
 */
void
graphene_vec2_normalize (const graphene_vec2_t *v,
                         graphene_vec2_t       *res)
{
  res->value = graphene_simd4f_normalize2 (v->value);
}

/**
 * graphene_vec2_min:
 * @a: a #graphene_vec2_t
 * @b: a #graphene_vec2_t
 * @res: (out caller-allocates): the resulting vector
 *
 * Compares the two given vectors and places the minimum
 * values of each component into @res.
 *
 * Since: 1.0
 */
void
graphene_vec2_min (const graphene_vec2_t *a,
                   const graphene_vec2_t *b,
                   graphene_vec2_t       *res)
{
  res->value = graphene_simd4f_min (a->value, b->value);
}

/**
 * graphene_vec2_max:
 * @a: a #graphene_vec2_t
 * @b: a #graphene_vec2_t
 * @res: (out caller-allocates): the resulting vector
 *
 * Compares the two given vectors and places the maximum
 * values of each component into @res.
 *
 * Since: 1.0
 */
void
graphene_vec2_max (const graphene_vec2_t *a,
                   const graphene_vec2_t *b,
                   graphene_vec2_t       *res)
{
  res->value = graphene_simd4f_max (a->value, b->value);
}

enum {
  VEC2_ZERO,
  VEC2_ONE,
  VEC2_X_AXIS,
  VEC2_Y_AXIS,

  N_STATIC_VEC2
};

static graphene_vec2_t static_vec2[N_STATIC_VEC2];
static bool static_vec2_init = false;

static inline void
init_static_vec2 (void)
{
  if (static_vec2_init)
    return;

  static_vec2[VEC2_ZERO].value = graphene_simd4f_init_zero ();
  static_vec2[VEC2_ONE].value = graphene_simd4f_splat (1.0f);
  static_vec2[VEC2_X_AXIS].value = graphene_simd4f_init (1.0f, 0.0f, 0.0f, 0.0f);
  static_vec2[VEC2_Y_AXIS].value = graphene_simd4f_init (0.0f, 1.0f, 0.0f, 0.0f);

  static_vec2_init = true;
}

/**
 * graphene_vec2_zero:
 *
 * Retrieves a constant vector with (0, 0) components.
 *
 * Returns: (transfer none): the zero vector
 *
 * Since: 1.0
 */
const graphene_vec2_t *
graphene_vec2_zero (void)
{
  init_static_vec2 ();

  return &(static_vec2[VEC2_ZERO]);
}

/**
 * graphene_vec2_one:
 *
 * Retrieves a constant vector with (1, 1) components.
 *
 * Returns: (transfer none): the one vector
 *
 * Since: 1.0
 */
const graphene_vec2_t *
graphene_vec2_one (void)
{
  init_static_vec2 ();

  return &(static_vec2[VEC2_ONE]);
}

/**
 * graphene_vec2_x_axis:
 *
 * Retrieves a constant vector with (1, 0) components.
 *
 * Returns: (transfer none): the X axis vector
 *
 * Since: 1.0
 */
const graphene_vec2_t *
graphene_vec2_x_axis (void)
{
  init_static_vec2 ();

  return &(static_vec2[VEC2_X_AXIS]);
}

/**
 * graphene_vec2_y_axis:
 *
 * Retrieves a constant vector with (0, 1) components.
 *
 * Returns: (transfer none): the Y axis vector
 *
 * Since: 1.0
 */
const graphene_vec2_t *
graphene_vec2_y_axis (void)
{
  init_static_vec2 ();

  return &(static_vec2[VEC2_Y_AXIS]);
}

/* }}} vec2 */

/* vec3 {{{ */

/**
 * graphene_vec3_alloc: (constructor)
 *
 * Allocates a new #graphene_vec3_t structure.
 *
 * The contents of the returned structure are undefined.
 *
 * Use graphene_vec3_init() to initialize the vector.
 *
 * Returns: (transfer full): the newly allocated #graphene_vec3_t
 *   structure. Use graphene_vec3_free() to free the resources allocated
 *   by this function.
 *
 * Since: 1.0
 */
graphene_vec3_t *
graphene_vec3_alloc (void)
{
  return graphene_alloc (sizeof (graphene_vec3_t), 1, 16);
}

/**
 * graphene_vec3_free:
 * @v: a #graphene_vec3_t
 *
 * Frees the resources allocated by @v
 *
 * Since: 1.0
 */
void
graphene_vec3_free (graphene_vec3_t *v)
{
  graphene_free (v);
}

/**
 * graphene_vec3_init:
 * @v: a #graphene_vec3_t
 * @x: the X field of the vector
 * @y: the Y field of the vector
 * @z: the Z field of the vector
 *
 * Initializes a #graphene_vec3_t using the given values.
 *
 * This function can be called multiple times.
 *
 * Returns: (transfer none): a pointer to the initialized
 *   vector
 *
 * Since: 1.0
 */
graphene_vec3_t *
graphene_vec3_init (graphene_vec3_t *v,
                    float            x,
                    float            y,
                    float            z)
{
  v->value = graphene_simd4f_init (x, y, z, 0.f);

  return v;
}

/**
 * graphene_vec3_init_from_vec3:
 * @v: a #graphene_vec3_t
 * @src: a #graphene_vec3_t
 *
 * Initializes a #graphene_vec3_t with the values of another
 * #graphene_vec3_t.
 *
 * Returns: (transfer none): the initialized vector
 *
 * Since: 1.0
 */
graphene_vec3_t *
graphene_vec3_init_from_vec3 (graphene_vec3_t       *v,
                              const graphene_vec3_t *src)
{
  v->value = src->value;

  return v;
}

/**
 * graphene_vec3_init_from_float:
 * @v: a #graphene_vec3_t
 * @src: (array fixed-size=3): an array of 3 floating point values
 *
 * Initializes a #graphene_vec3_t with the values from an array.
 *
 * Returns: (transfer none): the initialized vector
 *
 * Since: 1.0
 */
graphene_vec3_t *
graphene_vec3_init_from_float (graphene_vec3_t *v,
                               const float     *src)
{
  v->value = graphene_simd4f_init_3f (src);

  return v;
}

/**
 * graphene_vec3_get_x:
 * @v: a #graphene_vec3_t
 *
 * Retrieves the first component of the given vector @v.
 *
 * Returns: the value of the first component of the vector
 *
 * Since: 1.0
 */
float
graphene_vec3_get_x (const graphene_vec3_t *v)
{
  return graphene_simd4f_get_x (v->value);
}

/**
 * graphene_vec3_get_y:
 * @v: a #graphene_vec3_t
 *
 * Retrieves the second component of the given vector @v.
 *
 * Returns: the value of the second component of the vector
 *
 * Since: 1.0
 */
float
graphene_vec3_get_y (const graphene_vec3_t *v)
{
  return graphene_simd4f_get_y (v->value);
}

/**
 * graphene_vec3_get_z:
 * @v: a #graphene_vec3_t
 *
 * Retrieves the third component of the given vector @v.
 *
 * Returns: the value of the third component of the vector
 *
 * Since: 1.0
 */
float
graphene_vec3_get_z (const graphene_vec3_t *v)
{
  return graphene_simd4f_get_z (v->value);
}

/**
 * graphene_vec3_to_float:
 * @v: a #graphene_vec3_t
 * @dest: (out caller-allocates) (array fixed-size=3): return location for
 *   an array of floating point values
 *
 * Copies the components of a #graphene_vec3_t into the given array.
 *
 * Since: 1.0
 */
void
graphene_vec3_to_float (const graphene_vec3_t *v,
                        float                 *dest)
{
  graphene_simd4f_dup_3f (v->value, dest);
}

/**
 * graphene_vec3_add:
 * @a: a #graphene_vec3_t
 * @b: a #graphene_vec3_t
 * @res: (out caller-allocates): return location for the resulting vector
 *
 * Adds each component of the two given vectors.
 *
 * Since: 1.0
 */
void
graphene_vec3_add (const graphene_vec3_t *a,
                   const graphene_vec3_t *b,
                   graphene_vec3_t       *res)
{
  res->value = graphene_simd4f_add (a->value, b->value);
}

/**
 * graphene_vec3_subtract:
 * @a: a #graphene_vec3_t
 * @b: a #graphene_vec3_t
 * @res: (out caller-allocates): return location for the resulting vector
 *
 * Subtracts each component of the two given vectors.
 *
 * Since: 1.0
 */
void
graphene_vec3_subtract (const graphene_vec3_t *a,
                        const graphene_vec3_t *b,
                        graphene_vec3_t       *res)
{
  res->value = graphene_simd4f_sub (a->value, b->value);
}

/**
 * graphene_vec3_multiply:
 * @a: a #graphene_vec3_t
 * @b: a #graphene_vec3_t
 * @res: (out caller-allocates): return location for the resulting vector
 *
 * Multiplies each component of the two given vectors.
 *
 * Since: 1.0
 */
void
graphene_vec3_multiply (const graphene_vec3_t *a,
                        const graphene_vec3_t *b,
                        graphene_vec3_t       *res)
{
  res->value = graphene_simd4f_mul (a->value, b->value);
}

/**
 * graphene_vec3_divide:
 * @a: a #graphene_vec3_t
 * @b: a #graphene_vec3_t
 * @res: (out caller-allocates): return location for the resulting vector
 *
 * Divides each component of the two given vectors.
 *
 * Since: 1.0
 */
void
graphene_vec3_divide (const graphene_vec3_t *a,
                      const graphene_vec3_t *b,
                      graphene_vec3_t       *res)
{
  res->value = graphene_simd4f_div (a->value, b->value);
}

/**
 * graphene_vec3_cross:
 * @a: a #graphene_vec3_t
 * @b: a #graphene_vec3_t
 * @res: (out caller-allocates): return location for the resulting vector
 *
 * Computes the cross product of the two given vectors.
 *
 * Since: 1.0
 */
void
graphene_vec3_cross (const graphene_vec3_t *a,
                     const graphene_vec3_t *b,
                     graphene_vec3_t       *res)
{
  res->value = graphene_simd4f_cross3 (a->value, b->value);
}

/**
 * graphene_vec3_dot:
 * @a: a #graphene_vec3_t
 * @b: a #graphene_vec3_t
 *
 * Computes the dot product of the two given vectors.
 *
 * Returns: the value of the dot product
 *
 * Since: 1.0
 */
float
graphene_vec3_dot (const graphene_vec3_t *a,
                   const graphene_vec3_t *b)
{
  return graphene_simd4f_get_x (graphene_simd4f_dot3 (a->value, b->value));
}

/**
 * graphene_vec3_length:
 * @v: a #graphene_vec3_t
 *
 * Retrieves the length of the given vector @v.
 *
 * Returns: the value of the length of the vector
 *
 * Since: 1.0
 */
float
graphene_vec3_length (const graphene_vec3_t *v)
{
  return graphene_simd4f_get_x (graphene_simd4f_length3 (v->value));
}

/**
 * graphene_vec3_normalize:
 * @v: a #graphene_vec3_t
 * @res: (out caller-allocates): return location for the normalized vector
 *
 * Normalizes the given #graphene_vec3_t.
 *
 * Since: 1.0
 */
void
graphene_vec3_normalize (const graphene_vec3_t *v,
                         graphene_vec3_t       *res)
{
  res->value = graphene_simd4f_normalize3 (v->value);
}

/**
 * graphene_vec3_min:
 * @a: a #graphene_vec3_t
 * @b: a #graphene_vec3_t
 * @res: (out caller-allocates): return location for the result vector
 *
 * Compares each component of the two given vectors and creates a
 * vector that contains the minimum values.
 *
 * Since: 1.0
 */
void
graphene_vec3_min (const graphene_vec3_t *a,
                   const graphene_vec3_t *b,
                   graphene_vec3_t       *res)
{
  res->value = graphene_simd4f_min (a->value, b->value);
}

/**
 * graphene_vec3_max:
 * @a: a #graphene_vec3_t
 * @b: a #graphene_vec3_t
 * @res: (out caller-allocates): return location for the result vector
 *
 * Compares each component of the two given vectors and creates a
 * vector that contains the maximum values.
 *
 * Since: 1.0
 */
void
graphene_vec3_max (const graphene_vec3_t *a,
                   const graphene_vec3_t *b,
                   graphene_vec3_t       *res)
{
  res->value = graphene_simd4f_max (a->value, b->value);
}

/**
 * graphene_vec3_get_xy:
 * @v: a #graphene_vec3_t
 * @res: (out caller-allocates): return location for a #graphene_vec2_t
 *
 * Creates a #graphene_vec2_t that contains the first and second
 * components of the given #graphene_vec3_t.
 *
 * Since: 1.0
 */
void
graphene_vec3_get_xy (const graphene_vec3_t *v,
                      graphene_vec2_t       *res)
{
  res->value = graphene_simd4f_zero_zw (v->value);
}

/**
 * graphene_vec3_get_xy0:
 * @v: a #graphene_vec3_t
 * @res: (out caller-allocates): return location for a #graphene_vec3_t
 *
 * Creates a #graphene_vec3_t that contains the first two components of
 * the given #graphene_vec3_t, and the third component set to 0.
 *
 * Since: 1.0
 */
void
graphene_vec3_get_xy0 (const graphene_vec3_t *v,
                       graphene_vec3_t       *res)
{
  res->value = graphene_simd4f_zero_zw (v->value);
}

/**
 * graphene_vec3_get_xyz0:
 * @v: a #graphene_vec3_t
 * @res: (out caller-allocates): return location for the vector
 *
 * Converts a #graphene_vec3_t in a #graphene_vec4_t using 0.0
 * as the value for the fourth component of the resulting vector.
 *
 * Since: 1.0
 */
void
graphene_vec3_get_xyz0 (const graphene_vec3_t *v,
                        graphene_vec4_t       *res)
{
  res->value = graphene_simd4f_zero_w (v->value);
}

/**
 * graphene_vec3_get_xyz1:
 * @v: a #graphene_vec3_t
 * @res: (out caller-allocates): return location for the vector
 *
 * Converts a #graphene_vec3_t in a #graphene_vec4_t using 1.0
 * as the value for the fourth component of the resulting vector.
 *
 * Since: 1.0
 */
void
graphene_vec3_get_xyz1 (const graphene_vec3_t *v,
                        graphene_vec4_t       *res)
{
  res->value = graphene_simd4f_add (graphene_simd4f_zero_w (v->value),
                                    graphene_simd4f_init (0.f, 0.f, 0.f, 1.f));
}

/**
 * graphene_vec3_get_xyzw:
 * @v: a #graphene_vec3_t
 * @w: the value of the W component
 * @res: (out caller-allocates): return location for the vector
 *
 * Converts a #graphene_vec3_t in a #graphene_vec4_t using @w as
 * the value of the fourth component of the resulting vector.
 *
 * Since: 1.0
 */
void
graphene_vec3_get_xyzw (const graphene_vec3_t *v,
                        float                  w,
                        graphene_vec4_t       *res)
{
  res->value = graphene_simd4f_add (graphene_simd4f_zero_w (v->value),
                                    graphene_simd4f_init (0.f, 0.f, 0.f, w));
}

enum {
  VEC3_ZERO,
  VEC3_ONE,
  VEC3_X_AXIS,
  VEC3_Y_AXIS,
  VEC3_Z_AXIS,

  N_STATIC_VEC3
};

static graphene_vec3_t static_vec3[N_STATIC_VEC3];
static bool static_vec3_init = false;

static inline void
init_static_vec3 (void)
{
  if (static_vec3_init)
    return;

  static_vec3[VEC3_ZERO].value = graphene_simd4f_init_zero ();
  static_vec3[VEC3_ONE].value = graphene_simd4f_splat (1.0f);
  static_vec3[VEC3_X_AXIS].value = graphene_simd4f_init (1.0f, 0.0f, 0.0f, 0.0f);
  static_vec3[VEC3_Y_AXIS].value = graphene_simd4f_init (0.0f, 1.0f, 0.0f, 0.0f);
  static_vec3[VEC3_Z_AXIS].value = graphene_simd4f_init (0.0f, 0.0f, 1.0f, 0.0f);

  static_vec3_init = true;
}

/**
 * graphene_vec3_zero:
 *
 * Provides a constant pointer to a vector with three components,
 * all sets to 0.
 *
 * Returns: (transfer none): a constant vector
 *
 * Since: 1.0
 */
const graphene_vec3_t *
graphene_vec3_zero (void)
{
  init_static_vec3 ();

  return &(static_vec3[VEC3_ZERO]);
}

/**
 * graphene_vec3_one:
 *
 * Provides a constant pointer to a vector with three components,
 * all sets to 1.
 *
 * Returns: (transfer none): a constant vector
 *
 * Since: 1.0
 */
const graphene_vec3_t *
graphene_vec3_one (void)
{
  init_static_vec3 ();

  return &(static_vec3[VEC3_ONE]);
}

/**
 * graphene_vec3_x_axis:
 *
 * Provides a constant pointer to a vector with three components
 * with values set to (1, 0, 0).
 *
 * Returns: (transfer none): a constant vector
 *
 * Since: 1.0
 */
const graphene_vec3_t *
graphene_vec3_x_axis (void)
{
  init_static_vec3 ();

  return &(static_vec3[VEC3_X_AXIS]);
}

/**
 * graphene_vec3_y_axis:
 *
 * Provides a constant pointer to a vector with three components
 * with values set to (0, 1, 0).
 *
 * Returns: (transfer none): a constant vector
 *
 * Since: 1.0
 */
const graphene_vec3_t *
graphene_vec3_y_axis (void)
{
  init_static_vec3 ();

  return &(static_vec3[VEC3_Y_AXIS]);
}

/**
 * graphene_vec3_z_axis:
 *
 * Provides a constant pointer to a vector with three components
 * with values set to (0, 0, 1).
 *
 * Returns: (transfer none): a constant vector
 *
 * Since: 1.0
 */
const graphene_vec3_t *
graphene_vec3_z_axis (void)
{
  init_static_vec3 ();

  return &(static_vec3[VEC3_Z_AXIS]);
}

/* }}} vec3 */

/* vec4 {{{ */
/**
 * graphene_vec4_alloc: (constructor)
 *
 * Allocates a new #graphene_vec4_t structure.
 *
 * The contents of the returned structure are undefined.
 *
 * Use graphene_vec4_init() to initialize the vector.
 *
 * Returns: (transfer full): the newly allocated #graphene_vec4_t
 *   structure. Use graphene_vec4_free() to free the resources allocated
 *   by this function.
 *
 * Since: 1.0
 */
graphene_vec4_t *
graphene_vec4_alloc (void)
{
  return graphene_alloc (sizeof (graphene_vec4_t), 1, 16);
}

/**
 * graphene_vec4_free:
 * @v: a #graphene_vec4_t
 *
 * Frees the resources allocated by @v
 *
 * Since: 1.0
 */
void
graphene_vec4_free (graphene_vec4_t *v)
{
  graphene_free (v);
}

/**
 * graphene_vec4_init:
 * @v: a #graphene_vec4_t
 * @x: the X field of the vector
 * @y: the Y field of the vector
 * @z: the Z field of the vector
 * @w: the W field of the vector
 *
 * Initializes a #graphene_vec4_t using the given values.
 *
 * This function can be called multiple times.
 *
 * Returns: (transfer none): a pointer to the initialized
 *   vector
 *
 * Since: 1.0
 */
graphene_vec4_t *
graphene_vec4_init (graphene_vec4_t *v,
                    float            x,
                    float            y,
                    float            z,
                    float            w)
{
  v->value = graphene_simd4f_init (x, y, z, w);

  return v;
}

/**
 * graphene_vec4_init_from_vec4:
 * @v: a #graphene_vec4_t
 * @src: a #graphene_vec4_t
 *
 * Initializes a #graphene_vec4_t using the components of
 * another #graphene_vec4_t.
 *
 * Returns: (transfer none): the initialized vector
 *
 * Since: 1.0
 */
graphene_vec4_t *
graphene_vec4_init_from_vec4 (graphene_vec4_t       *v,
                              const graphene_vec4_t *src)
{
  v->value = src->value;

  return v;
}

/**
 * graphene_vec4_init_from_vec3:
 * @v: a #graphene_vec4_t
 * @src: a #graphene_vec3_t
 * @w: the value for the fourth component of @v
 *
 * Initializes a #graphene_vec4_t using the components of a
 * #graphene_vec3_t and the value of @w.
 *
 * Returns: (transfer none): the initialized vector
 *
 * Since: 1.0
 */
graphene_vec4_t *
graphene_vec4_init_from_vec3 (graphene_vec4_t       *v,
                              const graphene_vec3_t *src,
                              float                  w)
{
  v->value = graphene_simd4f_merge_w (src->value, w);

  return v;
}

/**
 * graphene_vec4_init_from_vec2:
 * @v: a #graphene_vec4_t
 * @src: a #graphene_vec2_t
 * @z: the value for the third component of @v
 * @w: the value for the fourth component of @v
 *
 * Initializes a #graphene_vec4_t using the components of a
 * #graphene_vec2_t and the values of @z and @w.
 *
 * Returns: (transfer none): the initialized vector
 *
 * Since: 1.0
 */
graphene_vec4_t *
graphene_vec4_init_from_vec2 (graphene_vec4_t       *v,
                              const graphene_vec2_t *src,
                              float                  z,
                              float                  w)
{
  v->value = graphene_simd4f_merge_low (src->value, graphene_simd4f_init (z, w, 0, 0));

  return v;
}

/**
 * graphene_vec4_init_from_float:
 * @v: a #graphene_vec4_t
 * @src: (array fixed-size=4): an array of four floating point values
 *
 * Initializes a #graphene_vec4_t with the values inside the given array.
 *
 * Returns: (transfer none): the initialized vector
 *
 * Since: 1.0
 */
graphene_vec4_t *
graphene_vec4_init_from_float (graphene_vec4_t *v,
                               const float     *src)
{
  v->value = graphene_simd4f_init_4f (src);

  return v;
}

/**
 * graphene_vec4_get_x:
 * @v: a #graphene_vec4_t
 *
 * Retrieves the value of the first component of the given #graphene_vec4_t.
 *
 * Returns: the value of the first component
 *
 * Since: 1.0
 */
float
graphene_vec4_get_x (const graphene_vec4_t *v)
{
  return graphene_simd4f_get_x (v->value);
}

/**
 * graphene_vec4_get_y:
 * @v: a #graphene_vec4_t
 *
 * Retrieves the value of the second component of the given #graphene_vec4_t.
 *
 * Returns: the value of the second component
 *
 * Since: 1.0
 */
float
graphene_vec4_get_y (const graphene_vec4_t *v)
{
  return graphene_simd4f_get_y (v->value);
}

/**
 * graphene_vec4_get_z:
 * @v: a #graphene_vec4_t
 *
 * Retrieves the value of the third component of the given #graphene_vec4_t.
 *
 * Returns: the value of the third component
 *
 * Since: 1.0
 */
float
graphene_vec4_get_z (const graphene_vec4_t *v)
{
  return graphene_simd4f_get_z (v->value);
}

/**
 * graphene_vec4_get_w:
 * @v: a #graphene_vec4_t
 *
 * Retrieves the value of the fourth component of the given #graphene_vec4_t.
 *
 * Returns: the value of the fourth component
 *
 * Since: 1.0
 */
float
graphene_vec4_get_w (const graphene_vec4_t *v)
{
  return graphene_simd4f_get_w (v->value);
}

/**
 * graphene_vec4_to_float:
 * @v: a #graphene_vec4_t
 * @dest: (out caller-allocates) (array fixed-size=4): return location for
 *   an array of floating point values
 *
 * Stores the components of the given #graphene_vec4_t into an array
 * of floating point values.
 *
 * Since: 1.0
 */
void
graphene_vec4_to_float (const graphene_vec4_t *v,
                        float                 *dest)
{
  graphene_simd4f_dup_4f (v->value, dest);
}

/**
 * graphene_vec4_add:
 * @a: a #graphene_vec4_t
 * @b: a #graphene_vec4_t
 * @res: (out caller-allocates): return location for the resulting vector
 *
 * Adds each component of the two given vectors.
 *
 * Since: 1.0
 */
void
graphene_vec4_add (const graphene_vec4_t *a,
                   const graphene_vec4_t *b,
                   graphene_vec4_t       *res)
{
  res->value = graphene_simd4f_add (a->value, b->value);
}

/**
 * graphene_vec4_subtract:
 * @a: a #graphene_vec4_t
 * @b: a #graphene_vec4_t
 * @res: (out caller-allocates): return location for the resulting vector
 *
 * Subtracts each component of the two given vectors.
 *
 * Since: 1.0
 */
void
graphene_vec4_subtract (const graphene_vec4_t *a,
                        const graphene_vec4_t *b,
                        graphene_vec4_t       *res)
{
  res->value = graphene_simd4f_sub (a->value, b->value);
}

/**
 * graphene_vec4_multiply:
 * @a: a #graphene_vec4_t
 * @b: a #graphene_vec4_t
 * @res: (out caller-allocates): return location for the resulting vector
 *
 * Multiplies each component of the two given vectors.
 *
 * Since: 1.0
 */
void
graphene_vec4_multiply (const graphene_vec4_t *a,
                        const graphene_vec4_t *b,
                        graphene_vec4_t       *res)
{
  res->value = graphene_simd4f_mul (a->value, b->value);
}

/**
 * graphene_vec4_divide:
 * @a: a #graphene_vec4_t
 * @b: a #graphene_vec4_t
 * @res: (out caller-allocates): return location for the resulting vector
 *
 * Divides each component of the two given vectors.
 *
 * Since: 1.0
 */
void
graphene_vec4_divide (const graphene_vec4_t *a,
                      const graphene_vec4_t *b,
                      graphene_vec4_t       *res)
{
  res->value = graphene_simd4f_div (a->value, b->value);
}

/**
 * graphene_vec4_dot:
 * @a: a #graphene_vec4_t
 * @b: a #graphene_vec4_t
 *
 * Computes the dot product of the two given vectors.
 *
 * Returns: the value of the dot product
 *
 * Since: 1.0
 */
float
graphene_vec4_dot (const graphene_vec4_t *a,
                   const graphene_vec4_t *b)
{
  return graphene_simd4f_get_x (graphene_simd4f_dot4 (a->value, b->value));
}

/**
 * graphene_vec4_length:
 * @v: a #graphene_vec4_t
 *
 * Computes the length of the given #graphene_vec4_t.
 *
 * Returns: the length of the vector
 *
 * Since: 1.0
 */
float
graphene_vec4_length (const graphene_vec4_t *v)
{
  return graphene_simd4f_get_x (graphene_simd4f_length4 (v->value));
}

/**
 * graphene_vec4_normalize:
 * @v: a #graphene_vec4_t
 * @res: (out caller-allocates): return location for the normalized
 *   vector
 *
 * Normalizes the given #graphene_vec4_t.
 *
 * Since: 1.0
 */
void
graphene_vec4_normalize (const graphene_vec4_t *v,
                         graphene_vec4_t       *res)
{
  res->value = graphene_simd4f_normalize4 (v->value);
}

/**
 * graphene_vec4_min:
 * @a: a #graphene_vec4_t
 * @b: a #graphene_vec4_t
 * @res: (out caller-allocates): return location for the result vector
 *
 * Compares each component of the two given vectors and creates a
 * vector that contains the minimum values.
 *
 * Since: 1.0
 */
void
graphene_vec4_min (const graphene_vec4_t *a,
                   const graphene_vec4_t *b,
                   graphene_vec4_t       *res)
{
  res->value = graphene_simd4f_min (a->value, b->value);
}

/**
 * graphene_vec4_max:
 * @a: a #graphene_vec4_t
 * @b: a #graphene_vec4_t
 * @res: (out caller-allocates): return location for the result vector
 *
 * Compares each component of the two given vectors and creates a
 * vector that contains the maximum values.
 *
 * Since: 1.0
 */
void
graphene_vec4_max (const graphene_vec4_t *a,
                   const graphene_vec4_t *b,
                   graphene_vec4_t       *res)
{
  res->value = graphene_simd4f_max (a->value, b->value);
}

/**
 * graphene_vec4_get_xy:
 * @v: a #graphene_vec4_t
 * @res: (out caller-allocates): return location for a #graphene_vec2_t
 *
 * Creates a #graphene_vec2_t that contains the first two components
 * of the given #graphene_vec4_t.
 *
 * Since: 1.0
 */
void
graphene_vec4_get_xy (const graphene_vec4_t *v,
                      graphene_vec2_t       *res)
{
  res->value = graphene_simd4f_zero_zw (v->value);
}

/**
 * graphene_vec4_get_xyz:
 * @v: a #graphene_vec4_t
 * @res: (out caller-allocates): return location for a graphene_vec3_t
 *
 * Creates a #graphene_vec3_t that contains the first three components
 * of the given #graphene_vec4_t.
 *
 * Since: 1.0
 */
void
graphene_vec4_get_xyz (const graphene_vec4_t *v,
                       graphene_vec3_t       *res)
{
  res->value = graphene_simd4f_zero_w (v->value);
}

enum {
  VEC4_ZERO,
  VEC4_ONE,
  VEC4_X_AXIS,
  VEC4_Y_AXIS,
  VEC4_Z_AXIS,
  VEC4_W_AXIS,

  N_STATIC_VEC4
};

static graphene_vec4_t static_vec4[N_STATIC_VEC4];
static bool static_vec4_init = false;

static inline void
init_static_vec4 (void)
{
  if (static_vec4_init)
    return;

  static_vec4[VEC4_ZERO].value = graphene_simd4f_init_zero ();
  static_vec4[VEC4_ONE].value = graphene_simd4f_splat (1.0f);
  static_vec4[VEC4_X_AXIS].value = graphene_simd4f_init (1.0f, 0.0f, 0.0f, 0.0f);
  static_vec4[VEC4_Y_AXIS].value = graphene_simd4f_init (0.0f, 1.0f, 0.0f, 0.0f);
  static_vec4[VEC4_Z_AXIS].value = graphene_simd4f_init (0.0f, 0.0f, 1.0f, 0.0f);
  static_vec4[VEC4_W_AXIS].value = graphene_simd4f_init (0.0f, 0.0f, 0.0f, 1.0f);

  static_vec4_init = true;
}

/**
 * graphene_vec4_zero:
 *
 * Retrieves a pointer to a #graphene_vec4_t with all its
 * components set to 0.
 *
 * Returns: (transfer none): a constant vector
 *
 * Since: 1.0
 */
const graphene_vec4_t *
graphene_vec4_zero (void)
{
  init_static_vec4 ();

  return &(static_vec4[VEC4_ZERO]);
}

/**
 * graphene_vec4_one:
 *
 * Retrieves a pointer to a #graphene_vec4_t with all its
 * components set to 1.
 *
 * Returns: (transfer none): a constant vector
 *
 * Since: 1.0
 */
const graphene_vec4_t *
graphene_vec4_one (void)
{
  init_static_vec4 ();

  return &(static_vec4[VEC4_ONE]);
}

/**
 * graphene_vec4_x_axis:
 *
 * Retrieves a pointer to a #graphene_vec4_t with its
 * components set to (1, 0, 0, 0).
 *
 * Returns: (transfer none): a constant vector
 *
 * Since: 1.0
 */
const graphene_vec4_t *
graphene_vec4_x_axis (void)
{
  init_static_vec4 ();

  return &(static_vec4[VEC4_X_AXIS]);
}

/**
 * graphene_vec4_y_axis:
 *
 * Retrieves a pointer to a #graphene_vec4_t with its
 * components set to (0, 1, 0, 0).
 *
 * Returns: (transfer none): a constant vector
 *
 * Since: 1.0
 */
const graphene_vec4_t *
graphene_vec4_y_axis (void)
{
  init_static_vec4 ();

  return &(static_vec4[VEC4_Y_AXIS]);
}

/**
 * graphene_vec4_z_axis:
 *
 * Retrieves a pointer to a #graphene_vec4_t with its
 * components set to (0, 0, 1, 0).
 *
 * Returns: (transfer none): a constant vector
 *
 * Since: 1.0
 */
const graphene_vec4_t *
graphene_vec4_z_axis (void)
{
  init_static_vec4 ();

  return &(static_vec4[VEC4_Z_AXIS]);
}

/**
 * graphene_vec4_w_axis:
 *
 * Retrieves a pointer to a #graphene_vec4_t with its
 * components set to (0, 0, 0, 1).
 *
 * Returns: (transfer none): a constant vector
 *
 * Since: 1.0
 */
const graphene_vec4_t *
graphene_vec4_w_axis (void)
{
  init_static_vec4 ();

  return &(static_vec4[VEC4_W_AXIS]);
}

/* }}} vec4 */
