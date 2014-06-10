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

#include "config.h"

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
  g_return_val_if_fail (v != NULL, NULL);

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
  g_return_val_if_fail (v != NULL, NULL);
  g_return_val_if_fail (src != NULL, v);

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
  g_return_val_if_fail (v != NULL, NULL);
  g_return_val_if_fail (src != NULL, v);

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
  g_return_val_if_fail (v != NULL, 0.f);

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
  g_return_val_if_fail (v != NULL, 0.f);

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
  g_return_if_fail (v != NULL);
  g_return_if_fail (dest != NULL);

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
  g_return_if_fail (a != NULL && b != NULL);
  g_return_if_fail (res != NULL);

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
  g_return_if_fail (a != NULL && b!= NULL);
  g_return_if_fail (res != NULL);

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
  g_return_if_fail (a != NULL && b!= NULL);
  g_return_if_fail (res != NULL);

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
  g_return_if_fail (a != NULL && b!= NULL);
  g_return_if_fail (res != NULL);

  res->value = graphene_simd4f_div (a->value, b->value);
}

/**
 * graphene_vec2_do:
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
  g_return_val_if_fail (a != NULL && b != NULL, 0.f);

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
  g_return_val_if_fail (v != NULL, 0.f);

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
  g_return_if_fail (v != NULL);
  g_return_if_fail (res != NULL);

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
  g_return_if_fail (a != NULL && b != NULL);
  g_return_if_fail (res != NULL);

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
  g_return_if_fail (a != NULL && b != NULL);
  g_return_if_fail (res != NULL);

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
static gboolean static_vec2_init = FALSE;

static inline void
init_static_vec2 (void)
{
  if (static_vec2_init)
    return;

  static_vec2[VEC2_ZERO].value = graphene_simd4f_init_zero ();
  static_vec2[VEC2_ONE].value = graphene_simd4f_splat (1.0f);
  static_vec2[VEC2_X_AXIS].value = graphene_simd4f_init (1.0f, 0.0f, 0.0f, 0.0f);
  static_vec2[VEC2_Y_AXIS].value = graphene_simd4f_init (0.0f, 1.0f, 0.0f, 0.0f);

  static_vec2_init = TRUE;
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
  g_return_val_if_fail (v != NULL, NULL);

  v->value = graphene_simd4f_init (x, y, z, 0.f);

  return v;
}

/**
 * graphene_vec3_init_from_vec3:
 * @v: ...
 * @src: ...
 *
 * ...
 *
 * Returns: (transfer none): ...
 *
 * Since: 1.0
 */
graphene_vec3_t *
graphene_vec3_init_from_vec3 (graphene_vec3_t       *v,
                              const graphene_vec3_t *src)
{
  g_return_val_if_fail (v != NULL, NULL);
  g_return_val_if_fail (src != NULL, v);

  v->value = src->value;

  return v;
}

/**
 * graphene_vec3_init_from_float:
 * @v: ...
 * @src: (array fixed-size=3): ...
 *
 * ...
 *
 * Returns: (transfer none): ...
 *
 * Since: 1.0
 */
graphene_vec3_t *
graphene_vec3_init_from_float (graphene_vec3_t *v,
                               const float     *src)
{
  g_return_val_if_fail (v != NULL, NULL);
  g_return_val_if_fail (src != NULL, v);

  v->value = graphene_simd4f_init_3f (src);

  return v;
}

float
graphene_vec3_get_x (const graphene_vec3_t *v)
{
  g_return_val_if_fail (v != NULL, 0.f);

  return graphene_simd4f_get_x (v->value);
}

float
graphene_vec3_get_y (const graphene_vec3_t *v)
{
  g_return_val_if_fail (v != NULL, 0.f);

  return graphene_simd4f_get_y (v->value);
}

float
graphene_vec3_get_z (const graphene_vec3_t *v)
{
  g_return_val_if_fail (v != NULL, 0.f);

  return graphene_simd4f_get_z (v->value);
}

/**
 * graphene_vec3_to_float:
 * @v: ...
 * @dest: (out caller-allocates) (array fixed-size=3): ...
 *
 * ...
 *
 * Since: 1.0
 */
void
graphene_vec3_to_float (const graphene_vec3_t *v,
                        float                 *dest)
{
  g_return_if_fail (v != NULL);
  g_return_if_fail (dest != NULL);

  graphene_simd4f_dup_3f (v->value, dest);
}

/**
 * graphene_vec3_add:
 * @a: ...
 * @b: ...
 * @res: (out caller-allocates): ...
 *
 * ...
 *
 * Since: 1.0
 */
void
graphene_vec3_add (const graphene_vec3_t *a,
                   const graphene_vec3_t *b,
                   graphene_vec3_t       *res)
{
  g_return_if_fail (a != NULL && b != NULL);
  g_return_if_fail (res != NULL);

  res->value = graphene_simd4f_add (a->value, b->value);
}

/**
 * graphene_vec3_subtract:
 * @a: ...
 * @b: ...
 * @res: (out caller-allocates): ...
 *
 * ...
 *
 * Since: 1.0
 */
void
graphene_vec3_subtract (const graphene_vec3_t *a,
                        const graphene_vec3_t *b,
                        graphene_vec3_t       *res)
{
  g_return_if_fail (a != NULL && b!= NULL);
  g_return_if_fail (res != NULL);

  res->value = graphene_simd4f_sub (a->value, b->value);
}

/**
 * graphene_vec3_multiply:
 * @a: ...
 * @b: ...
 * @res: (out caller-allocates): ...
 *
 * ...
 *
 * Since: 1.0
 */
void
graphene_vec3_multiply (const graphene_vec3_t *a,
                        const graphene_vec3_t *b,
                        graphene_vec3_t       *res)
{
  g_return_if_fail (a != NULL && b!= NULL);
  g_return_if_fail (res != NULL);

  res->value = graphene_simd4f_mul (a->value, b->value);
}

/**
 * graphene_vec3_divide:
 * @a: ...
 * @b: ...
 * @res: (out caller-allocates): ...
 *
 * ...
 *
 * Since: 1.0
 */
void
graphene_vec3_divide (const graphene_vec3_t *a,
                      const graphene_vec3_t *b,
                      graphene_vec3_t       *res)
{
  g_return_if_fail (a != NULL && b!= NULL);
  g_return_if_fail (res != NULL);

  res->value = graphene_simd4f_div (a->value, b->value);
}

/**
 * graphene_vec3_cross:
 * @a: ...
 * @b: ...
 * @res: (out caller-allocates): ...
 *
 * ...
 *
 * Since: 1.0
 */
void
graphene_vec3_cross (const graphene_vec3_t *a,
                     const graphene_vec3_t *b,
                     graphene_vec3_t       *res)
{
  g_return_if_fail (a != NULL && b != NULL);
  g_return_if_fail (res != NULL);

  res->value = graphene_simd4f_cross3 (a->value, b->value);
}

/**
 * graphene_vec3_dot:
 * @a: ...
 * @b: ...
 *
 * ...
 *
 * Since: 1.0
 */
float
graphene_vec3_dot (const graphene_vec3_t *a,
                   const graphene_vec3_t *b)
{
  g_return_val_if_fail (a != NULL && b != NULL, 0.f);

  return graphene_simd4f_get_x (graphene_simd4f_dot3 (a->value, b->value));
}

float
graphene_vec3_length (const graphene_vec3_t *v)
{
  g_return_val_if_fail (v != NULL, 0.f);

  return graphene_simd4f_get_x (graphene_simd4f_length3 (v->value));
}

/**
 * graphene_vec3_normalize:
 * @v: ...
 * @res: (out caller-allocates): ...
 *
 * ...
 *
 * Since: 1.0
 */
void
graphene_vec3_normalize (const graphene_vec3_t *v,
                         graphene_vec3_t       *res)
{
  g_return_if_fail (v != NULL);
  g_return_if_fail (res != NULL);

  res->value = graphene_simd4f_normalize3 (v->value);
}

/**
 * graphene_vec3_min:
 * @a: ...
 * @b: ...
 * @res: (out caller-allocates): ...
 *
 * ...
 *
 * Since: 1.0
 */
void
graphene_vec3_min (const graphene_vec3_t *a,
                   const graphene_vec3_t *b,
                   graphene_vec3_t       *res)
{
  g_return_if_fail (a != NULL && b != NULL);
  g_return_if_fail (res != NULL);

  res->value = graphene_simd4f_min (a->value, b->value);
}

/**
 * graphene_vec3_max:
 * @a: ...
 * @b: ...
 * @res: (out caller-allocates): ...
 *
 * ...
 *
 * Since: 1.0
 */
void
graphene_vec3_max (const graphene_vec3_t *a,
                   const graphene_vec3_t *b,
                   graphene_vec3_t       *res)
{
  g_return_if_fail (a != NULL && b != NULL);
  g_return_if_fail (res != NULL);

  res->value = graphene_simd4f_max (a->value, b->value);
}

/**
 * graphene_vec3_get_xy:
 * @v: ...
 * @res: (out caller-allocates): ...
 *
 * ...
 *
 * Since: 1.0
 */
void
graphene_vec3_get_xy (const graphene_vec3_t *v,
                      graphene_vec2_t       *res)
{
  g_return_if_fail (v != NULL);
  g_return_if_fail (res != NULL);

  res->value = graphene_simd4f_zero_zw (v->value);
}

/**
 * graphene_vec3_get_xy0:
 * @v: ...
 * @res: (out caller-allocates): ...
 *
 * ...
 *
 * Since: 1.0
 */
void
graphene_vec3_get_xy0 (const graphene_vec3_t *v,
                       graphene_vec3_t       *res)
{
  g_return_if_fail (v != NULL);
  g_return_if_fail (res != NULL);

  res->value = graphene_simd4f_mul (v->value, graphene_simd4f_init (1.f, 1.f, 0.f, 0.f));
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
  g_return_if_fail (v != NULL);
  g_return_if_fail (res != NULL);

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
  g_return_if_fail (v != NULL);
  g_return_if_fail (res != NULL);

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
  g_return_if_fail (v != NULL);
  g_return_if_fail (res != NULL);

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
static gboolean static_vec3_init = FALSE;

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

  static_vec3_init = TRUE;
}

const graphene_vec3_t *
graphene_vec3_zero (void)
{
  init_static_vec3 ();

  return &(static_vec3[VEC3_ZERO]);
}

const graphene_vec3_t *
graphene_vec3_one (void)
{
  init_static_vec3 ();

  return &(static_vec3[VEC3_ONE]);
}

const graphene_vec3_t *
graphene_vec3_x_axis (void)
{
  init_static_vec3 ();

  return &(static_vec3[VEC3_X_AXIS]);
}

const graphene_vec3_t *
graphene_vec3_y_axis (void)
{
  init_static_vec3 ();

  return &(static_vec3[VEC3_Y_AXIS]);
}

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
  g_return_val_if_fail (v != NULL, NULL);

  v->value = graphene_simd4f_init (x, y, z, w);

  return v;
}

graphene_vec4_t *
graphene_vec4_init_from_vec4 (graphene_vec4_t       *v,
                              const graphene_vec4_t *src)
{
  g_return_val_if_fail (v != NULL, NULL);
  g_return_val_if_fail (src != NULL, v);

  v->value = src->value;

  return v;
}

graphene_vec4_t *
graphene_vec4_init_from_vec3 (graphene_vec4_t       *v,
                              const graphene_vec3_t *src)
{
  g_return_val_if_fail (v != NULL, NULL);
  g_return_val_if_fail (src != NULL, v);

  v->value = src->value;

  return v;
}

graphene_vec4_t *
graphene_vec4_init_from_vec2 (graphene_vec4_t       *v,
                              const graphene_vec2_t *src)
{
  g_return_val_if_fail (v != NULL, NULL);
  g_return_val_if_fail (src != NULL, v);

  v->value = src->value;

  return v;
}

/**
 * graphene_vec4_init_from_float:
 * @v: ...
 * @src: (array fixed-size=4): ...
 *
 * ...
 *
 * Returns: (transfer none): ...
 *
 * Since: 1.0
 */
graphene_vec4_t *
graphene_vec4_init_from_float (graphene_vec4_t *v,
                               const float     *src)
{
  g_return_val_if_fail (v != NULL, NULL);
  g_return_val_if_fail (src != NULL, v);

  v->value = graphene_simd4f_init_4f (src);

  return v;
}

float
graphene_vec4_get_x (const graphene_vec4_t *v)
{
  g_return_val_if_fail (v != NULL, 0.f);

  return graphene_simd4f_get_x (v->value);
}

float
graphene_vec4_get_y (const graphene_vec4_t *v)
{
  g_return_val_if_fail (v != NULL, 0.f);

  return graphene_simd4f_get_y (v->value);
}

float
graphene_vec4_get_z (const graphene_vec4_t *v)
{
  g_return_val_if_fail (v != NULL, 0.f);

  return graphene_simd4f_get_z (v->value);
}

float
graphene_vec4_get_w (const graphene_vec4_t *v)
{
  g_return_val_if_fail (v != NULL, 0.f);

  return graphene_simd4f_get_w (v->value);
}

/**
 * graphene_vec4_to_float:
 * @v: ...
 * @dest: (out caller-allocates) (array fixed-size=4): ...
 *
 * ...
 *
 * Since: 1.0
 */
void
graphene_vec4_to_float (const graphene_vec4_t *v,
                        float                 *dest)
{
  g_return_if_fail (v != NULL);
  g_return_if_fail (dest != NULL);

  graphene_simd4f_dup_4f (v->value, dest);
}

/**
 * graphene_vec4_add:
 * @a: ...
 * @b: ...
 * @res: (out caller-allocates): ...
 *
 * ...
 *
 * Since: 1.0
 */
void
graphene_vec4_add (const graphene_vec4_t *a,
                   const graphene_vec4_t *b,
                   graphene_vec4_t       *res)
{
  g_return_if_fail (a != NULL && b != NULL);
  g_return_if_fail (res != NULL);

  res->value = graphene_simd4f_add (a->value, b->value);
}

/**
 * graphene_vec4_subtract:
 * @a: ...
 * @b: ...
 * @res: (out caller-allocates): ...
 *
 * ...
 *
 * Since: 1.0
 */
void
graphene_vec4_subtract (const graphene_vec4_t *a,
                        const graphene_vec4_t *b,
                        graphene_vec4_t       *res)
{
  g_return_if_fail (a != NULL && b!= NULL);
  g_return_if_fail (res != NULL);

  res->value = graphene_simd4f_sub (a->value, b->value);
}

/**
 * graphene_vec4_multiply:
 * @a: ...
 * @b: ...
 * @res: (out caller-allocates): ...
 *
 * ...
 *
 * Since: 1.0
 */
void
graphene_vec4_multiply (const graphene_vec4_t *a,
                        const graphene_vec4_t *b,
                        graphene_vec4_t       *res)
{
  g_return_if_fail (a != NULL && b!= NULL);
  g_return_if_fail (res != NULL);

  res->value = graphene_simd4f_mul (a->value, b->value);
}

/**
 * graphene_vec4_divide:
 * @a: ...
 * @b: ...
 * @res: (out caller-allocates): ...
 *
 * ...
 *
 * Since: 1.0
 */
void
graphene_vec4_divide (const graphene_vec4_t *a,
                      const graphene_vec4_t *b,
                      graphene_vec4_t       *res)
{
  g_return_if_fail (a != NULL && b!= NULL);
  g_return_if_fail (res != NULL);

  res->value = graphene_simd4f_div (a->value, b->value);
}

float
graphene_vec4_dot (const graphene_vec4_t *a,
                   const graphene_vec4_t *b)
{
  g_return_val_if_fail (a != NULL && b != NULL, 0.f);

  return graphene_simd4f_get_x (graphene_simd4f_dot4 (a->value, b->value));
}

float
graphene_vec4_length (const graphene_vec4_t *v)
{
  g_return_val_if_fail (v != NULL, 0.f);

  return graphene_simd4f_get_x (graphene_simd4f_length4 (v->value));
}

/**
 * graphene_vec4_normalize:
 * @v: ...
 * @res: (out caller-allocates): ...
 *
 * ...
 *
 * Since: 1.0
 */
void
graphene_vec4_normalize (const graphene_vec4_t *v,
                         graphene_vec4_t       *res)
{
  g_return_if_fail (v != NULL);
  g_return_if_fail (res != NULL);

  res->value = graphene_simd4f_normalize4 (v->value);
}

/**
 * graphene_vec4_min:
 * @a: ...
 * @b: ...
 * @res: (out caller-allocates): ...
 *
 * ...
 *
 * Since: 1.0
 */
void
graphene_vec4_min (const graphene_vec4_t *a,
                   const graphene_vec4_t *b,
                   graphene_vec4_t       *res)
{
  g_return_if_fail (a != NULL && b != NULL);
  g_return_if_fail (res != NULL);

  res->value = graphene_simd4f_min (a->value, b->value);
}

/**
 * graphene_vec4_max:
 * @a: ...
 * @b: ...
 * @res: (out caller-allocates): ...
 *
 * ...
 *
 * Since: 1.0
 */
void
graphene_vec4_max (const graphene_vec4_t *a,
                   const graphene_vec4_t *b,
                   graphene_vec4_t       *res)
{
  g_return_if_fail (a != NULL && b != NULL);
  g_return_if_fail (res != NULL);

  res->value = graphene_simd4f_max (a->value, b->value);
}

/**
 * graphene_vec4_get_xy:
 * @v: ...
 * @res: (out caller-allocates): ...
 *
 * ...
 *
 * Since: 1.0
 */
void
graphene_vec4_get_xy (const graphene_vec4_t *v,
                      graphene_vec2_t       *res)
{
  g_return_if_fail (v != NULL);
  g_return_if_fail (res != NULL);

  res->value = graphene_simd4f_zero_zw (v->value);
}

/**
 * graphene_vec4_get_xyz:
 * @v: ...
 * @res: (out caller-allocates): ...
 *
 * ...
 *
 * Since: 1.0
 */
void
graphene_vec4_get_xyz (const graphene_vec4_t *v,
                       graphene_vec3_t       *res)
{
  g_return_if_fail (v != NULL);
  g_return_if_fail (res != NULL);

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
static gboolean static_vec4_init = FALSE;

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

  static_vec4_init = TRUE;
}

const graphene_vec4_t *
graphene_vec4_zero (void)
{
  init_static_vec4 ();

  return &(static_vec4[VEC4_ZERO]);
}

const graphene_vec4_t *
graphene_vec4_one (void)
{
  init_static_vec4 ();

  return &(static_vec4[VEC4_ONE]);
}

const graphene_vec4_t *
graphene_vec4_x_axis (void)
{
  init_static_vec4 ();

  return &(static_vec4[VEC4_X_AXIS]);
}

const graphene_vec4_t *
graphene_vec4_y_axis (void)
{
  init_static_vec4 ();

  return &(static_vec4[VEC4_Y_AXIS]);
}

const graphene_vec4_t *
graphene_vec4_z_axis (void)
{
  init_static_vec4 ();

  return &(static_vec4[VEC4_Z_AXIS]);
}

const graphene_vec4_t *
graphene_vec4_w_axis (void)
{
  init_static_vec4 ();

  return &(static_vec4[VEC4_W_AXIS]);
}

/* }}} vec4 */
