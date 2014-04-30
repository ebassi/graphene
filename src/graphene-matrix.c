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

#include "graphene-point3d.h"
#include "graphene-simd4x4f.h"
#include "graphene-vectors-private.h"

static inline float
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
  g_return_val_if_fail (m != NULL, 0.f);

  return 0.f;
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
                                 const graphene_vec3_t   *p,
                                 graphene_vec3_t         *res)
{
}

void
graphene_matrix_transform_rect (const graphene_matrix_t *m,
                                const graphene_rect_t   *r,
                                graphene_quad_t         *res)
{
}

void
graphene_matrix_transform_bounds (const graphene_matrix_t *m,
                                  const graphene_rect_t   *r,
                                  graphene_rect_t         *res)
{
}

gboolean
graphene_matrix_untransform_point (const graphene_matrix_t *m,
                                   const graphene_point_t  *p,
                                   const graphene_rect_t   *bounds,
                                   graphene_point_t        *res)
{
  return FALSE;
}

gboolean
graphene_matrix_untransform_bounds (const graphene_matrix_t *m,
                                    const graphene_rect_t   *r,
                                    const graphene_rect_t   *bounds,
                                    graphene_rect_t         *res)
{
  return FALSE;
}

void
graphene_matrix_translate (graphene_matrix_t     *m,
                           const graphene_vec3_t *pos)
{
  graphene_simd4x4f_t trans_m;

  graphene_simd4x4f_translation (&trans_m,
                                 graphene_vec3_get_x (pos),
                                 graphene_vec3_get_y (pos),
                                 graphene_vec3_get_z (pos));
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
}

void
graphene_matrix_skew_xz (graphene_matrix_t *m,
                         float              factor)
{
}

void
graphene_matrix_skew_yz (graphene_matrix_t *m,
                         float              factor)
{
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
