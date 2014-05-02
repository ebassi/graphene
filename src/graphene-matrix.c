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
  g_return_val_if_fail (m != NULL, FALSE);

  return FALSE;
}

gboolean
graphene_matrix_is_singular (const graphene_matrix_t *m)
{
  return graphene_matrix_determinant (m) == 0.0f;
}

float
graphene_matrix_get_value (const graphene_matrix_t *m,
                           unsigned int             row,
                           unsigned int             col)
{
  graphene_simd4f_t r;
  float c;

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
    }

  return c;
}

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

void
graphene_matrix_transform_vec3 (const graphene_matrix_t *m,
                                const graphene_vec3_t   *v,
                                graphene_vec3_t         *res)
{
  g_return_if_fail (m != NULL && v != NULL);
  g_return_if_fail (res != NULL);

  graphene_simd4x4f_vec3_mul (&m->value, &v->value, &res->value);
}

void
graphene_matrix_transform_vec4 (const graphene_matrix_t *m,
                                const graphene_vec4_t   *v,
                                graphene_vec4_t         *res)
{
  g_return_if_fail (m != NULL && v != NULL);
  g_return_if_fail (res != NULL);

  graphene_simd4x4f_vec4_mul (&m->value, &v->value, &res->value);
}

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
  graphene_point_init (&points[1], graphene_rect_get_x (r) + graphene_rect_get_width (r), graphene_rect_get_y (r));
  graphene_point_init (&points[2], graphene_rect_get_x (r), graphene_rect_get_y (r) + graphene_rect_get_height (r));
  graphene_rect_get_bottom_right (r, &points[3]);

  graphene_matrix_transform_point (m, &points[0], &ret[0]);
  graphene_matrix_transform_point (m, &points[1], &ret[1]);
  graphene_matrix_transform_point (m, &points[2], &ret[2]);
  graphene_matrix_transform_point (m, &points[3], &ret[3]);

  graphene_quad_init (res, &ret[0], &ret[1], &ret[2], &ret[3]);
}

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
  graphene_point_init (&points[1], graphene_rect_get_x (r) + graphene_rect_get_width (r), graphene_rect_get_y (r));
  graphene_point_init (&points[2], graphene_rect_get_x (r), graphene_rect_get_y (r) + graphene_rect_get_height (r));
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
