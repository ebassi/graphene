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

#include "config.h"

#include "graphene-matrix.h"

#include "graphene-point.h"
#include "graphene-point3d.h"
#include "graphene-rect.h"
#include "graphene-simd4x4f.h"
#include "graphene-quad.h"
#include "graphene-quaternion.h"
#include "graphene-vectors-private.h"

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
  return g_slice_new (graphene_matrix_t);
}

void
graphene_matrix_free (graphene_matrix_t *m)
{
  if (G_LIKELY (m != NULL))
    g_slice_free (graphene_matrix_t, m);
}

/**
 * graphene_matrix_to_float:
 * @m: ...
 * @v: (array fixed-size=16) (out caller-allocates): ...
 *
 * ...
 *
 * Since: 1.0
 */
void
graphene_matrix_to_float (const graphene_matrix_t *m,
                          float                   *v)
{
  g_return_if_fail (m != NULL);
  g_return_if_fail (v != NULL);

  graphene_simd4x4f_to_float (&m->value, v);
}

graphene_matrix_t *
graphene_matrix_init_identity (graphene_matrix_t *m)
{
  g_return_val_if_fail (m != NULL, NULL);

  graphene_simd4x4f_init_identity (&m->value);

  return m;
}

/**
 * graphene_matrix_init_from_float:
 * @m: ...
 * @v: (array fixed-size=16): ...
 *
 * ...
 *
 * Returns: (transfer none): ...
 *
 * Since: 1.0
 */
graphene_matrix_t *
graphene_matrix_init_from_float (graphene_matrix_t *m,
                                 const float       *v)
{
  g_return_val_if_fail (m != NULL, NULL);
  g_return_val_if_fail (v != NULL, m);

  graphene_simd4x4f_init_from_float (&m->value, v);

  return m;
}

graphene_matrix_t *
graphene_matrix_init_from_vec4 (graphene_matrix_t     *m,
                                const graphene_vec4_t *v0,
                                const graphene_vec4_t *v1,
                                const graphene_vec4_t *v2,
                                const graphene_vec4_t *v3)
{
  g_return_val_if_fail (m != NULL, NULL);
  g_return_val_if_fail (v0 != NULL && v1 != NULL && v2 != NULL && v3 != NULL, m);

  m->value = graphene_simd4x4f_init (v0->value,
                                     v1->value,
                                     v2->value,
                                     v3->value);

  return m;
}

graphene_matrix_t *
graphene_matrix_init_from_matrix (graphene_matrix_t       *m,
                                  const graphene_matrix_t *src)
{
  g_return_val_if_fail (m != NULL, NULL);
  g_return_val_if_fail (src != NULL, NULL);

  m->value = src->value;

  return m;
}

graphene_matrix_t *
graphene_matrix_init_perspective (graphene_matrix_t *m,
                                  float              fovy,
                                  float              aspect,
                                  float              z_near,
                                  float              z_far)
{
  g_return_val_if_fail (m != NULL, NULL);

  graphene_simd4x4f_init_perspective (&m->value, fovy, aspect, z_near, z_far);

  return m;
}

graphene_matrix_t *
graphene_matrix_init_ortho (graphene_matrix_t *m,
                            float              left,
                            float              right,
                            float              top,
                            float              bottom,
                            float              z_near,
                            float              z_far)
{
  g_return_val_if_fail (m != NULL, NULL);

  graphene_simd4x4f_init_ortho (&m->value, left, right, top, bottom, z_near, z_far);

  return m;
}

graphene_matrix_t *
graphene_matrix_init_look_at (graphene_matrix_t     *m,
                              const graphene_vec3_t *eye,
                              const graphene_vec3_t *center,
                              const graphene_vec3_t *up)
{
  g_return_val_if_fail (m != NULL, NULL);

  graphene_simd4x4f_init_look_at (&m->value, eye->value, center->value, up->value);

  return m;
}

graphene_matrix_t *
graphene_matrix_init_scale (graphene_matrix_t *m,
                            float              x,
                            float              y,
                            float              z)
{
  g_return_val_if_fail (m != NULL, NULL);

  m->value =
    graphene_simd4x4f_init (graphene_simd4f_init (   x, 0.0f, 0.0f, 0.0f),
                            graphene_simd4f_init (0.0f,    y, 0.0f, 0.0f),
                            graphene_simd4f_init (0.0f, 0.0f,    x, 0.0f),
                            graphene_simd4f_init (0.0f, 0.0f, 0.0f, 1.0f));

  return m;
}

graphene_matrix_t *
graphene_matrix_init_translate (graphene_matrix_t        *m,
                                const graphene_point3d_t *p)
{
  g_return_val_if_fail (m != NULL, NULL);
  g_return_val_if_fail (p != NULL, m);

  m->value =
    graphene_simd4x4f_init (graphene_simd4f_init (1.0f, 0.0f, 0.0f, 0.0f),
                            graphene_simd4f_init (0.0f, 1.0f, 0.0f, 0.0f),
                            graphene_simd4f_init (0.0f, 0.0f, 1.0f, 0.0f),
                            graphene_simd4f_init (p->x, p->y, p->z, 1.0f));

  return m;
}

graphene_matrix_t *
graphene_matrix_init_skew (graphene_matrix_t *m,
                           float              x_skew,
                           float              y_skew)
{
  float t_x, t_y;

  g_return_val_if_fail (m != NULL, NULL);

  t_x = tanf (x_skew);
  t_y = tanf (y_skew);

  m->value =
    graphene_simd4x4f_init (graphene_simd4f_init (1.0f,  t_y, 0.0f, 0.0f),
                            graphene_simd4f_init ( t_x, 1.0f, 0.0f, 0.0f),
                            graphene_simd4f_init (0.0f, 0.0f, 1.0f, 0.0f),
                            graphene_simd4f_init (0.0f, 0.0f, 0.0f, 1.0f));

  return m;
}

graphene_matrix_t *
graphene_matrix_init_rotate (graphene_matrix_t     *m,
                             float                  angle,
                             const graphene_vec3_t *axis)
{
  g_return_val_if_fail (m != NULL, NULL);
  g_return_val_if_fail (axis != NULL, m);

  graphene_simd4x4f_rotation (&m->value, angle, axis->value);

  return m;
}

gboolean
graphene_matrix_is_identity (const graphene_matrix_t *m)
{
  g_return_val_if_fail (m != NULL, FALSE);

  return graphene_simd4x4f_is_identity (&m->value);
}

gboolean
graphene_matrix_is_2d (const graphene_matrix_t *m)
{
  g_return_val_if_fail (m != NULL, FALSE);

  return graphene_simd4x4f_is_2d (&m->value);
}

gboolean
graphene_matrix_is_backface_visible (const graphene_matrix_t *m)
{
  graphene_matrix_t tmp;

  g_return_val_if_fail (m != NULL, FALSE);

  graphene_matrix_inverse (m, &tmp);

  /* inverse.zz < 0 */
  return graphene_matrix_get_value (&tmp, 2, 2) < 0.f;
}

gboolean
graphene_matrix_is_singular (const graphene_matrix_t *m)
{
  return graphene_matrix_determinant (m) == 0.0f;
}

/**
 * graphene_matrix_get_row:
 * @m: ...
 * @index_: ...
 * @res: (out caller-allocates): ...
 *
 * ...
 *
 * Since: 1.0
 */
void
graphene_matrix_get_row (const graphene_matrix_t *m,
                         unsigned int             index_,
                         graphene_vec4_t         *res)
{
  g_return_if_fail (m != NULL);
  g_return_if_fail (index_ >= 0 && index_ < 4);
  g_return_if_fail (res != NULL);

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
      g_assert_not_reached ();
    }
}

float
graphene_matrix_get_value (const graphene_matrix_t *m,
                           unsigned int             row,
                           unsigned int             col)
{
  graphene_simd4f_t r;
  float c = 0.f;

  g_return_val_if_fail (m != NULL, 0.f);
  g_return_val_if_fail (row >= 0 && row < 4, 0.f);
  g_return_val_if_fail (col >= 0 && row < 4, 0.f);

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
      g_assert_not_reached ();
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
      g_assert_not_reached ();
    }

  return c;
}

/**
 * graphene_matrix_multiply:
 * @a: ...
 * @b: ...
 * @res: (out caller-allocates): ...
 *
 * ...
 *
 * Since: 1.0
 */
void
graphene_matrix_multiply (const graphene_matrix_t *a,
                          const graphene_matrix_t *b,
                          graphene_matrix_t       *res)
{
  g_return_if_fail (a != NULL && b != NULL);
  g_return_if_fail (res != NULL);

  graphene_simd4x4f_mul (&a->value, &b->value, &res->value);
}

float
graphene_matrix_determinant (const graphene_matrix_t *m)
{
  graphene_simd4f_t det;

  g_return_val_if_fail (m != NULL, 0.f);

  graphene_simd4x4f_determinant (&m->value, &det, NULL);

  return graphene_simd4f_get_x (det);
}

/**
 * graphene_matrix_transform_vec3:
 * @m: ...
 * @v: ...
 * @res: (out caller-allocates): ...
 *
 * ...
 *
 * Since: 1.0
 */
void
graphene_matrix_transform_vec3 (const graphene_matrix_t *m,
                                const graphene_vec3_t   *v,
                                graphene_vec3_t         *res)
{
  g_return_if_fail (m != NULL && v != NULL);
  g_return_if_fail (res != NULL);

  graphene_simd4x4f_vec3_mul (&m->value, &v->value, &res->value);
}

/**
 * graphene_matrix_transform_vec4:
 * @m: ...
 * @v: ...
 * @res: (out caller-allocates): ...
 *
 * ...
 *
 * Since: 1.0
 */
void
graphene_matrix_transform_vec4 (const graphene_matrix_t *m,
                                const graphene_vec4_t   *v,
                                graphene_vec4_t         *res)
{
  g_return_if_fail (m != NULL && v != NULL);
  g_return_if_fail (res != NULL);

  graphene_simd4x4f_vec4_mul (&m->value, &v->value, &res->value);
}

/**
 * graphene_matrix_transform_point:
 * @m: ...
 * @p: ...
 * @res: (out caller-allocates): ...
 *
 * ...
 *
 * Since: 1.0
 */
void
graphene_matrix_transform_point (const graphene_matrix_t *m,
                                 const graphene_point_t  *p,
                                 graphene_point_t        *res)
{
  graphene_simd4f_t vec3;

  g_return_if_fail (m != NULL && p != NULL);
  g_return_if_fail (res != NULL);

  vec3 = graphene_simd4f_init (p->x, p->y, 0.0f, 0.0f);
  graphene_simd4x4f_vec3_mul (&m->value, &vec3, &vec3);

  res->x = graphene_simd4f_get_x (vec3);
  res->y = graphene_simd4f_get_y (vec3);
}

/**
 * graphene_matrix_transform_rect:
 * @m: ...
 * @r: ...
 * @res: (out caller-allocates): ...
 *
 * ...
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

  g_return_if_fail (m != NULL && r != NULL);
  g_return_if_fail (res != NULL);

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
 * @m: ...
 * @r: ...
 * @res: (out caller-allocates): ...
 *
 * ...
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

  g_return_if_fail (m != NULL && r != NULL);
  g_return_if_fail (res != NULL);

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
 * @m: ...
 * @p: ...
 * @res: (out caller-allocates): ...
 *
 * ...
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

  g_return_if_fail (m != NULL);
  g_return_if_fail (p != NULL);
  g_return_if_fail (res != NULL);

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

gboolean
graphene_matrix_untransform_point (const graphene_matrix_t *m,
                                   const graphene_point_t  *p,
                                   const graphene_rect_t   *bounds,
                                   graphene_point_t        *res)
{
  graphene_matrix_t inverse;
  graphene_rect_t bounds_t;

  g_return_val_if_fail (m != NULL, FALSE);
  g_return_val_if_fail (p != NULL, FALSE);
  g_return_val_if_fail (bounds != NULL, FALSE);
  g_return_val_if_fail (res != NULL, FALSE);

  if (graphene_matrix_is_2d (m))
    {
      graphene_matrix_inverse (m, &inverse);
      graphene_matrix_transform_point (&inverse, p, res);
      return TRUE;
    }

  graphene_matrix_transform_bounds (m, bounds, &bounds_t);
  if (!graphene_rect_contains_point (&bounds_t, p))
    return FALSE;

  graphene_matrix_inverse (m, &inverse);
  graphene_matrix_project_point (&inverse, p, res);

  return TRUE;
}

void
graphene_matrix_untransform_bounds (const graphene_matrix_t *m,
                                    const graphene_rect_t   *r,
                                    const graphene_rect_t   *bounds,
                                    graphene_rect_t         *res)
{
  graphene_matrix_t inverse;
  graphene_rect_t bounds_t;
  graphene_rect_t rect;

  g_return_if_fail (m != NULL && r != NULL);
  g_return_if_fail (bounds != NULL);
  g_return_if_fail (res != NULL);

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

void
graphene_matrix_translate (graphene_matrix_t        *m,
                           const graphene_point3d_t *pos)
{
  graphene_simd4x4f_t trans_m;

  graphene_simd4x4f_translation (&trans_m, pos->x, pos->y, pos->z);
  graphene_simd4x4f_mul (&m->value, &trans_m, &m->value);
}

void
graphene_matrix_rotate (graphene_matrix_t     *m,
                        float                  angle,
                        const graphene_vec3_t *axis)
{
  graphene_simd4x4f_t rot_m;

  graphene_simd4x4f_rotation (&rot_m, angle, axis->value);
  graphene_simd4x4f_mul (&m->value, &rot_m, &m->value);
}

void
graphene_matrix_scale (graphene_matrix_t *m,
                       float              factor_x,
                       float              factor_y,
                       float              factor_z)
{
  graphene_simd4x4f_t scale_m;

  graphene_simd4x4f_scale (&scale_m, factor_x, factor_y, factor_z);
  graphene_simd4x4f_mul (&m->value, &scale_m, &m->value);
}

void
graphene_matrix_skew_xy (graphene_matrix_t *m,
                         float              factor)
{
  graphene_simd4f_t m_x, m_y;

  g_return_if_fail (m != NULL);

  m_x = m->value.x;
  m_y = m->value.y;

  m->value.y = graphene_simd4f_add (m_y, graphene_simd4f_mul (m_x, graphene_simd4f_splat (factor)));
}

void
graphene_matrix_skew_xz (graphene_matrix_t *m,
                         float              factor)
{
  graphene_simd4f_t m_x, m_z;

  g_return_if_fail (m != NULL);

  m_x = m->value.x;
  m_z = m->value.z;

  m->value.z = graphene_simd4f_add (m_z, graphene_simd4f_mul (m_x, graphene_simd4f_splat (factor)));
}

void
graphene_matrix_skew_yz (graphene_matrix_t *m,
                         float              factor)
{
  graphene_simd4f_t m_y, m_z;

  g_return_if_fail (m != NULL);

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

void
graphene_matrix_transpose (const graphene_matrix_t *m,
                           graphene_matrix_t       *res)
{
  g_return_if_fail (m != NULL);
  g_return_if_fail (res != NULL);

  graphene_simd4x4f_transpose (&m->value, &res->value);
}

void
graphene_matrix_inverse (const graphene_matrix_t *m,
                         graphene_matrix_t       *res)
{
  g_return_if_fail (m != NULL);
  g_return_if_fail (res != NULL);

  graphene_simd4x4f_inverse (&m->value, &res->value);
}

void
graphene_matrix_perspective (const graphene_matrix_t *m,
                             float                    depth,
                             graphene_matrix_t       *res)
{

  g_return_if_fail (m != NULL);
  g_return_if_fail (depth > 0.0f);
  g_return_if_fail (res != NULL);

  res->value = m->value;

  graphene_simd4x4f_perspective (&res->value, depth);
}

void
graphene_matrix_normalize (const graphene_matrix_t *m,
                           graphene_matrix_t       *res)
{
  graphene_simd4f_t n;
  float ww;

  g_return_if_fail (m != NULL);
  g_return_if_fail (res != NULL);

  ww = graphene_matrix_get_value (m, 3, 3);
  n = graphene_simd4f_splat (ww);

  res->value.x = graphene_simd4f_div (m->value.x, n);
  res->value.y = graphene_simd4f_div (m->value.y, n);
  res->value.z = graphene_simd4f_div (m->value.z, n);
  res->value.w = graphene_simd4f_div (m->value.w, n);
}

float
graphene_matrix_get_x_scale (const graphene_matrix_t *m)
{
  g_return_val_if_fail (m != NULL, 0.f);

  return graphene_simd4f_get_x (m->value.x);
}

float
graphene_matrix_get_y_scale (const graphene_matrix_t *m)
{
  g_return_val_if_fail (m != NULL, 0.f);

  return graphene_simd4f_get_y (m->value.y);
}

float
graphene_matrix_get_z_scale (const graphene_matrix_t *m)
{
  g_return_val_if_fail (m != NULL, 0.f);

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

static gboolean
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
    return FALSE;

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

  return TRUE;
}

static gboolean
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
    return FALSE;

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
    return FALSE;

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

  return TRUE;
}

/**
 * graphene_matrix_interpolate:
 * @a: ...
 * @b: ...
 * @factor: ...
 * @res: (out caller-allocates): ...
 *
 * ...
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

  g_return_if_fail (a != NULL && b != NULL);
  g_return_if_fail (res != NULL);

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
    graphene_matrix_multiply (&tmp, res, res);

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
  if (scale_r.x != 1.f && scale_r.y != 1.f && scale_r.z != 0.f)
    graphene_matrix_scale (res, scale_r.x, scale_r.y, scale_r.z);
}
