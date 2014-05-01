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

#include "graphene-matrix.h"
#include "graphene-simd4f.h"

graphene_quaternion_t *
graphene_quaternion_alloc (void)
{
  return g_slice_new (graphene_quaternion_t);
}

void
graphene_quaternion_free (graphene_quaternion_t *q)
{
  if (G_LIKELY (q != NULL))
    g_slice_free (graphene_quaternion_t, q);
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

  res->x = graphene_simd4f_get_x (sum);
  res->y = graphene_simd4f_get_y (sum);
  res->z = graphene_simd4f_get_z (sum);
  res->w = graphene_simd4f_get_w (sum);
}
