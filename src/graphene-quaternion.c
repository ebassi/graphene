/* graphene-quaternion.c: Quaternion
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

#include <math.h>

#include "graphene-quaternion.h"

#include "graphene-simd4f.h"
#include "graphene-vec3.h"
#include "graphene-vec4.h"
#include "graphene-matrix.h"

graphene_quaternion_t *
graphene_quaternion_alloc (void)
{
  return calloc (1, sizeof (graphene_quaternion_t));
}

void
graphene_quaternion_free (graphene_quaternion_t *q)
{
  free (q);
}

graphene_quaternion_t *
graphene_quaternion_init (graphene_quaternion_t *q,
                          float                  x,
                          float                  y,
                          float                  z,
                          float                  w)
{
  g_return_val_if_fail (q != NULL, NULL);

  q->x = x;
  q->y = y;
  q->z = z;
  q->w = w;

  return q;
}

graphene_quaternion_t *
graphene_quaternion_init_identity (graphene_quaternion_t *q)
{
  g_return_val_if_fail (q != NULL, NULL);

  q->w = 1.f;
  q->x = q->y = q->z = 0.f;

  return q;
}

graphene_quaternion_t *
graphene_quaternion_init_from_quaternion (graphene_quaternion_t       *q,
                                          const graphene_quaternion_t *src)
{
  g_return_val_if_fail (q != NULL, NULL);
  g_return_val_if_fail (src != NULL, q);

  *q = *src;

  return q;
}

static graphene_quaternion_t *
graphene_quaternion_init_from_simd4f (graphene_quaternion_t *q,
                                      graphene_simd4f_t      v)
{
  q->x = graphene_simd4f_get_x (v);
  q->y = graphene_simd4f_get_y (v);
  q->z = graphene_simd4f_get_z (v);
  q->w = graphene_simd4f_get_w (v);

  return q;
}

graphene_quaternion_t *
graphene_quaternion_init_from_vec4 (graphene_quaternion_t *q,
                                    const graphene_vec4_t *src)
{
  g_return_val_if_fail (q != NULL, NULL);
  g_return_val_if_fail (src != NULL, q);

  return graphene_quaternion_init_from_simd4f (q, src->value);
}

void
graphene_quaternion_to_vec4 (const graphene_quaternion_t *q,
                             graphene_vec4_t             *res)
{
  g_return_if_fail (q != NULL);
  g_return_if_fail (res != NULL);

  res->value = graphene_simd4f_init (q->x, q->y, q->z, q->w);
}

graphene_quaternion_t *
graphene_quaternion_init_from_matrix (graphene_quaternion_t   *q,
                                      const graphene_matrix_t *m)
{
  float xx, yy, zz;

  g_return_val_if_fail (q != NULL, NULL);
  g_return_val_if_fail (m != NULL, q);

  xx = graphene_matrix_get_value (m, 0, 0);
  yy = graphene_matrix_get_value (m, 1, 1);
  zz = graphene_matrix_get_value (m, 2, 2);

  q->w = 0.5f * sqrtf (MAX (1 + xx + yy + zz, 0.f));
  q->x = 0.5f * sqrtf (MAX (1 + xx - yy - zz, 0.f));
  q->y = 0.5f * sqrtf (MAX (1 - xx + yy - zz, 0.f));
  q->z = 0.5f * sqrtf (MAX (1 - xx - yy + zz, 0.f));

  if (graphene_matrix_get_value (m, 2, 1) > graphene_matrix_get_value (m, 1, 2))
    q->x = -q->x;

  if (graphene_matrix_get_value (m, 0, 2) > graphene_matrix_get_value (m, 2, 0))
    q->y = -q->y;

  if (graphene_matrix_get_value (m, 1, 0) > graphene_matrix_get_value (m, 0, 1))
    q->z = -q->z;

  return q;
}

void
graphene_quaternion_to_matrix (const graphene_quaternion_t *q,
                               graphene_matrix_t           *m)
{
  graphene_vec4_t m_x, m_y, m_z, m_w;

  g_return_if_fail (q != NULL);
  g_return_if_fail (m != NULL);

  graphene_vec4_init (&m_x,
                      1.f - 2.f * (q->y * q->y + q->z * q->z),
                            2.f * (q->x * q->y + q->w * q->z),
                            2.f * (q->x * q->z - q->w * q->y),
                      0.f);
  graphene_vec4_init (&m_y,
                            2.f * (q->x * q->y - q->w * q->z),
                      1.f - 2.f * (q->x * q->x + q->z * q->z),
                            2.f * (q->y * q->z + q->w * q->x),
                      0.f);
  graphene_vec4_init (&m_z,
                            2.f * (q->x * q->z + q->w * q->y),
                            2.f * (q->y * q->z - q->w * q->x),
                      1.f - 2.f * (q->x * q->x + q->y * q->y),
                      0.f);
  graphene_vec4_init (&m_w, 0.f, 0.f, 0.f, 0.f);

  graphene_matrix_init_from_vec4 (m, &m_x, &m_y, &m_z, &m_w);
}

void
graphene_quaternion_slerp (const graphene_quaternion_t *a,
                           const graphene_quaternion_t *b,
                           float                        factor,
                           graphene_quaternion_t       *res)
{
  float theta, r_sin_theta, right_v, left_v, dot;
  graphene_simd4f_t v_a, v_b, left, right, sum;

  g_return_if_fail (a != NULL && b != NULL);
  g_return_if_fail (res != NULL);

  v_a = graphene_simd4f_init (a->x, a->y, a->z, a->w);
  v_b = graphene_simd4f_init (b->x, b->y, b->z, b->w);

  dot = CLAMP (graphene_simd4f_get_x (graphene_simd4f_dot4 (v_a, v_b)), -1.f, 1.f);
  if (dot == 1.f)
    {
      *res = *a;
      return;
    }

  theta = acos (dot);
  r_sin_theta = 1.f / sqrtf (1.f - dot * dot);
  right_v = sinf (factor * theta) * r_sin_theta;
  left_v = cosf (factor * theta) - dot * right_v;

  left = graphene_simd4f_init (a->x, a->y, a->z, a->w);
  right = graphene_simd4f_init (b->x, b->y, b->z, b->w);

  left = graphene_simd4f_mul (left, graphene_simd4f_splat (left_v));
  right = graphene_simd4f_mul (right, graphene_simd4f_splat (right_v));
  sum = graphene_simd4f_add (left, right);

  graphene_quaternion_init_from_simd4f (res, sum);
}

graphene_quaternion_t *
graphene_quaternion_init_from_angles (graphene_quaternion_t *q,
                                      float                  deg_x,
                                      float                  deg_y,
                                      float                  deg_z)
{
  float sin_x, sin_y, sin_z;
  float cos_x, cos_y, cos_z;

  g_return_val_if_fail (q != NULL, NULL);

  sin_x = sinf (deg_x * (GRAPHENE_PI / 180.f) * .5f);
  sin_y = sinf (deg_y * (GRAPHENE_PI / 180.f) * .5f);
  sin_z = sinf (deg_z * (GRAPHENE_PI / 180.f) * .5f);

  cos_x = cosf (deg_x * (GRAPHENE_PI / 180.f) * .5f);
  cos_y = cosf (deg_y * (GRAPHENE_PI / 180.f) * .5f);
  cos_z = cosf (deg_z * (GRAPHENE_PI / 180.f) * .5f);

  q->w = sin_x * sin_y * sin_z + cos_x * cos_y * cos_z;
  q->x = sin_x * cos_y * cos_z + cos_x * sin_y * sin_z;
  q->y = cos_x * sin_y * cos_z + sin_x * cos_y * sin_z;
  q->x = cos_x * cos_y * sin_z + sin_x * sin_y * cos_z;

  return q;
}

graphene_quaternion_t *
graphene_quaternion_init_from_angle_vec3 (graphene_quaternion_t *q,
                                          float                  angle,
                                          const graphene_vec3_t *axis)
{
  float sin_a, cos_a;
  graphene_simd4f_t axis_n;

  g_return_val_if_fail (q != NULL, NULL);
  g_return_val_if_fail (axis != NULL, q);

  sin_a = sinf (angle / 2.f);
  cos_a = cosf (angle / 2.f);
  axis_n = graphene_simd4f_mul (graphene_simd4f_normalize3 (axis->value),
                                graphene_simd4f_splat (sin_a));

  q->x = graphene_simd4f_get_x (axis_n);
  q->y = graphene_simd4f_get_y (axis_n);
  q->z = graphene_simd4f_get_z (axis_n);
  q->w = cos_a;

  return q;
}

void
graphene_quaternion_to_angle_vec3 (const graphene_quaternion_t *q,
                                   float                       *angle,
                                   graphene_vec3_t             *axis)
{
  graphene_quaternion_t q_n;
  float cos_a, sin_a;

  g_return_if_fail (q != NULL);
  g_return_if_fail (angle != NULL);
  g_return_if_fail (axis != NULL);

  graphene_quaternion_normalize (q, &q_n);

  cos_a = q_n.w;

  *angle = acosf (cos_a) * 2.f;

  sin_a = sqrtf (1.f - cos_a * cos_a);
  if (fabsf (sin_a) < 0.00005)
    sin_a = 1.f;

  graphene_vec3_init (axis, q_n.x / sin_a, q_n.y / sin_a, q_n.z / sin_a);
}

gboolean
graphene_quaternion_equal (const graphene_quaternion_t *a,
                           const graphene_quaternion_t *b)
{
  graphene_simd4f_t v_a, v_b;

  if (a == b)
    return TRUE;

  if (a == NULL || b == NULL)
    return FALSE;

  v_a = graphene_simd4f_init (a->x, a->y, a->z, a->w);
  v_b = graphene_simd4f_init (b->x, b->y, b->z, b->w);

  return graphene_simd4f_cmp_eq (v_a, v_b);
}

float
graphene_quaternion_dot (const graphene_quaternion_t *a,
                         const graphene_quaternion_t *b)
{
  graphene_simd4f_t v_a, v_b;

  g_return_val_if_fail (a != NULL && b != NULL, 0.f);

  v_a = graphene_simd4f_init (a->x, a->y, a->z, a->w);
  v_b = graphene_simd4f_init (b->x, b->y, b->z, b->w);

  return graphene_simd4f_get_x (graphene_simd4f_dot4 (v_a, v_b));
}

void
graphene_quaternion_invert (const graphene_quaternion_t *q,
                            graphene_quaternion_t       *res)
{
  g_return_if_fail (q != NULL);
  g_return_if_fail (res != NULL);

  res->x = -q->x;
  res->y = -q->y;
  res->z = -q->z;
}

void
graphene_quaternion_normalize (const graphene_quaternion_t *q,
                               graphene_quaternion_t       *res)
{
  graphene_simd4f_t v_q;

  g_return_if_fail (q != NULL);
  g_return_if_fail (res != NULL);

  v_q = graphene_simd4f_init (q->x, q->y, q->z, q->w);
  v_q = graphene_simd4f_normalize4 (v_q);

  graphene_quaternion_init_from_simd4f (res, v_q);
}
