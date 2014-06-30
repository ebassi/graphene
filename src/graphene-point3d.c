/* graphene-point3d.hc: Point in 3D space
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

/**
 * SECTION:graphene-point3d
 * @Title: Point3D
 * @Short_Description: A point with 3 coordinates
 *
 * #graphene_point3d_t is a data structure capable of describing a point with
 * three coordinates:
 *
 *  * @graphene_point3d_t.x
 *  * @graphene_point3d_t.y
 *  * @graphene_point3d_t.z
 */

#include "graphene-private.h"
#include "graphene-point3d.h"
#include "graphene-simd4f.h"
#include "graphene-vec3.h"

#include <math.h>

/**
 * graphene_point3d_alloc: (constructor)
 *
 * Allocates a #graphene_point3d_t structure.
 *
 * Returns: (transfer full): the newly allocated structure.
 *   Use graphene_point3d_free() to free the resources
 *   allocated by this function.
 *
 * Since: 1.0
 */
graphene_point3d_t *
graphene_point3d_alloc (void)
{
  return calloc (1, sizeof (graphene_point3d_t));
}

/**
 * graphene_point3d_free:
 * @p: a #graphene_point3d_t
 *
 * Frees the resources allocated via graphene_point3d_alloc().
 *
 * Since: 1.0
 */
void
graphene_point3d_free (graphene_point3d_t *p)
{
  free (p);
}

/**
 * graphene_point3d_init:
 * @p: the #graphene_point3d_t to initialize
 * @x: the X coordinate of the point
 * @y: the Y coordinate of the point
 * @z: the Z coordinate of the point
 *
 * Initializes a #graphene_point3d_t with the given coordinates.
 *
 * Returns: (transfer none): the initialized #graphene_point3d_t
 *
 * Since: 1.0
 */
graphene_point3d_t *
graphene_point3d_init (graphene_point3d_t *p,
                       float               x,
                       float               y,
                       float               z)
{
  p->x = x;
  p->y = y;
  p->z = z;

  return p;
}

static graphene_point3d_t *
graphene_point3d_init_from_simd4f (graphene_point3d_t *p,
                                   graphene_simd4f_t   v)
{
  p->x = graphene_simd4f_get_x (v);
  p->y = graphene_simd4f_get_y (v);
  p->z = graphene_simd4f_get_z (v);

  return p;
}

/**
 * graphene_point3d_init_from_point:
 * @p: a #graphene_point3d_t
 * @src: a #graphene_point3d_t
 *
 * Initializes a #graphene_point3d_t using the coordinates of
 * another #graphene_point3d_t.
 *
 * Returns: (transfer none): the initialized point
 *
 * Since: 1.0
 */
graphene_point3d_t *
graphene_point3d_init_from_point (graphene_point3d_t       *p,
                                  const graphene_point3d_t *src)
{
  *p = *src;

  return p;
}

/**
 * graphene_point3d_init_from_vec3:
 * @p: a #graphene_point3d_t
 * @v: a #graphene_vec3_t
 *
 * Initializes a #graphene_point3d_t using the components
 * of a #graphene_vec3_t.
 *
 * Returns: (transfer none): the initialized #graphene_point3d_t
 *
 * Since: 1.0
 */
graphene_point3d_t *
graphene_point3d_init_from_vec3 (graphene_point3d_t    *p,
                                 const graphene_vec3_t *v)
{
  return graphene_point3d_init_from_simd4f (p, v->value);
}

/**
 * graphene_point3d_to_vec3:
 * @p: a #graphene_point3d_t
 * @v: (out caller-allocates): return location for a #graphene_vec3_t
 *
 * Stores the coordinates of a #graphene_point3d_t into a
 * #graphene_vec3_t.
 *
 * Since: 1.0
 */
void
graphene_point3d_to_vec3 (const graphene_point3d_t *p,
                          graphene_vec3_t          *v)
{
  v->value = graphene_simd4f_init (p->x, p->y, p->z, 0.f);
}

/**
 * graphene_point3d_equal:
 * @a: a #graphene_point3d_t
 * @b: a #graphene_point3d_t
 *
 * Checks whether two given points are equal.
 *
 * Returns: %true if the points are equal
 *
 * Since: 1.0
 */
bool
graphene_point3d_equal (const graphene_point3d_t *a,
                        const graphene_point3d_t *b)
{
  if (a == b)
    return true;

  if (a == NULL || b == NULL)
    return false;

  return graphene_point3d_near (a, b, GRAPHENE_FLOAT_EPSILON);
}

/**
 * graphene_point3d_near:
 * @a: a #graphene_point3d_t
 * @b: a #graphene_point3d_t
 * @epsilon: fuzzyness factor
 *
 * Checks whether the two points are near each other, within
 * an @epsilon factor.
 *
 * Returns: %true if the points are near each other
 *
 * Since: 1.0
 */
bool
graphene_point3d_near (const graphene_point3d_t *a,
                       const graphene_point3d_t *b,
                       float                     epsilon)
{
  graphene_simd4f_t v_a, v_b, v_res;

  if (a == b)
    return true;

  v_a = graphene_simd4f_init (a->x, a->y, a->z, 0.f);
  v_b = graphene_simd4f_init (b->x, b->y, b->z, 0.f);
  v_res = graphene_simd4f_sub (v_a, v_b);

  return fabsf (graphene_simd4f_get_x (v_res)) < epsilon &&
         fabsf (graphene_simd4f_get_y (v_res)) < epsilon &&
         fabsf (graphene_simd4f_get_z (v_res)) < epsilon;
}

/**
 * graphene_point3d_scale:
 * @p: a #graphene_point3d_t
 * @factor: the scaling factor
 * @res: (out caller-allocates): return location for the scaled point
 *
 * Scales the coordinates of the given #graphene_point3d_t by
 * the given @factor.
 *
 * Since: 1.0
 */
void
graphene_point3d_scale (const graphene_point3d_t *p,
                        float                     factor,
                        graphene_point3d_t       *res)
{
  graphene_simd4f_t v;

  v = graphene_simd4f_init (p->x, p->y, p->z, 0.f);
  v = graphene_simd4f_mul (v, graphene_simd4f_splat (factor));

  graphene_point3d_init_from_simd4f (res, v);
}

/**
 * graphene_point3d_cross:
 * @a: a #graphene_point3d_t
 * @b: a #graphene_point3d_t
 * @res: (out caller-allocates): return location for the cross
 *   product
 *
 * Computes the cross product of the two given #graphene_point3d_t.
 *
 * Since: 1.0
 */
void
graphene_point3d_cross (const graphene_point3d_t *a,
                        const graphene_point3d_t *b,
                        graphene_point3d_t       *res)
{
  graphene_simd4f_t av, bv, resv;

  av = graphene_simd4f_init (a->x, a->y, a->z, 0.f);
  bv = graphene_simd4f_init (b->x, b->y, b->z, 0.f);
  resv = graphene_simd4f_cross3 (av, bv);

  graphene_point3d_init_from_simd4f (res, resv);
}

/**
 * graphene_point3d_dot:
 * @a: a #graphene_point3d_t
 * @b: a #graphene_point3d_t
 *
 * Computes the dot product of the two given #graphene_point3d_t.
 *
 * Returns: the value of the dot product
 *
 * Since: 1.0
 */
float
graphene_point3d_dot (const graphene_point3d_t *a,
                      const graphene_point3d_t *b)
{
  graphene_simd4f_t av, bv, resv;

  av = graphene_simd4f_init (a->x, a->y, a->z, 0.f);
  bv = graphene_simd4f_init (b->x, b->y, b->z, 0.f);
  resv = graphene_simd4f_dot3 (av, bv);

  return graphene_simd4f_get_x (resv);
}

/**
 * graphene_point3d_length:
 * @p: a #graphene_point3d_t
 *
 * Computes the length of the vector represented by the
 * coordinates of the given #graphene_point3d_t.
 *
 * Returns: the length of the vector represented by the point
 *
 * Since: 1.0
 */
float
graphene_point3d_length (const graphene_point3d_t *p)
{
  graphene_simd4f_t res;

  res = graphene_simd4f_init (p->x, p->y, p->z, 0.f);

  return graphene_simd4f_get_x (graphene_simd4f_length3 (res));
}

/**
 * graphene_point3d_normalize:
 * @p: a #graphene_point3d_t
 * @res: (out caller-allocates): return location for the normalized
 *   #graphene_point3d_t
 *
 * Computes the normalization of the vector represented by the
 * coordinates of the given #graphene_point3d_t.
 *
 * Since: 1.0
 */
void
graphene_point3d_normalize (const graphene_point3d_t *p,
                            graphene_point3d_t       *res)
{
  graphene_simd4f_t v;

  v = graphene_simd4f_init (p->x, p->y, p->z, 0.f);
  v = graphene_simd4f_normalize3 (v);

  graphene_point3d_init_from_simd4f (res, v);
}

/**
 * graphene_point3d_interpolate:
 * @a: a #graphene_point3d_t
 * @b: a #graphene_point3d_t
 * @factor: the interpolation factor
 * @res: (out caller-allocates): the return location for the
 *   interpolated #graphene_point3d_t
 *
 * Linearly interpolates each component of @a and @b using the
 * provided @factor, and places the result in @res.
 *
 * Since: 1.0
 */
void
graphene_point3d_interpolate (const graphene_point3d_t *a,
                              const graphene_point3d_t *b,
                              double                    factor,
                              graphene_point3d_t       *res)
{
  graphene_simd4f_t a_v, b_v, r_v;

  /* linear interpolation: r = a + (b - a) * f */
  a_v = graphene_simd4f_init (a->x, a->y, a->z, 0.f);
  b_v = graphene_simd4f_init (b->x, b->y, b->z, 0.f);
  r_v = graphene_simd4f_add (a_v,
                             graphene_simd4f_mul (graphene_simd4f_sub (b_v, a_v),
                                                  graphene_simd4f_splat (factor)));

  graphene_point3d_init_from_simd4f (res, r_v);
}

static const graphene_point3d_t _graphene_point3d_zero = GRAPHENE_POINT3D_INIT_ZERO;

/**
 * graphene_point3d_zero:
 *
 * Retrieves a constant point with all three coordinates set to 0.
 *
 * Returns: (transfer none): a zero point
 *
 * Since: 1.0
 */
const graphene_point3d_t *
graphene_point3d_zero (void)
{
  return &_graphene_point3d_zero;
}
