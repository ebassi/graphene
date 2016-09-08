/* graphene-matrix.h: 4x4 matrix
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
 * SECTION:graphene-matrix
 * @title: Matrix
 * @short_description: 4x4 matrices
 *
 * #graphene_matrix_t is a type that provides a 4x4 square matrix, useful for
 * representing 3D transformations.
 *
 * The matrix is treated as row-major, i.e. it has four vectors (x, y, z, and
 * w) representing rows, and elements of each vector are a column:
 *
 * |[<!-- language="plain" -->
 *   | x |    | x.x   x.y   x.z   x.w |
 *   | y | -\ | y.x   y.y   y.z   y.w |
 *   | z | -/ | z.x   z.y   z.z   z.w |
 *   | w |    | w.x   w.y   w.z   w.w |
 * ]|
 *
 * It is possible to easily convert a #graphene_matrix_t to and from an array
 * of floating point values that can be used with other libraries.
 *
 * The contents of a #graphene_matrix_t are private, and direct access is not
 * possible. You can modify and read the contents of a #graphene_matrix_t
 * only through the provided API.
 */

#include "graphene-private.h"

#include "graphene-matrix.h"

#include "graphene-alloc-private.h"
#include "graphene-box.h"
#include "graphene-euler.h"
#include "graphene-point.h"
#include "graphene-point3d.h"
#include "graphene-quad.h"
#include "graphene-quaternion.h"
#include "graphene-ray.h"
#include "graphene-rect.h"
#include "graphene-simd4x4f.h"
#include "graphene-sphere.h"
#include "graphene-vectors-private.h"

#include <stdio.h>

/**
 * graphene_matrix_alloc: (constructor)
 *
 * Allocates a new #graphene_matrix_t.
 *
 * Returns: (transfer full): the newly allocated matrix
 *
 * Since: 1.0
 */
graphene_matrix_t *
graphene_matrix_alloc (void)
{
  return graphene_aligned_alloc (sizeof (graphene_matrix_t), 1, 16);
}

/**
 * graphene_matrix_free:
 * @m: a #graphene_matrix_t
 *
 * Frees the resources allocated by graphene_matrix_alloc().
 *
 * Since: 1.0
 */
void
graphene_matrix_free (graphene_matrix_t *m)
{
  graphene_aligned_free (m);
}

/**
 * graphene_matrix_to_float:
 * @m: a #graphene_matrix_t
 * @v: (array fixed-size=16) (out caller-allocates): return location
 *   for an array of floating point values. The array must be capable
 *   of holding at least 16 values.
 *
 * Converts a #graphene_matrix_t to an array of floating point
 * values.
 *
 * Since: 1.0
 */
void
graphene_matrix_to_float (const graphene_matrix_t *m,
                          float                   *v)
{
  graphene_simd4x4f_to_float (&m->value, v);
}

static const float graphene_identity_matrix_floats[16] = {
  1.f, 0.f, 0.f, 0.f,
  0.f, 1.f, 0.f, 0.f,
  0.f, 0.f, 1.f, 0.f,
  0.f, 0.f, 0.f, 1.f,
};

/**
 * graphene_matrix_init_identity:
 * @m: a #graphene_matrix_t
 *
 * Initializes a #graphene_matrix_t with the identity matrix.
 *
 * Returns: (transfer none): the initialized matrix
 *
 * Since: 1.0
 */
graphene_matrix_t *
graphene_matrix_init_identity (graphene_matrix_t *m)
{
  graphene_simd4x4f_init_from_float (&m->value, graphene_identity_matrix_floats);

  return m;
}

/**
 * graphene_matrix_init_from_float:
 * @m: a #graphene_matrix_t
 * @v: (array fixed-size=16): an array of at least 16 floating
 *   point values
 *
 * Initializes a #graphene_matrix_t with the given array of floating
 * point values.
 *
 * Returns: (transfer none): the initialized matrix
 *
 * Since: 1.0
 */
graphene_matrix_t *
graphene_matrix_init_from_float (graphene_matrix_t *m,
                                 const float       *v)
{
  graphene_simd4x4f_init_from_float (&m->value, v);

  return m;
}

/**
 * graphene_matrix_init_from_vec4:
 * @m: a #graphene_matrix_t
 * @v0: the first row vector
 * @v1: the second row vector
 * @v2: the third row vector
 * @v3: the fourth row vector
 *
 * Initializes a #graphene_matrix_t with the given four row
 * vectors.
 *
 * Returns: (transfer none): the initialized matrix
 *
 * Since: 1.0
 */
graphene_matrix_t *
graphene_matrix_init_from_vec4 (graphene_matrix_t     *m,
                                const graphene_vec4_t *v0,
                                const graphene_vec4_t *v1,
                                const graphene_vec4_t *v2,
                                const graphene_vec4_t *v3)
{
  m->value = graphene_simd4x4f_init (v0->value,
                                     v1->value,
                                     v2->value,
                                     v3->value);

  return m;
}

/**
 * graphene_matrix_init_from_matrix:
 * @m: a #graphene_matrix_t
 * @src: a #graphene_matrix_t
 *
 * Initializes a #graphene_matrix_t using the values of the
 * given matrix.
 *
 * Returns: (transfer none): the initialized matrix
 *
 * Since: 1.0
 */
graphene_matrix_t *
graphene_matrix_init_from_matrix (graphene_matrix_t       *m,
                                  const graphene_matrix_t *src)
{
  m->value = src->value;

  return m;
}

/**
 * graphene_matrix_init_perspective:
 * @m: a #graphene_matrix_t
 * @fovy: the field of view angle, in degrees
 * @aspect: the aspect value
 * @z_near: the near Z plane
 * @z_far: the far Z plane
 *
 * Initializes a #graphene_matrix_t with a perspective projection.
 *
 * Returns: (transfer none): the initialized matrix
 *
 * Since: 1.0
 */
graphene_matrix_t *
graphene_matrix_init_perspective (graphene_matrix_t *m,
                                  float              fovy,
                                  float              aspect,
                                  float              z_near,
                                  float              z_far)
{
  float fovy_rad = GRAPHENE_DEG_TO_RAD (fovy);

  graphene_simd4x4f_init_perspective (&m->value, fovy_rad, aspect, z_near, z_far);

  return m;
}

/**
 * graphene_matrix_init_ortho:
 * @m: a #graphene_matrix_t
 * @left: the left edge of the clipping plane
 * @right: the right edge of the clipping plane
 * @top: the top edge of the clipping plane
 * @bottom: the bottom edge of the clipping plane
 * @z_near: the distance of the near clipping plane
 * @z_far: the distance of the far clipping plane
 *
 * Initializes a #graphene_matrix_t with an orthographic projection.
 *
 * Returns: (transfer none): the initialized matrix
 *
 * Since: 1.0
 */
graphene_matrix_t *
graphene_matrix_init_ortho (graphene_matrix_t *m,
                            float              left,
                            float              right,
                            float              top,
                            float              bottom,
                            float              z_near,
                            float              z_far)
{
  graphene_simd4x4f_init_ortho (&m->value, left, right, top, bottom, z_near, z_far);

  return m;
}

/**
 * graphene_matrix_init_look_at:
 * @m: a #graphene_matrix_t
 * @eye: the vector describing the position to look from
 * @center: the vector describing the position to look at
 * @up: the vector describing the world's upward direction; usually,
 *   this is the graphene_vec3_y_axis() vector
 *
 * Initializes a #graphene_matrix_t so that it positions the "camera"
 * at the given @eye coordinates towards an object at the @center
 * coordinates. The top of the camera is aligned to the direction
 * of the @up vector.
 *
 * Returns: (transfer none): the initialized matrix
 *
 * Since: 1.0
 */
graphene_matrix_t *
graphene_matrix_init_look_at (graphene_matrix_t     *m,
                              const graphene_vec3_t *eye,
                              const graphene_vec3_t *center,
                              const graphene_vec3_t *up)
{
  graphene_simd4x4f_init_look_at (&m->value, eye->value, center->value, up->value);

  return m;
}

/**
 * graphene_matrix_init_frustum:
 * @m: a #graphene_matrix_t
 * @left: distance of the left clipping plane
 * @right: distance of the right clipping plane
 * @bottom: distance of the bottom clipping plane
 * @top: distance of the top clipping plane
 * @z_near: distance of the near clipping plane
 * @z_far: distance of the far clipping plane
 *
 * Initializes a #graphene_matrix_t compatible with #graphene_frustum_t.
 *
 * See also: graphene_frustum_init_from_matrix()
 *
 * Returns: (transfer none): the initialized matrix
 *
 * Since: 1.2
 */
graphene_matrix_t *
graphene_matrix_init_frustum (graphene_matrix_t *m,
                              float              left,
                              float              right,
                              float              bottom,
                              float              top,
                              float              z_near,
                              float              z_far)
{
  graphene_simd4x4f_init_frustum (&m->value, left, right, bottom, top, z_near, z_far);

  return m;
}

/**
 * graphene_matrix_init_scale:
 * @m: a #graphene_matrix_t
 * @x: the scale factor on the X axis
 * @y: the scale factor on the Y axis
 * @z: the scale factor on the Z axis
 *
 * Initializes a #graphene_matrix_t with the given scaling factors.
 *
 * Returns: (transfer none): the initialized matrix
 *
 * Since: 1.0
 */
graphene_matrix_t *
graphene_matrix_init_scale (graphene_matrix_t *m,
                            float              x,
                            float              y,
                            float              z)
{
  m->value =
    graphene_simd4x4f_init (graphene_simd4f_init (   x, 0.0f, 0.0f, 0.0f),
                            graphene_simd4f_init (0.0f,    y, 0.0f, 0.0f),
                            graphene_simd4f_init (0.0f, 0.0f,    z, 0.0f),
                            graphene_simd4f_init (0.0f, 0.0f, 0.0f, 1.0f));

  return m;
}

/**
 * graphene_matrix_init_translate:
 * @m: a #graphene_matrix_t
 * @p: the translation coordinates
 *
 * Initializes a #graphene_matrix_t with a translation to the
 * given coordinates.
 *
 * Returns: (transfer none): the initialized matrix
 *
 * Since: 1.0
 */
graphene_matrix_t *
graphene_matrix_init_translate (graphene_matrix_t        *m,
                                const graphene_point3d_t *p)
{
  m->value =
    graphene_simd4x4f_init (graphene_simd4f_init (1.0f, 0.0f, 0.0f, 0.0f),
                            graphene_simd4f_init (0.0f, 1.0f, 0.0f, 0.0f),
                            graphene_simd4f_init (0.0f, 0.0f, 1.0f, 0.0f),
                            graphene_simd4f_init (p->x, p->y, p->z, 1.0f));

  return m;
}

/**
 * graphene_matrix_init_skew:
 * @m: a #graphene_matrix_t
 * @x_skew: skew factor on the X axis
 * @y_skew: skew factor on the Y axis
 *
 * Initializes a #graphene_matrix_t with a skew transformation
 * with the given factors.
 *
 * Returns: (transfer none): the initialized matrix
 *
 * Since: 1.0
 */
graphene_matrix_t *
graphene_matrix_init_skew (graphene_matrix_t *m,
                           float              x_skew,
                           float              y_skew)
{
  float t_x, t_y;

  t_x = tanf (x_skew);
  t_y = tanf (y_skew);

  m->value =
    graphene_simd4x4f_init (graphene_simd4f_init (1.0f,  t_y, 0.0f, 0.0f),
                            graphene_simd4f_init ( t_x, 1.0f, 0.0f, 0.0f),
                            graphene_simd4f_init (0.0f, 0.0f, 1.0f, 0.0f),
                            graphene_simd4f_init (0.0f, 0.0f, 0.0f, 1.0f));

  return m;
}

/**
 * graphene_matrix_init_rotate:
 * @m: a #graphene_matrix_t
 * @angle: the rotation angle, in degrees
 * @axis: the axis vector as a #graphene_vec3_t
 *
 * Initializes @m to represent a rotation of @angle degrees on
 * the axis represented by the @axis vector.
 *
 * Returns: (transfer none): the initialized matrix
 *
 * Since: 1.0
 */
graphene_matrix_t *
graphene_matrix_init_rotate (graphene_matrix_t     *m,
                             float                  angle,
                             const graphene_vec3_t *axis)
{
  float rad = GRAPHENE_DEG_TO_RAD (angle);

  graphene_simd4x4f_rotation (&m->value, rad, axis->value);

  return m;
}

/**
 * graphene_matrix_is_identity:
 * @m: a #graphene_matrix_t
 *
 * Checks whether the given #graphene_matrix_t is the identity matrix.
 *
 * Returns: `true` if the matrix is the identity matrix
 *
 * Since: 1.0
 */
bool
graphene_matrix_is_identity (const graphene_matrix_t *m)
{
  return graphene_simd4x4f_is_identity (&m->value);
}

/**
 * graphene_matrix_is_2d:
 * @m: a #graphene_matrix_t
 *
 * Checks whether the given #graphene_matrix_t is compatible with an
 * a 2D affine transformation matrix.
 *
 * Returns: `true` if the matrix is compatible with an affine
 *   transformation matrix
 *
 * Since: 1.0
 */
bool
graphene_matrix_is_2d (const graphene_matrix_t *m)
{
#if 0
  float res[4];

  graphene_simd4f_dup_4f (m->value.x, res);
  if (!(graphene_fuzzy_equals (res[2], 0.f, 0.000001) &&
        graphene_fuzzy_equals (res[3], 0.f, 0.000001)))
    return false;

  graphene_simd4f_dup_4f (m->value.y, res);
  if (!(graphene_fuzzy_equals (res[2], 0.f, 0.000001) &&
        graphene_fuzzy_equals (res[3], 0.f, 0.000001)))
    return false;

  graphene_simd4f_dup_4f (m->value.z, res);
  if (!(graphene_fuzzy_equals (res[0], 0.f, 0.000001) &&
        graphene_fuzzy_equals (res[1], 0.f, 0.000001) &&
        graphene_fuzzy_equals (res[2], 1.f, 0.000001) &&
        graphene_fuzzy_equals (res[3], 0.f, 0.000001)))
    return false;

  graphene_simd4f_dup_4f (m->value.w, res);
  if (!(graphene_fuzzy_equals (res[2], 0.f, 0.000001) &&
        graphene_fuzzy_equals (res[3], 1.f, 0.000001)))
    return false;

  return true;
#else
  return graphene_simd4x4f_is_2d (&m->value);
#endif
}

/**
 * graphene_matrix_is_backface_visible:
 * @m: a #graphene_matrix_t
 *
 * Checks whether a #graphene_matrix_t has a visible back face.
 *
 * Returns: `true` if the back face of the matrix is visible
 *
 * Since: 1.0
 */
bool
graphene_matrix_is_backface_visible (const graphene_matrix_t *m)
{
  graphene_simd4x4f_t tmp;

  if (!graphene_simd4x4f_inverse (&m->value, &tmp))
    return false;

  /* inverse.zz < 0 */
  return graphene_simd4f_get_z (tmp.z) < 0.f;
}

/**
 * graphene_matrix_is_singular:
 * @m: a #graphene_matrix_t
 *
 * Checks whether a matrix is singular.
 *
 * Returns: `true` if the matrix is singular
 *
 * Since: 1.0
 */
bool
graphene_matrix_is_singular (const graphene_matrix_t *m)
{
  graphene_simd4f_t det;

  graphene_simd4x4f_determinant (&m->value, &det, NULL);

  return fabsf (graphene_simd4f_get_x (det)) <= GRAPHENE_FLOAT_EPSILON;
}

/**
 * graphene_matrix_init_from_2d:
 * @m: a #graphene_matrix_t
 * @xx: the xx member
 * @yx: the yx member
 * @xy: the xy member
 * @yy: the yy member
 * @x_0: the x0 member
 * @y_0: the y0 member
 *
 * Initializes a #graphene_matrix_t from the values of an affine
 * transformation matrix.
 *
 * The arguments map to the following matrix layout:
 *
 * |[<!-- language="plain" -->
 *   | xx yx |   |  a  b  0 |
 *   | xy yy | = |  c  d  0 |
 *   | x0 y0 |   | tx ty  1 |
 * ]|
 *
 * This function can be used to convert between a matrix type from
 * other libraries and a #graphene_matrix_t.
 *
 * Returns: (transfer none): the initialized matrix
 *
 * Since: 1.0
 */
graphene_matrix_t *
graphene_matrix_init_from_2d (graphene_matrix_t *m,
                              double             xx,
                              double             yx,
                              double             xy,
                              double             yy,
                              double             x_0,
                              double             y_0)
{
  m->value = graphene_simd4x4f_init (graphene_simd4f_init ( xx,  yx, 0.f, 0.f),
                                     graphene_simd4f_init ( xy,  yy, 0.f, 0.f),
                                     graphene_simd4f_init (0.f, 0.f, 1.f, 0.f),
                                     graphene_simd4f_init (x_0, y_0, 0.f, 1.f));

  return m;
}

/**
 * graphene_matrix_to_2d:
 * @m: a #graphene_matrix_t
 * @xx: (out): return location for the xx member
 * @yx: (out): return location for the yx member
 * @xy: (out): return location for the xy member
 * @yy: (out): return location for the yy member
 * @x_0: (out): return location for the x0 member
 * @y_0: (out): return location for the y0 member
 *
 * Converts a #graphene_matrix_t to an affine transformation
 * matrix, if the given matrix is compatible.
 *
 * The returned values have the following layout:
 *
 * |[<!-- language="plain" -->
 *   | xx yx |   |  a  b  0 |
 *   | xy yy | = |  c  d  0 |
 *   | x0 y0 |   | tx ty  1 |
 * ]|
 *
 * This function can be used to convert between a #graphene_matrix_t
 * and a matrix type from other libraries.
 *
 * Returns: `true` if the matrix is compatible with an affine
 *   transformation matrix
 *
 * Since: 1.0
 */
bool
graphene_matrix_to_2d (const graphene_matrix_t *m,
                       double                  *xx,
                       double                  *yx,
                       double                  *xy,
                       double                  *yy,
                       double                  *x_0,
                       double                  *y_0)
{
  float res[4];

  graphene_simd4f_dup_4f (m->value.x, res);
  if (res[2] != 0.f && res[3] != 0.f)
    return false;

  if (xx != NULL)
    *xx = res[0];
  if (yx != NULL)
    *yx = res[1];

  graphene_simd4f_dup_4f (m->value.y, res);
  if (res[2] != 0.f && res[3] != 0.f)
    return false;

  if (xy != NULL)
    *xy = res[0];
  if (yy != NULL)
    *yy = res[1];

  if (graphene_simd4f_cmp_neq (m->value.z, graphene_simd4f_init (0.f, 0., 1.f, 0.f)))
    return false;

  graphene_simd4f_dup_4f (m->value.w, res);
  if (res[2] != 0.f && res[3] != 1.f)
    return false;

  if (x_0 != NULL)
    *x_0 = res[0];
  if (y_0 != NULL)
    *y_0 = res[1];

  return true;
}

/**
 * graphene_matrix_get_row:
 * @m: a #graphene_matrix_t
 * @index_: the index of the row vector, between 0 and 3
 * @res: (out caller-allocates): return location for the #graphene_vec4_t
 *   that is used to store the row vector
 *
 * Retrieves the given row vector at @index_ inside a matrix.
 *
 * Since: 1.0
 */
void
graphene_matrix_get_row (const graphene_matrix_t *m,
                         unsigned int             index_,
                         graphene_vec4_t         *res)
{
  switch (index_)
    {
    case 0:
      res->value = m->value.x;
      break;

    case 1:
      res->value = m->value.y;
      break;

    case 2:
      res->value = m->value.z;
      break;

    case 3:
      res->value = m->value.w;
      break;

    default:
      res->value = graphene_simd4f_init_zero ();
      break;
    }
}

/**
 * graphene_matrix_get_value:
 * @m: a #graphene_matrix_t
 * @row: the row index
 * @col: the column index
 *
 * Retrieves the value at the given @row and @col index.
 *
 * Returns: the value at the given indices
 *
 * Since: 1.0
 */
float
graphene_matrix_get_value (const graphene_matrix_t *m,
                           unsigned int             row,
                           unsigned int             col)
{
  graphene_simd4f_t r;

  if (row > 3 || col > 3)
    return 0.f;

  switch (row)
    {
    case 0:
      r = m->value.x;
      break;

    case 1:
      r = m->value.y;
      break;

    case 2:
      r = m->value.z;
      break;

    case 3:
      r = m->value.w;
      break;

    default:
      return 0.f;
    }

  switch (col)
    {
    case 0:
      return graphene_simd4f_get (r, 0);

    case 1:
      return graphene_simd4f_get (r, 1);

    case 2:
      return graphene_simd4f_get (r, 2);

    case 3:
      return graphene_simd4f_get (r, 3);

    default:
      return 0.f;
    }

  return 0.f;
}

/**
 * graphene_matrix_multiply:
 * @a: a #graphene_matrix_t
 * @b: a #graphene_matrix_t
 * @res: (out caller-allocates): return location for the matrix
 *   result
 *
 * Multiplies two #graphene_matrix_t.
 *
 * Remember: matrix multiplication is not commutative, except for the
 * identity matrix; the product of this multiplication is (A * B).
 *
 * Since: 1.0
 */
void
graphene_matrix_multiply (const graphene_matrix_t *a,
                          const graphene_matrix_t *b,
                          graphene_matrix_t       *res)
{
  graphene_simd4x4f_matrix_mul (&a->value, &b->value, &res->value);
}

/**
 * graphene_matrix_determinant:
 * @m: a #graphene_matrix_t
 *
 * Computes the determinant of the given matrix.
 *
 * Returns: the value of the determinant
 *
 * Since: 1.0
 */
float
graphene_matrix_determinant (const graphene_matrix_t *m)
{
  graphene_simd4f_t det;

  graphene_simd4x4f_determinant (&m->value, &det, NULL);

  return graphene_simd4f_get_x (det);
}

/**
 * graphene_matrix_transform_vec3:
 * @m: a #graphene_matrix_t
 * @v: a #graphene_vec3_t
 * @res: (out caller-allocates): return location for a #graphene_vec3_t
 *
 * Transforms the given #graphene_vec3_t using the matrix @m.
 *
 * Since: 1.0
 */
void
graphene_matrix_transform_vec3 (const graphene_matrix_t *m,
                                const graphene_vec3_t   *v,
                                graphene_vec3_t         *res)
{
  graphene_simd4x4f_vec3_mul (&m->value, &v->value, &res->value);
}

/**
 * graphene_matrix_transform_vec4:
 * @m: a #graphene_matrix_t
 * @v: a #graphene_vec4_t
 * @res: (out caller-allocates): return location for a #graphene_vec4_t
 *
 * Transforms the given #graphene_vec4_t using the matrix @m.
 *
 * Since: 1.0
 */
void
graphene_matrix_transform_vec4 (const graphene_matrix_t *m,
                                const graphene_vec4_t   *v,
                                graphene_vec4_t         *res)
{
  graphene_simd4x4f_vec4_mul (&m->value, &v->value, &res->value);
}

/**
 * graphene_matrix_transform_point:
 * @m: a #graphene_matrix_t
 * @p: a #graphene_point_t
 * @res: (out caller-allocates): return location for the
 *   transformed #graphene_point_t
 *
 * Transforms the given #graphene_point_t using the matrix @m.
 *
 * Since: 1.0
 */
void
graphene_matrix_transform_point (const graphene_matrix_t *m,
                                 const graphene_point_t  *p,
                                 graphene_point_t        *res)
{
  graphene_simd4f_t vec3;

  vec3 = graphene_simd4f_init (p->x, p->y, 0.0f, 0.0f);
  graphene_simd4x4f_vec3_mul (&m->value, &vec3, &vec3);

  res->x = graphene_simd4f_get_x (vec3);
  res->y = graphene_simd4f_get_y (vec3);
}

/**
 * graphene_matrix_transform_point3d:
 * @m: a #graphene_matrix_t
 * @p: a #graphene_point3d_t
 * @res: (out caller-allocates): return location for the result
 *
 * Transforms the given #graphene_point3d_t using the matrix @m.
 *
 * Unlike graphene_matrix_transform_vec3(), this function will take into
 * account the fourth row vector of the #graphene_matrix_t.
 *
 * Since: 1.2
 */
void
graphene_matrix_transform_point3d (const graphene_matrix_t  *m,
                                   const graphene_point3d_t *p,
                                   graphene_point3d_t       *res)
{
  graphene_simd4f_t vec3;

  vec3 = graphene_simd4f_init (p->x, p->y, p->z, 0.f);
  graphene_simd4x4f_point3_mul (&m->value, &vec3, &vec3);

  res->x = graphene_simd4f_get_x (vec3);
  res->y = graphene_simd4f_get_y (vec3);
  res->z = graphene_simd4f_get_z (vec3);
}

/**
 * graphene_matrix_transform_rect:
 * @m: a #graphene_matrix_t
 * @r: a #graphene_rect_t
 * @res: (out caller-allocates): return location for the
 *   transformed quad
 *
 * Transforms a #graphene_rect_t using the given matrix @m. The
 * result is a coplanar quad.
 *
 * Since: 1.0
 */
void
graphene_matrix_transform_rect (const graphene_matrix_t *m,
                                const graphene_rect_t   *r,
                                graphene_quad_t         *res)
{
  graphene_point_t ret[4];

#define TRANSFORM_POINT(matrix,rect,corner,out_p)   do { \
  graphene_simd4f_t __s; \
  graphene_point_t __p; \
  graphene_rect_get_ ## corner (rect, &__p); \
  __s = graphene_simd4f_init (__p.x, __p.y, 0.f, 1.f); \
  graphene_simd4x4f_vec4_mul (&matrix->value, &__s, &__s); \
  out_p.x = graphene_simd4f_get_x (__s); \
  out_p.y = graphene_simd4f_get_y (__s);           } while (0)

  TRANSFORM_POINT (m, r, top_left, ret[0]);
  TRANSFORM_POINT (m, r, top_right, ret[1]);
  TRANSFORM_POINT (m, r, bottom_right, ret[2]);
  TRANSFORM_POINT (m, r, bottom_left, ret[3]);

#undef TRANSFORM_POINT

  graphene_quad_init (res, &ret[0], &ret[1], &ret[2], &ret[3]);
}

/**
 * graphene_matrix_transform_bounds:
 * @m: a #graphene_matrix_t
 * @r: a #graphene_rect_t
 * @res: (out caller-allocates): return location for the bounds
 *   of the transformed rectangle
 *
 * Transforms a #graphene_rect_t using the given matrix @m. The
 * result is the bounding box containing the coplanar quad.
 *
 * Since: 1.0
 */
void
graphene_matrix_transform_bounds (const graphene_matrix_t *m,
                                  const graphene_rect_t   *r,
                                  graphene_rect_t         *res)
{
  graphene_point_t ret[4];
  float min_x, min_y;
  float max_x, max_y;

#define TRANSFORM_POINT(matrix,rect,corner,out_p)   do { \
  graphene_simd4f_t __s; \
  graphene_point_t __p; \
  graphene_rect_get_ ## corner (rect, &__p); \
  __s = graphene_simd4f_init (__p.x, __p.y, 0.f, 1.f); \
  graphene_simd4x4f_vec4_mul (&matrix->value, &__s, &__s); \
  out_p.x = graphene_simd4f_get_x (__s); \
  out_p.y = graphene_simd4f_get_y (__s);           } while (0)

  TRANSFORM_POINT (m, r, top_left, ret[0]);
  TRANSFORM_POINT (m, r, top_right, ret[1]);
  TRANSFORM_POINT (m, r, bottom_right, ret[2]);
  TRANSFORM_POINT (m, r, bottom_left, ret[3]);

#undef TRANSFORM_POINT

#if 0
  {
    int i;

    min_x = max_x = ret[0].x;
    min_y = max_y = ret[0].y;

    for (i = 1; i < 4; i += 1)
      {
        min_x = MIN (ret[i].x, min_x);
        min_y = MIN (ret[i].y, min_y);

        max_x = MAX (ret[i].x, max_x);
        max_y = MAX (ret[i].y, max_y);
      }
  }
#else
  {
    const graphene_simd4f_t vx = graphene_simd4f_init (ret[0].x, ret[1].x, ret[2].x, ret[3].x);
    const graphene_simd4f_t vy = graphene_simd4f_init (ret[0].y, ret[1].y, ret[2].y, ret[3].y);

    min_x = graphene_simd4f_get_x (graphene_simd4f_min_val (vx));
    min_y = graphene_simd4f_get_x (graphene_simd4f_min_val (vy));

    max_x = graphene_simd4f_get_x (graphene_simd4f_max_val (vx));
    max_y = graphene_simd4f_get_x (graphene_simd4f_max_val (vy));
  }
#endif

  graphene_rect_init (res, min_x, min_y, max_x - min_x, max_y - min_y);
}

/**
 * graphene_matrix_transform_sphere:
 * @m: a #graphene_matrix_t
 * @s: a #graphene_sphere_t
 * @res: (out caller-allocates): return location for the bounds
 *   of the transformed sphere
 *
 * Transforms a #graphene_sphere_t using the given matrix @m. The
 * result is the bounding sphere containing the transformed sphere.
 *
 * Since: 1.2
 */
void
graphene_matrix_transform_sphere (const graphene_matrix_t *m,
                                  const graphene_sphere_t *s,
                                  graphene_sphere_t       *res)
{
  float max_scale;

  graphene_simd4x4f_point3_mul (&m->value, &s->center.value, &res->center.value);

  max_scale = graphene_simd4f_dot3_scalar (m->value.x, m->value.x);
  max_scale = fmaxf (max_scale, graphene_simd4f_dot3_scalar (m->value.y, m->value.y));
  max_scale = fmaxf (max_scale, graphene_simd4f_dot3_scalar (m->value.z, m->value.z));

  res->radius = s->radius * sqrtf (max_scale);
}

/**
 * graphene_matrix_transform_box:
 * @m: a #graphene_matrix_t
 * @b: a #graphene_box_t
 * @res: (out caller-allocates): return location for the bounds
 *   of the transformed box
 *
 * Transforms a #graphene_box_t using the given matrix @m. The
 * result is the bounding box containing the transformed box.
 *
 * Since: 1.2
 */
void
graphene_matrix_transform_box (const graphene_matrix_t *m,
                               const graphene_box_t    *b,
                               graphene_box_t          *res)
{
  graphene_vec3_t points[8];
  unsigned int i;

  graphene_box_get_vertices (b, points);

  for (i = 0; i < 8; i++)
    graphene_simd4x4f_point3_mul (&m->value, &(points[i].value), &(points[i].value));

  graphene_box_init_from_vectors (res, 8, points);
}

/**
 * graphene_matrix_transform_ray:
 * @m: a #graphene_matrix_t
 * @r: a #graphene_ray_t
 * @res: (out caller-allocates): return location for the
 *   transformed ray
 *
 * Transform a #graphene_ray_t using the given matrix @m.
 *
 * Since: 1.4
 */
void
graphene_matrix_transform_ray (const graphene_matrix_t *m,
                               const graphene_ray_t    *r,
                               graphene_ray_t          *res)
{
  graphene_vec3_t origin, direction;

  graphene_vec3_add (&r->direction, &r->origin, &direction);
  graphene_matrix_transform_vec3 (m, &direction, &direction);

  graphene_matrix_transform_vec3 (m, &r->origin, &origin);

  graphene_vec3_subtract (&direction, &origin, &direction);

  graphene_ray_init_from_vec3 (res, &origin, &direction);
}

/**
 * graphene_matrix_project_point:
 * @m: a #graphene_matrix_t
 * @p: a #graphene_point_t
 * @res: (out caller-allocates): return location for the projected
 *   point
 *
 * Projects a #graphene_point_t using the matrix @m.
 *
 * Since: 1.0
 */
void
graphene_matrix_project_point (const graphene_matrix_t *m,
                               const graphene_point_t  *p,
                               graphene_point_t        *res)
{
  graphene_simd4f_t pa, pb, pc;
  float a[3], b[3];
  float t;

  pa = graphene_simd4f_init (p->x, p->y, 0.f, 0.f);
  pb = graphene_simd4f_init (p->x, p->y, 1.f, 0.f);

  graphene_simd4x4f_vec3_mul (&m->value, &pa, &pa);
  graphene_simd4x4f_vec3_mul (&m->value, &pb, &pb);
  pc = graphene_simd4f_sub (pa, pb);

  graphene_simd4f_dup_3f (pa, a);
  graphene_simd4f_dup_3f (pc, b);
  t = -a[2] / b[2];

  graphene_point_init (res, a[0] + t * b[0], a[1] + t * b[1]);
}

/**
 * graphene_matrix_project_rect_bounds:
 * @m: a #graphene_matrix_t
 * @r: a #graphene_rect_t
 * @res: (out caller-allocates): return location for the projected
 *   rectangle
 *
 * Projects a #graphene_rect_t using the given matrix.
 *
 * The resulting rectangle is the bounding box capable of containing
 * fully the projected rectangle.
 *
 * Since: 1.0
 */
void
graphene_matrix_project_rect_bounds (const graphene_matrix_t *m,
                                     const graphene_rect_t   *r,
                                     graphene_rect_t         *res)
{
  graphene_point_t points[4];
  graphene_point_t ret[4];
  float min_x, min_y;
  float max_x, max_y;
  int i;

  graphene_rect_get_top_left (r, &points[0]);
  graphene_rect_get_top_right (r, &points[1]);
  graphene_rect_get_bottom_left (r, &points[2]);
  graphene_rect_get_bottom_right (r, &points[3]);

  graphene_matrix_project_point (m, &points[0], &ret[0]);
  graphene_matrix_project_point (m, &points[1], &ret[1]);
  graphene_matrix_project_point (m, &points[2], &ret[2]);
  graphene_matrix_project_point (m, &points[3], &ret[3]);

  min_x = max_x = ret[0].x;
  min_y = max_y = ret[0].y;

  for (i = 1; i < 4; i++)
    {
      min_x = MIN (ret[i].x, min_x);
      min_y = MIN (ret[i].y, min_y);

      max_x = MAX (ret[i].x, max_x);
      max_y = MAX (ret[i].y, max_y);
    }

  graphene_rect_init (res, min_x, min_y, max_x - min_x, max_y - min_y);
}

/**
 * graphene_matrix_project_rect:
 * @m: a #graphene_matrix_t
 * @r: a #graphene_rect_t
 * @res: (out caller-allocates): return location for the projected
 *   rectangle
 *
 * Projects a #graphene_rect_t using the given matrix.
 *
 * Since: 1.2
 */
void
graphene_matrix_project_rect (const graphene_matrix_t *m,
                              const graphene_rect_t   *r,
                              graphene_quad_t         *res)
{
  graphene_point_t p[4];

  graphene_rect_get_top_left (r, &p[0]);
  graphene_matrix_project_point (m, &p[0], &p[0]);

  graphene_rect_get_top_right (r, &p[1]);
  graphene_matrix_project_point (m, &p[1], &p[1]);

  graphene_rect_get_bottom_left (r, &p[2]);
  graphene_matrix_project_point (m, &p[2], &p[2]);

  graphene_rect_get_bottom_right (r, &p[3]);
  graphene_matrix_project_point (m, &p[3], &p[3]);

  graphene_quad_init_from_points (res, p);
}

/**
 * graphene_matrix_untransform_point:
 * @m: a #graphene_matrix_t
 * @p: a #graphene_point_t
 * @bounds: the bounds of the transformation
 * @res: (out caller-allocates): return location for the
 *   untransformed point
 *
 * Undoes the transformation of a #graphene_point_t using the
 * given matrix, within the given rectangular @bounds.
 *
 * Returns: `true` if the point was successfully untransformed
 *
 * Since: 1.0
 */
bool
graphene_matrix_untransform_point (const graphene_matrix_t *m,
                                   const graphene_point_t  *p,
                                   const graphene_rect_t   *bounds,
                                   graphene_point_t        *res)
{
  graphene_matrix_t inverse;
  graphene_rect_t bounds_t;

  if (graphene_matrix_is_2d (m))
    {
      if (!graphene_matrix_inverse (m, &inverse))
        return false;

      graphene_matrix_transform_point (&inverse, p, res);
      return true;
    }

  graphene_matrix_transform_bounds (m, bounds, &bounds_t);
  if (!graphene_rect_contains_point (&bounds_t, p))
    return false;

  if (!graphene_matrix_inverse (m, &inverse))
    return false;

  graphene_matrix_project_point (&inverse, p, res);

  return true;
}

/**
 * graphene_matrix_untransform_bounds:
 * @m: a #graphene_matrix_t
 * @r: a #graphene_rect_t
 * @bounds: the bounds of the transformation
 * @res: (out caller-allocates): return location for the
 *   untransformed rectangle
 *
 * Undoes the transformation on the points of a #graphene_rect_t
 * using the given matrix, within the given rectangular @bounds.
 *
 * Since: 1.0
 */
void
graphene_matrix_untransform_bounds (const graphene_matrix_t *m,
                                    const graphene_rect_t   *r,
                                    const graphene_rect_t   *bounds,
                                    graphene_rect_t         *res)
{
  graphene_matrix_t inverse;
  graphene_rect_t bounds_t;
  graphene_rect_t rect;

  if (graphene_matrix_is_2d (m))
    {
      if (!graphene_matrix_inverse (m, &inverse))
        return;

      graphene_matrix_transform_bounds (&inverse, r, res);
      return;
    }

  graphene_matrix_transform_bounds (m, bounds, &bounds_t);
  if (!graphene_rect_intersection (r, &bounds_t, &rect))
    {
      graphene_rect_init (res, 0.f, 0.f, 0.f, 0.f);
      return;
    }

  if (!graphene_matrix_inverse (m, &inverse))
    return;

  graphene_matrix_project_rect_bounds (&inverse, &rect, res);
}

/**
 * graphene_matrix_unproject_point3d:
 * @projection: a #graphene_matrix_t for the projection matrix
 * @modelview: a #graphene_matrix_t for the modelview matrix; this is
 *   the inverse of the modelview used when projecting the point
 * @point: a #graphene_point3d_t with the coordinates of the point
 * @res: (out caller-allocates): return location for the unprojected
 *   point
 *
 * Unprojects the given @point using the @projection matrix and
 * a @modelview matrix.
 *
 * Since: 1.2
 */
void
graphene_matrix_unproject_point3d (const graphene_matrix_t  *projection,
                                   const graphene_matrix_t  *modelview,
                                   const graphene_point3d_t *point,
                                   graphene_point3d_t       *res)
{
  graphene_simd4x4f_t tmp;
  graphene_simd4f_t v;
  float values[4];
  float inv_w;

  if (!graphene_simd4x4f_inverse (&projection->value, &tmp))
    return;

  graphene_simd4x4f_matrix_mul (&tmp, &modelview->value, &tmp);

  v = graphene_simd4f_init (point->x, point->y, point->z, 1.f);
  graphene_simd4x4f_vec4_mul (&tmp, &v, &v);

  inv_w = 1.f / graphene_simd4f_get_w (v);
  v = graphene_simd4f_mul (v, graphene_simd4f_splat (inv_w));

  graphene_simd4f_dup_4f (v, values);
  graphene_point3d_init (res, values[0], values[1], values[2]);
}

/**
 * graphene_matrix_translate:
 * @m: a #graphene_matrix_t
 * @pos: a #graphene_point3d_t
 *
 * Adds a translation transformation to @m using the coordinates
 * of the given #graphene_point3d_t.
 *
 * Since: 1.0
 */
void
graphene_matrix_translate (graphene_matrix_t        *m,
                           const graphene_point3d_t *pos)
{
  graphene_simd4x4f_t trans_m;

  graphene_simd4x4f_translation (&trans_m, pos->x, pos->y, pos->z);
  graphene_simd4x4f_matrix_mul (&m->value, &trans_m, &m->value);
}

/**
 * graphene_matrix_rotate_quaternion:
 * @m: a #graphene_matrix_t
 * @q: a rotation described by a #graphene_quaternion_t
 *
 * Adds a rotation transformation to @m, using the given
 * #graphene_quaternion_t.
 *
 * Since: 1.2
 */
void
graphene_matrix_rotate_quaternion (graphene_matrix_t           *m,
                                   const graphene_quaternion_t *q)
{
  graphene_matrix_t rot;

  graphene_quaternion_to_matrix (q, &rot);
  graphene_matrix_multiply (m, &rot, m);
}

/**
 * graphene_matrix_rotate_euler:
 * @m: a #graphene_matrix_t
 * @e: a rotation described by a #graphene_euler_t
 *
 * Adds a rotation transformation to @m, using the given
 * #graphene_euler_t.
 *
 * Since: 1.2
 */
void
graphene_matrix_rotate_euler (graphene_matrix_t      *m,
                              const graphene_euler_t *e)
{
  graphene_quaternion_t q;

  graphene_quaternion_init_from_euler (&q, e);
  graphene_matrix_rotate_quaternion (m, &q);
}

static inline void
graphene_matrix_rotate_internal (graphene_simd4x4f_t     *m,
                                 float                    rad,
                                 const graphene_simd4f_t  axis)
{
  graphene_simd4x4f_t rot_m;

  graphene_simd4x4f_rotation (&rot_m, rad, axis);
  graphene_simd4x4f_matrix_mul (m, &rot_m, m);
}

/**
 * graphene_matrix_rotate:
 * @m: a #graphene_matrix_t
 * @angle: the rotation angle, in degrees
 * @axis: the rotation axis, as a #graphene_vec3_t
 *
 * Adds a rotation transformation to @m, using the given @angle
 * and @axis vector.
 *
 * Since: 1.0
 */
void
graphene_matrix_rotate (graphene_matrix_t     *m,
                        float                  angle,
                        const graphene_vec3_t *axis)
{
  graphene_matrix_rotate_internal (&m->value, GRAPHENE_DEG_TO_RAD (angle), axis->value);
}

/**
 * graphene_matrix_rotate_x:
 * @m: a #graphene_matrix_t
 * @angle: the rotation angle, in degrees
 *
 * Adds a rotation transformation around the X axis to @m, using
 * the given @angle.
 *
 * Since: 1.0
 */
void
graphene_matrix_rotate_x (graphene_matrix_t *m,
                          float              angle)
{
  graphene_matrix_rotate_internal (&m->value, GRAPHENE_DEG_TO_RAD (angle),
                                   graphene_simd4f_init (1.f, 0.f, 0.f, 0.f));
}

/**
 * graphene_matrix_rotate_y:
 * @m: a #graphene_matrix_t
 * @angle: the rotation angle, in degrees
 *
 * Adds a rotation transformation around the Y axis to @m, using
 * the given @angle.
 *
 * Since: 1.0
 */
void
graphene_matrix_rotate_y (graphene_matrix_t *m,
                          float              angle)
{
  graphene_matrix_rotate_internal (&m->value, GRAPHENE_DEG_TO_RAD (angle),
                                   graphene_simd4f_init (0.f, 1.f, 0.f, 0.f));
}

/**
 * graphene_matrix_rotate_z:
 * @m: a #graphene_matrix_t
 * @angle: the rotation angle, in degrees
 *
 * Adds a rotation transformation around the Z axis to @m, using
 * the given @angle.
 *
 * Since: 1.0
 */
void
graphene_matrix_rotate_z (graphene_matrix_t *m,
                          float              angle)
{
  graphene_matrix_rotate_internal (&m->value, GRAPHENE_DEG_TO_RAD (angle),
                                   graphene_simd4f_init (0.f, 0.f, 1.f, 0.f));
}

/**
 * graphene_matrix_scale:
 * @m: a #graphene_matrix_t
 * @factor_x: scaling factor on the X axis
 * @factor_y: scaling factor on the Y axis
 * @factor_z: scaling factor on the Z axis
 *
 * Adds a scaling transformation to @m, using the three
 * given factors.
 *
 * Since: 1.0
 */
void
graphene_matrix_scale (graphene_matrix_t *m,
                       float              factor_x,
                       float              factor_y,
                       float              factor_z)
{
  graphene_simd4x4f_t scale_m;

  graphene_simd4x4f_scale (&scale_m, factor_x, factor_y, factor_z);
  graphene_simd4x4f_matrix_mul (&m->value, &scale_m, &m->value);
}

/**
 * graphene_matrix_skew_xy:
 * @m: a #graphene_matrix_t
 * @factor: skew factor
 *
 * Adds a skew of @factor on the X and Y axis to the given matrix.
 *
 * Since: 1.0
 */
void
graphene_matrix_skew_xy (graphene_matrix_t *m,
                         float              factor)
{
  graphene_simd4f_t m_x, m_y;

  m_x = m->value.x;
  m_y = m->value.y;

  m->value.y = graphene_simd4f_madd (m_x, graphene_simd4f_splat (factor), m_y);
}

/**
 * graphene_matrix_skew_xz:
 * @m: a #graphene_matrix_t
 * @factor: skew factor
 *
 * Adds a skew of @factor on the X and Z axis to the given matrix.
 *
 * Since: 1.0
 */
void
graphene_matrix_skew_xz (graphene_matrix_t *m,
                         float              factor)
{
  graphene_simd4f_t m_x, m_z;

  m_x = m->value.x;
  m_z = m->value.z;

  m->value.z = graphene_simd4f_madd (m_x, graphene_simd4f_splat (factor), m_z);
}

/**
 * graphene_matrix_skew_yz:
 * @m: a #graphene_matrix_t
 * @factor: skew factor
 *
 * Adds a skew of @factor on the Y and Z axis to the given matrix.
 *
 * Since: 1.0
 */
void
graphene_matrix_skew_yz (graphene_matrix_t *m,
                         float              factor)
{
  graphene_simd4f_t m_y, m_z;

  m_y = m->value.y;
  m_z = m->value.z;

  m->value.z = graphene_simd4f_madd (m_y, graphene_simd4f_splat (factor), m_z);
}

static inline void
graphene_matrix_transpose_transform_vec4 (const graphene_matrix_t *m,
                                          const graphene_vec4_t   *v,
                                          graphene_vec4_t         *res)
{
  float x, y, z, w;

  x = graphene_simd4f_get_x (graphene_simd4f_dot4 (m->value.x, v->value));
  y = graphene_simd4f_get_x (graphene_simd4f_dot4 (m->value.y, v->value));
  z = graphene_simd4f_get_x (graphene_simd4f_dot4 (m->value.z, v->value));
  w = graphene_simd4f_get_x (graphene_simd4f_dot4 (m->value.w, v->value));

  graphene_vec4_init (res, x, y, z, w);
}

/**
 * graphene_matrix_transpose:
 * @m: a #graphene_matrix_t
 * @res: (out caller-allocates): return location for the
 *   transposed matrix
 *
 * Transposes the given matrix.
 *
 * Since: 1.0
 */
void
graphene_matrix_transpose (const graphene_matrix_t *m,
                           graphene_matrix_t       *res)
{
  graphene_simd4x4f_transpose (&m->value, &res->value);
}

/**
 * graphene_matrix_inverse:
 * @m: a #graphene_matrix_t
 * @res: (out caller-allocates): return location for the
 *   inverse matrix
 *
 * Inverts the given matrix.
 *
 * Returns: `true` if the matrix is invertible
 *
 * Since: 1.0
 */
bool
graphene_matrix_inverse (const graphene_matrix_t *m,
                         graphene_matrix_t       *res)
{
  return graphene_simd4x4f_inverse (&m->value, &res->value);
}

/**
 * graphene_matrix_perspective:
 * @m: a #graphene_matrix_t
 * @depth: the depth of the perspective
 * @res: (out caller-allocates): return location for the
 *   perspective matrix
 *
 * Applies a perspective of @depth to the matrix.
 *
 * Since: 1.0
 */
void
graphene_matrix_perspective (const graphene_matrix_t *m,
                             float                    depth,
                             graphene_matrix_t       *res)
{

  res->value = m->value;

  graphene_simd4x4f_perspective (&res->value, depth);
}

/**
 * graphene_matrix_normalize:
 * @m: a #graphene_matrix_t
 * @res: (out caller-allocates): return location for the normalized matrix
 *
 * Normalizes the given #graphene_matrix_t.
 *
 * Since: 1.0
 */
void
graphene_matrix_normalize (const graphene_matrix_t *m,
                           graphene_matrix_t       *res)
{
  graphene_simd4f_t n;
  float ww;

  ww = graphene_simd4f_get_w (m->value.w);
  if (ww == 0.f)
    return;

  n = graphene_simd4f_splat (1.f / ww);

  res->value.x = graphene_simd4f_mul (m->value.x, n);
  res->value.y = graphene_simd4f_mul (m->value.y, n);
  res->value.z = graphene_simd4f_mul (m->value.z, n);
  res->value.w = graphene_simd4f_mul (m->value.w, n);
}

/**
 * graphene_matrix_get_x_scale:
 * @m: a #graphene_matrix_t
 *
 * Retrieves the scaling factor on the X axis in @m.
 *
 * Returns: the value of the scaling factor
 *
 * Since: 1.0
 */
float
graphene_matrix_get_x_scale (const graphene_matrix_t *m)
{
  return graphene_simd4f_get_x (m->value.x);
}

/**
 * graphene_matrix_get_y_scale:
 * @m: a #graphene_matrix_t
 *
 * Retrieves the scaling factor on the Y axis in @m.
 *
 * Returns: the value of the scaling factor
 *
 * Since: 1.0
 */
float
graphene_matrix_get_y_scale (const graphene_matrix_t *m)
{
  return graphene_simd4f_get_y (m->value.y);
}

/**
 * graphene_matrix_get_z_scale:
 * @m: a #graphene_matrix_t
 *
 * Retrieves the scaling factor on the Z axis in @m.
 *
 * Returns: the value of the scaling factor
 *
 * Since: 1.0
 */
float
graphene_matrix_get_z_scale (const graphene_matrix_t *m)
{
  return graphene_simd4f_get_z (m->value.z);
}

/* the algorithm for decomposing two matrices and interpolating
 * between their components in order to reassemble a matrix that
 * is equivalent to a linear interpolation of the original two
 * matrices is taken from the CSS3 Transforms specification:
 *
 * http://dev.w3.org/csswg/css-transforms/
 *
 * specifically, the 2D matrix decomposition is taken from:
 *
 * http://dev.w3.org/csswg/css-transforms/#decomposing-a-2d-matrix
 *
 * while the 3D matrix decomposition is taken from:
 *
 * http://dev.w3.org/csswg/css-transforms/#decomposing-a-3d-matrix
 *
 * both, in turn, refer to the `unmatrix` program published in
 * the book "Graphics Gems II" edited by Jim Arvo; the code is
 * available at:
 *
 * http://tog.acm.org/resources/GraphicsGems/gemsii/unmatrix.c
 */

#define XY_SHEAR        0
#define XZ_SHEAR        1
#define YZ_SHEAR        2

#define M_11    0
#define M_12    1
#define M_21    2
#define M_22    3

static bool
matrix_decompose_2d (const graphene_matrix_t *m,
                     graphene_point_t        *translate_r,
                     graphene_point_t        *scale_r,
                     double                  *angle_r,
                     float                    m_r[4])
{
  float row0x = graphene_matrix_get_value (m, 0, 0);
  float row0y = graphene_matrix_get_value (m, 1, 0);
  float row1x = graphene_matrix_get_value (m, 0, 1);
  float row1y = graphene_matrix_get_value (m, 1, 1);
  float angle;
  float det;

  if (fabsf (row0x * row1y - row0y * row1x) < FLT_EPSILON)
    return false;

  graphene_point_init (translate_r,
                       graphene_matrix_get_value (m, 3, 0),
                       graphene_matrix_get_value (m, 3, 1));

  scale_r->x = sqrtf (row0x * row0x + row0y * row0y);
  scale_r->y = sqrtf (row1x * row1x + row1y * row1y);

  det = row0x * row1y - row0y * row1x;
  if (det < 0)
    {
      if (row0x < row1y)
        scale_r->x = -scale_r->x;
      else
        scale_r->y = -scale_r->y;
    }

  if (scale_r->x != 0.f)
    {
      row0x = row0x * (1.f / scale_r->x);
      row0y = row0y * (1.f / scale_r->y);
    }

  if (scale_r->y != 0.f)
    {
      row1x = row1x * (1.f / scale_r->x);
      row1y = row1y * (1.f / scale_r->y);
    }

  angle = atan2 (row0y, row0x);

  if (angle)
    {
      double sn = -row0y, cs = row0x;
      double m11 = row0x, m12 = row0y;
      double m21 = row1x, m22 = row1y;

      row0x = cs * m11 + sn * m21;
      row0y = cs * m12 + sn * m22;
      row1x = -sn * m11 + cs * m21;
      row1y = -sn * m12 + cs * m22;
    }

  m_r[M_11] = row0x;
  m_r[M_12] = row0y;
  m_r[M_21] = row1x;
  m_r[M_22] = row1y;

  *angle_r = GRAPHENE_RAD_TO_DEG (angle);

  return true;
}

static bool
matrix_decompose_3d (const graphene_matrix_t *m,
                     graphene_point3d_t      *scale_r,
                     float                    shear_r[3],
                     graphene_quaternion_t   *rotate_r,
                     graphene_point3d_t      *translate_r,
                     graphene_vec4_t         *perspective_r)
{
  graphene_matrix_t local, perspective;
  float shear_xy, shear_xz, shear_yz;
  float scale_x, scale_y, scale_z;
  graphene_simd4f_t perspective_v;
  graphene_simd4f_t cross;

  if (graphene_matrix_get_value (m, 3, 3) == 0.f)
    return false;

  local = *m;

  /* normalize the matrix */
  graphene_matrix_normalize (&local, &local);

  /* perspective is used to solve for the perspective component,
   * but it also provides an easy way to test for singularity of
   * the upper 3x3 component
   */
  perspective = local;
  perspective.value.w = graphene_simd4f_init (0.f, 0.f, 0.f, 1.f);

  if (graphene_matrix_determinant (&perspective) == 0.f)
    return false;

  perspective_v = graphene_simd4f_init (graphene_simd4f_get_w (local.value.x),
                                        graphene_simd4f_get_w (local.value.y),
                                        graphene_simd4f_get_w (local.value.z),
                                        graphene_simd4f_get_w (local.value.w));

  /* isolate the perspective */
  if (!graphene_simd4f_is_zero3 (perspective_v))
    {
      graphene_matrix_t tmp;

      /* perspective_r is the right hand side of the equation */
      perspective_r->value = perspective_v;

      /* solve the equation by inverting perspective and multiplying
       * the inverse with the perspective vector; we don't need to
       * check if the matrix is invertible here because we just checked
       * whether the determinant is not zero.
       */
      graphene_matrix_inverse (&perspective, &tmp);
      graphene_matrix_transpose_transform_vec4 (&tmp, perspective_r, perspective_r);

      /* Clear the perspective component */
      local.value.x = graphene_simd4f_merge_w (local.value.x, 0.f);
      local.value.y = graphene_simd4f_merge_w (local.value.y, 0.f);
      local.value.z = graphene_simd4f_merge_w (local.value.z, 0.f);
      local.value.w = graphene_simd4f_merge_w (local.value.w, 1.f);
    }
  else
    graphene_vec4_init (perspective_r, 0.f, 0.f, 0.f, 1.f);

  /* next, take care of the translation partition */
  translate_r->x = graphene_simd4f_get_x (local.value.w);
  translate_r->y = graphene_simd4f_get_y (local.value.w);
  translate_r->z = graphene_simd4f_get_z (local.value.w);
  local.value.w = graphene_simd4f_init (0.f, 0.f, 0.f, graphene_simd4f_get_w (local.value.w));

  /* now get scale and shear */

  /* compute the X scale factor and normalize the first row */
  scale_x = graphene_simd4f_get_x (graphene_simd4f_length4 (local.value.x));
  local.value.x = graphene_simd4f_normalize4 (local.value.x);

  /* compute XY shear factor and the second row orthogonal to the first */
  shear_xy = graphene_simd4f_get_x (graphene_simd4f_dot4 (local.value.x, local.value.y));
  local.value.y = graphene_simd4f_sub (local.value.y, graphene_simd4f_mul (local.value.x, graphene_simd4f_splat (shear_xy)));

  /* now, compute the Y scale factor and normalize the second row */
  scale_y = graphene_simd4f_get_x (graphene_simd4f_length4 (local.value.y));
  local.value.y = graphene_simd4f_normalize4 (local.value.y);
  shear_xy /= scale_y;

  /* compute XZ and YZ shears, make the third row orthogonal */
  shear_xz = graphene_simd4f_get_x (graphene_simd4f_dot4 (local.value.x, local.value.z));
  local.value.z = graphene_simd4f_sub (local.value.z, graphene_simd4f_mul (local.value.x, graphene_simd4f_splat (shear_xz)));
  shear_yz = graphene_simd4f_get_x (graphene_simd4f_dot4 (local.value.y, local.value.z));
  local.value.z = graphene_simd4f_sub (local.value.z, graphene_simd4f_mul (local.value.y, graphene_simd4f_splat (shear_yz)));

  /* next, get the Z scale and normalize the third row */
  scale_z = graphene_simd4f_get_x (graphene_simd4f_length4 (local.value.z));
  local.value.z = graphene_simd4f_normalize4 (local.value.z);

  shear_xz /= scale_z;
  shear_yz /= scale_z;

  shear_r[XY_SHEAR] = shear_xy;
  shear_r[XZ_SHEAR] = shear_xz;
  shear_r[YZ_SHEAR] = shear_yz;

  /* at this point, the matrix is orthonormal. we check for a
   * coordinate system flip. if the determinant is -1, then
   * negate the matrix and the scaling factors
   */
  cross = graphene_simd4f_dot3 (local.value.x, graphene_simd4f_cross3 (local.value.y, local.value.z));
  if (graphene_simd4f_get_x (cross) < 0.f)
    {
      scale_x *= -1.f;
      scale_y *= -1.f;
      scale_z *= -1.f;

      local.value.x = graphene_simd4f_neg (local.value.x);
      local.value.y = graphene_simd4f_neg (local.value.y);
      local.value.z = graphene_simd4f_neg (local.value.z);
    }

  graphene_point3d_init (scale_r, scale_x, scale_y, scale_z);

  /* get the rotations out */
  graphene_quaternion_init_from_matrix (rotate_r, &local);

  return true;
}

/**
 * graphene_matrix_interpolate:
 * @a: a #graphene_matrix_t
 * @b: a #graphene_matrix_t
 * @factor: the linear interpolation factor
 * @res: (out caller-allocates): return location for the
 *   interpolated matrix
 *
 * Linearly interpolates the two given #graphene_matrix_t by
 * interpolating the decomposed transformations separately.
 *
 * If either matrix cannot be reduced to their transformations
 * then the interpolation cannot be performed, and this function
 * will return an identity matrix.
 *
 * Since: 1.0
 */
void
graphene_matrix_interpolate (const graphene_matrix_t *a,
                             const graphene_matrix_t *b,
                             double                   factor,
                             graphene_matrix_t       *res)
{
  bool success = false;

  graphene_matrix_init_identity (res);

  /* Special case the decomposition if we're interpolating between two
   * affine transformations.
   */
  if (graphene_matrix_is_2d (a) &&
      graphene_matrix_is_2d (b))
    {
      graphene_point_t translate_a = GRAPHENE_POINT_INIT (0.f, 0.f);
      graphene_point_t translate_b = GRAPHENE_POINT_INIT (0.f, 0.f);
      graphene_point_t translate_res;
      graphene_point_t scale_a = GRAPHENE_POINT_INIT (1.f, 1.f);
      graphene_point_t scale_b = GRAPHENE_POINT_INIT (1.f, 1.f);
      graphene_point_t scale_res;
      double rotate_a, rotate_b, rotate_res;
      float m_a[4], m_b[4], m_res[4];
      float rot_sin, rot_cos;
      graphene_simd4x4f_t tmp_m;

      success |= matrix_decompose_2d (a, &translate_a, &scale_a, &rotate_a, m_a);
      success |= matrix_decompose_2d (b, &translate_b, &scale_b, &rotate_b, m_b);

      /* If we cannot decompose either matrix we bail out with an identity */
      if (!success)
        return;

      /* Flip the scaling factor and angle so they are consistent */
      if ((scale_a.x < 0 && scale_b.y < 0) || (scale_a.y < 0 && scale_b.x < 0 ))
        {
          scale_a.x = -scale_a.x;
          scale_a.y = -scale_a.y;

          rotate_a += (rotate_a < 0) ? 180 : -180;
        }

      /* Do not rotate "the long way around" */
      if (!rotate_a)
        rotate_a = 360;
      if (!rotate_b)
        rotate_b = 360;

      if (fabs (rotate_a - rotate_b) > 180)
        {
          if (rotate_a > rotate_b)
            rotate_a -= 360;
          else
            rotate_b -= 360;
        }

      graphene_point_interpolate (&translate_a, &translate_b, factor, &translate_res);
      graphene_point_interpolate (&scale_a, &scale_b, factor, &scale_res);
      rotate_res = graphene_lerp (rotate_a, rotate_b, factor);

      /* Interpolate each component of the (2,2) matrices */
      {
        graphene_simd4f_t tmp_va = graphene_simd4f_init_4f (m_a);
        graphene_simd4f_t tmp_vb = graphene_simd4f_init_4f (m_b);
        graphene_simd4f_t tmp_vres = graphene_simd4f_interpolate (tmp_va, tmp_vb, factor);

        graphene_simd4f_dup_4f (tmp_vres, m_res);
      }

      /* Initialize using the transposed (2,2) matrix */
      res->value.x = graphene_simd4f_init (m_res[M_11], m_res[M_21], 0.f, 0.f);
      res->value.y = graphene_simd4f_init (m_res[M_12], m_res[M_22], 0.f, 0.f);
      res->value.z = graphene_simd4f_init (        0.f,         0.f, 1.f, 0.f);

      /* Translate */
      res->value.w = graphene_simd4f_init (translate_res.x * m_res[M_11] + translate_res.y * m_res[M_21],
                                           translate_res.x * m_res[M_12] + translate_res.y * m_res[M_22],
                                           0.f,
                                           1.f);

      /* Rotate using a (2,2) rotation matrix */
      graphene_sincos (GRAPHENE_DEG_TO_RAD (rotate_res), &rot_sin, &rot_cos);
      tmp_m = graphene_simd4x4f_init (graphene_simd4f_init (rot_cos, -rot_sin, 0.f, 0.f),
                                      graphene_simd4f_init (rot_sin,  rot_cos, 0.f, 0.f),
                                      graphene_simd4f_init (    0.f,      0.f, 1.f, 0.f),
                                      graphene_simd4f_init (    0.f,      0.f, 0.f, 1.f));
      graphene_simd4x4f_matrix_mul (&res->value, &tmp_m, &res->value);

      /* Scale */
      graphene_simd4x4f_scale (&tmp_m, scale_res.x, scale_res.y, 1.f);
      graphene_simd4x4f_matrix_mul (&res->value, &tmp_m, &res->value);
    }
  else
    {
      graphene_point3d_t scale_a = { 1.f, 1.f, 1.f }, translate_a;
      graphene_quaternion_t rotate_a;
      float shear_a[3] = { 0.f, 0.f, 0.f };

      graphene_point3d_t scale_b = { 1.f, 1.f, 1.f }, translate_b;
      graphene_quaternion_t rotate_b;
      float shear_b[3] = { 0.f, 0.f, 0.f };

      graphene_point3d_t scale_r = { 1.f, 1.f, 1.f }, translate_r;
      graphene_quaternion_t rotate_r;
      float shear;

      graphene_vec4_t perspective_a;
      graphene_vec4_t perspective_b;
      graphene_simd4f_t tmp;

      success |= matrix_decompose_3d (a, &scale_a, shear_a, &rotate_a, &translate_a, &perspective_a);
      success |= matrix_decompose_3d (b, &scale_b, shear_b, &rotate_b, &translate_b, &perspective_b);

      /* If we cannot decompose either matrix we bail out with an identity */
      if (!success)
        return;

      /* Interpolate the perspective row */
      tmp = graphene_simd4f_interpolate (perspective_a.value, perspective_b.value, factor);
      res->value.x = graphene_simd4f_init (1.f, 0.f, 0.f, graphene_simd4f_get_x (tmp));
      res->value.y = graphene_simd4f_init (0.f, 1.f, 0.f, graphene_simd4f_get_y (tmp));
      res->value.z = graphene_simd4f_init (0.f, 0.f, 1.f, graphene_simd4f_get_z (tmp));
      res->value.w = graphene_simd4f_init (0.f, 0.f, 0.f, graphene_simd4f_get_w (tmp));

      /* Translate */
      graphene_point3d_interpolate (&translate_a, &translate_b, factor, &translate_r);
      graphene_matrix_translate (res, &translate_r);

      /* Rotate */
      graphene_quaternion_slerp (&rotate_a, &rotate_b, factor, &rotate_r);
      graphene_matrix_rotate_quaternion (res, &rotate_r);

      /* Skew */
      shear = graphene_lerp (shear_a[YZ_SHEAR], shear_b[YZ_SHEAR], factor);
      if (shear != 0.f)
        graphene_matrix_skew_yz (res, shear);

      shear = graphene_lerp (shear_a[XZ_SHEAR], shear_b[XZ_SHEAR], factor);
      if (shear != 0.f)
        graphene_matrix_skew_xz (res, shear);

      shear = graphene_lerp (shear_a[XY_SHEAR], shear_b[XY_SHEAR], factor);
      if (shear != 0.f)
        graphene_matrix_skew_xy (res, shear);

      /* Scale */
      graphene_point3d_interpolate (&scale_a, &scale_b, factor, &scale_r);
      if (scale_r.x != 1.f && scale_r.y != 1.f && scale_r.z != 1.f)
        graphene_matrix_scale (res, scale_r.x, scale_r.y, scale_r.z);
    }
}

#undef M_11
#undef M_12
#undef M_21
#undef M_22
#undef XY_SHEAR
#undef XZ_SHEAR
#undef YZ_SHEAR

/**
 * graphene_matrix_print:
 * @m: The matrix to print
 *
 * Prints the contents of a matrix.
 *
 * Since: 1.0
 */
void
graphene_matrix_print (const graphene_matrix_t *m)
{
  int i;

  for (i = 0; i < 4; i++)
    {
      fprintf (stderr,
               "| %+.6f %+.6f %+.6f %+.6f |\n",
               graphene_matrix_get_value (m, i, 0),
               graphene_matrix_get_value (m, i, 1),
               graphene_matrix_get_value (m, i, 2),
               graphene_matrix_get_value (m, i, 3));
    }
}
