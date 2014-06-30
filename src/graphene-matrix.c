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
 * |[
 *   | x |    | x.x   x.y   x.z   x.w |
 *   | y | -\ | y.x   y.y   y.z   y.w |
 *   | z | -/ | z.x   z.y   z.z   z.w |
 *   | w |    | w.x   w.y   w.z   w.w |
 * ]|
 *
 * The layout of a #graphene_matrix_t matrix in memory will be
 * identical to GL and Direct3D matrices, so it's cheap to convert between
 * #graphene_matrix_t and arrays of floating point values to pass to other
 * API.
 *
 * The contents of a #graphene_matrix_t are private, and direct access is not
 * possible. You can modify and read the contents of a #graphene_matrix_t
 * only through the provided API.
 */

#include "graphene-private.h"

#include "graphene-matrix.h"

#include "graphene-alloc-private.h"
#include "graphene-point.h"
#include "graphene-point3d.h"
#include "graphene-rect.h"
#include "graphene-simd4x4f.h"
#include "graphene-quad.h"
#include "graphene-quaternion.h"
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
  return graphene_alloc (sizeof (graphene_matrix_t), 1, 16);
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
  graphene_free (m);
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
  graphene_simd4x4f_init_identity (&m->value);

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
  float fovy_rad;

  fovy_rad = fovy * GRAPHENE_PI / 180.f;

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
  float rad;

  rad = angle * GRAPHENE_PI / 180.f;

  graphene_simd4x4f_rotation (&m->value, rad, axis->value);

  return m;
}

/**
 * graphene_matrix_is_identity:
 * @m: a #graphene_matrix_t
 *
 * Checks whether the given #graphene_matrix_t is the identity matrix.
 *
 * Returns: %true if the matrix is the identity matrix
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
 * Returns: %true if the matrix is compatible with an affine
 *   transformation matrix
 *
 * Since: 1.0
 */
bool
graphene_matrix_is_2d (const graphene_matrix_t *m)
{
  return graphene_simd4x4f_is_2d (&m->value);
}

/**
 * graphene_matrix_is_backface_visible:
 * @m: a #graphene_matrix_t
 *
 * Checks whether a #graphene_matrix_t has a visible back face.
 *
 * Returns: %true if the back face of the matrix is visible
 *
 * Since: 1.0
 */
bool
graphene_matrix_is_backface_visible (const graphene_matrix_t *m)
{
  graphene_matrix_t tmp;

  graphene_matrix_inverse (m, &tmp);

  /* inverse.zz < 0 */
  return graphene_matrix_get_value (&tmp, 2, 2) < 0.f;
}

/**
 * graphene_matrix_is_singular:
 * @m: a #graphene_matrix_t
 *
 * Checks whether a matrix is singular.
 *
 * Returns: %true if the matrix is singular
 *
 * Since: 1.0
 */
bool
graphene_matrix_is_singular (const graphene_matrix_t *m)
{
  return graphene_matrix_determinant (m) == 0.0f;
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
 * |[
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
                                     graphene_simd4f_init ( yx,  yy, 0.f, 0.f),
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
 * |[
 *   | xx yx |   |  a  b  0 |
 *   | xy yy | = |  c  d  0 |
 *   | x0 y0 |   | tx ty  1 |
 * ]|
 *
 * This function can be used to convert between a #graphene_matrix_t
 * and a matrix type from other libraries.
 *
 * Returns: %true if the matrix is compatible with an affine
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
  if (!graphene_simd4x4f_is_2d (&m->value))
    return false;

  if (xx != NULL)
    *xx = graphene_matrix_get_value (m, 0, 0);
  if (yx != NULL)
    *yx = graphene_matrix_get_value (m, 0, 1);
  if (xy != NULL)
    *xy = graphene_matrix_get_value (m, 1, 0);
  if (yy != NULL)
    *yy = graphene_matrix_get_value (m, 1, 1);
  if (x_0 != NULL)
    *x_0 = graphene_matrix_get_value (m, 3, 0);
  if (y_0 != NULL)
    *y_0 = graphene_matrix_get_value (m, 3, 1);

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
 * @m: a #grapheme_matrix_t
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
  float c = 0.f;

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
      c = graphene_simd4f_get_x (r);
      break;

    case 1:
      c = graphene_simd4f_get_y (r);
      break;

    case 2:
      c = graphene_simd4f_get_z (r);
      break;

    case 3:
      c = graphene_simd4f_get_w (r);
      break;

    default:
      return 0;
    }

  return c;
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
  graphene_point_t points[4];
  graphene_point_t ret[4];

  graphene_rect_get_top_left (r, &points[0]);
  graphene_rect_get_top_right (r, &points[1]);
  graphene_rect_get_bottom_right (r, &points[2]);
  graphene_rect_get_bottom_left (r, &points[3]);

  graphene_matrix_transform_point (m, &points[0], &ret[0]);
  graphene_matrix_transform_point (m, &points[1], &ret[1]);
  graphene_matrix_transform_point (m, &points[2], &ret[2]);
  graphene_matrix_transform_point (m, &points[3], &ret[3]);

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
  graphene_point_t points[4];
  graphene_point_t ret[4];
  float min_x, min_y;
  float max_x, max_y;
  int i;

  graphene_rect_get_top_left (r, &points[0]);
  graphene_rect_get_top_right (r, &points[1]);
  graphene_rect_get_bottom_left (r, &points[2]);
  graphene_rect_get_bottom_right (r, &points[3]);

  graphene_matrix_transform_point (m, &points[0], &ret[0]);
  graphene_matrix_transform_point (m, &points[1], &ret[1]);
  graphene_matrix_transform_point (m, &points[2], &ret[2]);
  graphene_matrix_transform_point (m, &points[3], &ret[3]);

  min_x = max_x = ret[0].x;
  min_y = max_y = ret[0].y;

  for (i = 1; i < 4; i += 1)
    {
      min_x = MIN (ret[i].x, min_x);
      min_y = MIN (ret[i].y, min_y);

      max_x = MAX (ret[i].x, max_x);
      max_y = MAX (ret[i].y, max_y);
    }

  graphene_rect_init (res, min_x, min_y, max_x - min_x, max_y - min_y);
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
  graphene_vec3_t pa, qa;
  graphene_vec3_t pback, qback, uback;
  float p_z, u_z, t;

  graphene_vec3_init (&pa, p->x, p->y, 0.0f);
  graphene_vec3_init (&qa, p->x, p->y, 1.0f);

  graphene_matrix_transform_vec3 (m, &pa, &pback);
  graphene_matrix_transform_vec3 (m, &qa, &qback);

  graphene_vec3_subtract (&qback, &pback, &uback);

  p_z = graphene_vec3_get_z (&pback);
  u_z = graphene_vec3_get_z (&uback);
  t = -p_z / u_z;

  graphene_point_init (res,
                       graphene_vec3_get_x (&pback) + t * graphene_vec3_get_x (&uback),
                       graphene_vec3_get_y (&pback) + t * graphene_vec3_get_y (&uback));
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
 * Returns: %true if the point was successfully untransformed
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
      graphene_matrix_inverse (m, &inverse);
      graphene_matrix_transform_point (&inverse, p, res);
      return true;
    }

  graphene_matrix_transform_bounds (m, bounds, &bounds_t);
  if (!graphene_rect_contains_point (&bounds_t, p))
    return false;

  graphene_matrix_inverse (m, &inverse);
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
      graphene_matrix_inverse (m, &inverse);
      graphene_matrix_transform_bounds (&inverse, r, res);
      return;
    }

  graphene_matrix_transform_bounds (m, bounds, &bounds_t);
  if (!graphene_rect_intersection (r, &bounds_t, &rect))
    {
      graphene_rect_init (res, 0.f, 0.f, 0.f, 0.f);
      return;
    }

  graphene_matrix_inverse (m, &inverse);
  graphene_matrix_project_rect_bounds (&inverse, &rect, res);
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

static inline void
graphene_matrix_rotate_internal (graphene_simd4x4f_t *m,
                                 float                angle,
                                 graphene_simd4f_t    axis)
{
  float rad = angle * GRAPHENE_PI / 180.f;
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
  graphene_matrix_rotate_internal (&m->value, angle, axis->value);
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
  graphene_matrix_rotate_internal (&m->value, angle, graphene_simd4f_init (1.f, 0.f, 0.f, 0.f));
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
  graphene_matrix_rotate_internal (&m->value, angle, graphene_simd4f_init (0.f, 1.f, 0.f, 0.f));
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
  graphene_matrix_rotate_internal (&m->value, angle, graphene_simd4f_init (0.f, 0.f, 1.f, 0.f));
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

  m->value.y = graphene_simd4f_add (m_y, graphene_simd4f_mul (m_x, graphene_simd4f_splat (factor)));
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

  m->value.z = graphene_simd4f_add (m_z, graphene_simd4f_mul (m_x, graphene_simd4f_splat (factor)));
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

  m->value.z = graphene_simd4f_add (m_z, graphene_simd4f_mul (m_y, graphene_simd4f_splat (factor)));
}

static void
graphene_matrix_transpose_transform_vec4 (const graphene_matrix_t *m,
                                          const graphene_vec4_t   *v,
                                          graphene_vec4_t         *res)
{
  float x, y, z, w;

  x = graphene_simd4f_get_x (graphene_simd4f_sum (graphene_simd4f_mul (m->value.x, v->value)));
  y = graphene_simd4f_get_x (graphene_simd4f_sum (graphene_simd4f_mul (m->value.y, v->value)));
  z = graphene_simd4f_get_x (graphene_simd4f_sum (graphene_simd4f_mul (m->value.z, v->value)));
  w = graphene_simd4f_get_x (graphene_simd4f_sum (graphene_simd4f_mul (m->value.w, v->value)));

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
 * Since: 1.0
 */
void
graphene_matrix_inverse (const graphene_matrix_t *m,
                         graphene_matrix_t       *res)
{
  graphene_simd4x4f_inverse (&m->value, &res->value);
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

  ww = graphene_matrix_get_value (m, 3, 3);
  n = graphene_simd4f_splat (ww);

  res->value.x = graphene_simd4f_div (m->value.x, n);
  res->value.y = graphene_simd4f_div (m->value.y, n);
  res->value.z = graphene_simd4f_div (m->value.z, n);
  res->value.w = graphene_simd4f_div (m->value.w, n);
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

static bool
matrix_decompose_2d (const graphene_matrix_t *m,
                     graphene_point3d_t      *scale_r,
                     float                    shear_r[3],
                     graphene_quaternion_t   *rotate_r,
                     graphene_point3d_t      *translate_r)
{
  float A = graphene_matrix_get_value (m, 0, 0);
  float B = graphene_matrix_get_value (m, 1, 0);
  float C = graphene_matrix_get_value (m, 0, 1);
  float D = graphene_matrix_get_value (m, 1, 1);
  float scale_x, scale_y;
  float shear_xy;
  float rotate;

  if (A * D == B * C)
    return false;

  scale_x = sqrtf (A * A + B * B);
  A /= scale_x;
  B /= scale_x;

  shear_xy = A * C + B * D;
  C -= A * shear_xy;
  D -= B * shear_xy;

  scale_y = sqrtf (C * C + D * D);
  C /= scale_y;
  D /= scale_y;
  shear_xy /= scale_y;

  if (A * D < B * C)
    {
      A = -A;
      B = -B;
      C = -C;
      D = -D;

      shear_xy = -shear_xy;
      scale_x = -scale_x;
    }

  rotate = atan2f (B, A);
  graphene_quaternion_init (rotate_r, 0.f, 0.f, sin (rotate / 2.f), cos (rotate / 2.f));

  shear_r[XY_SHEAR] = shear_xy;
  graphene_point3d_init (scale_r, scale_x, scale_y, 1.f);

  graphene_point3d_init (translate_r,
                         graphene_matrix_get_value (m, 3, 0),
                         graphene_matrix_get_value (m, 3, 1),
                         0.f);

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
  graphene_simd4f_t dot, cross;

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

  /* isolate the perspective */
  if (graphene_simd4f_is_zero3 (local.value.w))
    {
      graphene_matrix_t tmp;

      /* perspective_r is the right hand side of the equation */
      perspective_r->value = local.value.w;

      /* solve the equation by inverting perspective and multiplying
       * the inverse with the perspective vector
       */
      graphene_matrix_inverse (&perspective, &tmp);
      graphene_matrix_transpose_transform_vec4 (&tmp, perspective_r, perspective_r);

      /* clear the perspective partition */
      local.value.w = graphene_simd4f_init (0.f, 0.f, 0.f, 1.f);
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
  local.value.x = graphene_simd4f_div (local.value.x, graphene_simd4f_splat (scale_x));

  /* compute XY shear factor and the second row orthogonal to the first */
  shear_xy = graphene_simd4f_get_x (graphene_simd4f_dot4 (local.value.x, local.value.y));
  local.value.y = graphene_simd4f_sub (local.value.y, graphene_simd4f_mul (local.value.x, graphene_simd4f_splat (shear_xy)));

  /* now, compute the Y scale factor and normalize the second row */
  scale_y = graphene_simd4f_get_x (graphene_simd4f_length4 (local.value.y));
  local.value.y = graphene_simd4f_div (local.value.y, graphene_simd4f_splat (scale_y));
  shear_xy /= scale_y;

  /* compute XZ and YZ shears, make the third row orthogonal */
  shear_xz = graphene_simd4f_get_x (graphene_simd4f_dot4 (local.value.x, local.value.z));
  local.value.z = graphene_simd4f_sub (local.value.z, graphene_simd4f_mul (local.value.x, graphene_simd4f_splat (shear_xz)));
  shear_yz = graphene_simd4f_get_x (graphene_simd4f_dot4 (local.value.y, local.value.z));
  local.value.z = graphene_simd4f_sub (local.value.z, graphene_simd4f_mul (local.value.y, graphene_simd4f_splat (shear_yz)));

  /* next, get the Z scale and normalize the third row */
  scale_z = graphene_simd4f_get_x (graphene_simd4f_length4 (local.value.z));
  local.value.z = graphene_simd4f_div (local.value.z, graphene_simd4f_splat (scale_z));

  shear_xz /= scale_z;
  shear_yz /= scale_z;

  shear_r[XY_SHEAR] = shear_xy;
  shear_r[XZ_SHEAR] = shear_xz;
  shear_r[YZ_SHEAR] = shear_yz;

  /* at this point, the matrix is orthonormal. we check for a
   * coordinate system flip. if the determinant is -1, then
   * negate the matrix and the scaling factors
   */
  dot = graphene_simd4f_cross3 (local.value.y, local.value.z);
  cross = graphene_simd4f_dot4 (local.value.x, dot);
  if (graphene_simd4f_get_x (cross) < 0.f)
    {
      scale_x *= -1.f;
      scale_y *= -1.f;
      scale_z *= -1.f;

      graphene_simd4f_mul (local.value.x, graphene_simd4f_splat (-1.f));
      graphene_simd4f_mul (local.value.y, graphene_simd4f_splat (-1.f));
      graphene_simd4f_mul (local.value.z, graphene_simd4f_splat (-1.f));
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
 * Since: 1.0
 */
void
graphene_matrix_interpolate (const graphene_matrix_t *a,
                             const graphene_matrix_t *b,
                             double                   factor,
                             graphene_matrix_t       *res)
{
  graphene_point3d_t scale_a = { 1.f, 1.f, 1.f }, translate_a;
  graphene_vec4_t perspective_a;
  graphene_quaternion_t rotate_a;
  float shear_a[3] = { 0.f, 0.f, 0.f };

  graphene_point3d_t scale_b = { 1.f, 1.f, 1.f }, translate_b;
  graphene_vec4_t perspective_b;
  graphene_quaternion_t rotate_b;
  float shear_b[3] = { 0.f, 0.f, 0.f };

  graphene_point3d_t scale_r = { 1.f, 1.f, 1.f }, translate_r;
  graphene_quaternion_t rotate_r;
  graphene_matrix_t tmp;
  float shear;

  if (graphene_matrix_is_2d (a) &&
      graphene_matrix_is_2d (b))
    {
      graphene_vec4_init (&perspective_a, 0.f, 0.f, 0.f, 1.f);
      graphene_vec4_init (&perspective_b, 0.f, 0.f, 0.f, 1.f);
      matrix_decompose_2d (a, &scale_a, shear_a, &rotate_a, &translate_a);
      matrix_decompose_2d (b, &scale_b, shear_b, &rotate_b, &translate_b);
    }
  else
    {
      matrix_decompose_3d (a, &scale_a, shear_a, &rotate_a, &translate_a, &perspective_a);
      matrix_decompose_3d (b, &scale_b, shear_b, &rotate_b, &translate_b, &perspective_b);
    }

  res->value.w = graphene_simd4f_interpolate (perspective_a.value,
                                              perspective_b.value,
                                              factor);

  graphene_point3d_interpolate (&translate_a, &translate_b, factor, &translate_r);
  graphene_matrix_translate (res, &translate_r);

  graphene_quaternion_slerp (&rotate_a, &rotate_b, factor, &rotate_r);
  graphene_quaternion_to_matrix (&rotate_r, &tmp);
  if (!graphene_matrix_is_identity (&tmp))
    graphene_matrix_multiply (res, &tmp, res);

  shear = shear_a[YZ_SHEAR] + (shear_b[YZ_SHEAR] - shear_a[YZ_SHEAR]) * factor;
  if (shear != 0.f)
    graphene_matrix_skew_yz (res, shear);

  shear = shear_a[XZ_SHEAR] + (shear_b[XZ_SHEAR] - shear_a[XZ_SHEAR]) * factor;
  if (shear != 0.f)
    graphene_matrix_skew_xz (res, shear);

  shear = shear_a[XY_SHEAR] + (shear_b[XY_SHEAR] - shear_a[XY_SHEAR]) * factor;
  if (shear != 0.f)
    graphene_matrix_skew_xy (res, shear);

  graphene_point3d_interpolate (&scale_a, &scale_b, factor, &scale_r);
  if (scale_r.x != 1.f && scale_r.y != 1.f && scale_r.z != 1.f)
    graphene_matrix_scale (res, scale_r.x, scale_r.y, scale_r.z);
}

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
               "%.5f %.5f %.5f %.5f\n",
               graphene_matrix_get_value (m, i, 0),
               graphene_matrix_get_value (m, i, 1),
               graphene_matrix_get_value (m, i, 2),
               graphene_matrix_get_value (m, i, 3));
    }
}
