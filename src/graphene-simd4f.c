/* graphene-simd4f.c
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

#include <string.h>
#include <math.h>

#include "graphene-simd4f.h"

#if defined(GRAPHENE_USE_SSE) || defined(GRAPHENE_USE_GCC) || defined(GRAPHENE_USE_ARM_NEON)

graphene_simd4f_t
(graphene_simd4f_init) (float x,
                        float y,
                        float z,
                        float w)
{
  return graphene_simd4f_init (x, y, z, w);
}

graphene_simd4f_t
(graphene_simd4f_init_zero) (void)
{
  return graphene_simd4f_init_zero ();
}

graphene_simd4f_t
(graphene_simd4f_init_4f) (const float *v)
{
  return graphene_simd4f_init_4f (v);
}

graphene_simd4f_t
(graphene_simd4f_init_3f) (const float *v)
{
  return graphene_simd4f_init_3f (v);
}

graphene_simd4f_t
(graphene_simd4f_init_2f) (const float *v)
{
  return graphene_simd4f_init_2f (v);
}

void
(graphene_simd4f_dup_4f) (const graphene_simd4f_t  s,
                          float                   *v)
{
  graphene_simd4f_dup_4f (s, v);
}

void
(graphene_simd4f_dup_3f) (const graphene_simd4f_t  s,
                          float                   *v)
{
  graphene_simd4f_dup_3f (s, v);
}

void
(graphene_simd4f_dup_2f) (const graphene_simd4f_t  s,
                        float                   *v)
{
  graphene_simd4f_dup_2f (s, v);
}

float
(graphene_simd4f_get_x) (const graphene_simd4f_t s)
{
  return graphene_simd4f_get_x (s);
}

float
(graphene_simd4f_get_y) (const graphene_simd4f_t s)
{
  return graphene_simd4f_get_y (s);
}

float
(graphene_simd4f_get_z) (const graphene_simd4f_t s)
{
  return graphene_simd4f_get_z (s);
}

float
(graphene_simd4f_get_w) (const graphene_simd4f_t s)
{
  return graphene_simd4f_get_w (s);
}

graphene_simd4f_t
(graphene_simd4f_splat) (float v)
{
  return graphene_simd4f_splat (v);
}

graphene_simd4f_t
(graphene_simd4f_splat_x) (graphene_simd4f_t v)
{
  return graphene_simd4f_splat_x (v);
}

graphene_simd4f_t
(graphene_simd4f_splat_y) (graphene_simd4f_t v)
{
  return graphene_simd4f_splat_y (v);
}

graphene_simd4f_t
(graphene_simd4f_splat_z) (graphene_simd4f_t v)
{
  return graphene_simd4f_splat_z (v);
}

graphene_simd4f_t
(graphene_simd4f_splat_w) (graphene_simd4f_t v)
{
  return graphene_simd4f_splat_w (v);
}

graphene_simd4f_t
(graphene_simd4f_reciprocal) (graphene_simd4f_t v)
{
  return graphene_simd4f_reciprocal (v);
}

graphene_simd4f_t
(graphene_simd4f_sqrt) (graphene_simd4f_t v)
{
  return graphene_simd4f_sqrt (v);
}

graphene_simd4f_t
(graphene_simd4f_rsqrt) (graphene_simd4f_t v)
{
  return graphene_simd4f_rsqrt (v);
}

graphene_simd4f_t
(graphene_simd4f_add) (const graphene_simd4f_t a,
                       const graphene_simd4f_t b)
{
  return graphene_simd4f_add (a, b);
}

graphene_simd4f_t
(graphene_simd4f_sub) (const graphene_simd4f_t a,
                       const graphene_simd4f_t b)
{
  return graphene_simd4f_sub (a, b);
}

graphene_simd4f_t
(graphene_simd4f_mul) (const graphene_simd4f_t a,
                       const graphene_simd4f_t b)
{
  return graphene_simd4f_mul (a, b);
}

graphene_simd4f_t
(graphene_simd4f_div) (const graphene_simd4f_t a,
                       const graphene_simd4f_t b)
{
  return graphene_simd4f_div (a, b);
}

graphene_simd4f_t
(graphene_simd4f_cross3) (const graphene_simd4f_t a,
                          const graphene_simd4f_t b)
{
  return graphene_simd4f_cross3 (a, b);
}

graphene_simd4f_t
(graphene_simd4f_min) (const graphene_simd4f_t a,
                       const graphene_simd4f_t b)
{
  return graphene_simd4f_min (a, b);
}

graphene_simd4f_t
(graphene_simd4f_max) (const graphene_simd4f_t a,
                       const graphene_simd4f_t b)
{
  return graphene_simd4f_max (a, b);
}

graphene_simd4f_t
(graphene_simd4f_shuffle_wxyz) (const graphene_simd4f_t s)
{
  return graphene_simd4f_shuffle_wxyz (s);
}

graphene_simd4f_t
(graphene_simd4f_shuffle_zwxy) (const graphene_simd4f_t s)
{
  return graphene_simd4f_shuffle_zwxy (s);
}

graphene_simd4f_t
(graphene_simd4f_shuffle_yzwx) (const graphene_simd4f_t s)
{
  return graphene_simd4f_shuffle_yzwx (s);
}

graphene_simd4f_t
(graphene_simd4f_zero_w) (const graphene_simd4f_t s)
{
  return graphene_simd4f_zero_w (s);
}

graphene_simd4f_t
(graphene_simd4f_zero_zw) (const graphene_simd4f_t s)
{
  return graphene_simd4f_zero_zw (s);
}

graphene_simd4f_t
(graphene_simd4f_merge_high) (const graphene_simd4f_t a,
                              const graphene_simd4f_t b)
{
  return graphene_simd4f_merge_high (a, b);
}

graphene_simd4f_t
(graphene_simd4f_flip_sign_0101) (const graphene_simd4f_t s)
{
  return graphene_simd4f_flip_sign_0101 (s);
}

graphene_simd4f_t
(graphene_simd4f_flip_sign_1010) (const graphene_simd4f_t s)
{
  return graphene_simd4f_flip_sign_1010 (s);
}

gboolean
(graphene_simd4f_cmp_eq) (const graphene_simd4f_t a,
                          const graphene_simd4f_t b)
{
  return graphene_simd4f_cmp_eq (a, b);
}

gboolean
(graphene_simd4f_cmp_neq) (const graphene_simd4f_t a,
                           const graphene_simd4f_t b)
{
  return graphene_simd4f_cmp_neq (a, b);
}
#else

graphene_simd4f_t
(graphene_simd4f_init) (float x,
                        float y,
                        float z,
                        float w)
{
  graphene_simd4f_t s = { x, y, z, w };
  return s;
}

graphene_simd4f_t
(graphene_simd4f_init_zero) (void)
{
  return graphene_simd4f_init (0.f, 0.f, 0.f, 0.f);
}

graphene_simd4f_t
(graphene_simd4f_init_4f) (const float *v)
{
  return graphene_simd4f_init (v[0], v[1], v[2], v[3]);
}

graphene_simd4f_t
(graphene_simd4f_init_3f) (const float *v)
{
  return graphene_simd4f_init (v[0], v[1], v[2], 0.f);
}

graphene_simd4f_t
(graphene_simd4f_init_2f) (const float *v)
{
  return graphene_simd4f_init (v[0], v[1], 0.f, 0.f);
}

void
(graphene_simd4f_dup_4f) (const graphene_simd4f_t  s,
                          float                   *v)
{
  memcpy (v, &s, sizeof (float) * 4);
}

void
(graphene_simd4f_dup_3f) (const graphene_simd4f_t  s,
                          float                   *v)
{
  memcpy (v, &s, sizeof (float) * 3);
}

void
(graphene_simd4f_dup_2f) (const graphene_simd4f_t  s,
                        float                   *v)
{
  memcpy (v, &s, sizeof (float) * 2);
}

float
(graphene_simd4f_get_x) (const graphene_simd4f_t s)
{
  return s.x;
}

float
(graphene_simd4f_get_y) (const graphene_simd4f_t s)
{
  return s.y;
}

float
(graphene_simd4f_get_z) (const graphene_simd4f_t s)
{
  return s.z;
}

float
(graphene_simd4f_get_w) (const graphene_simd4f_t s)
{
  return s.w;
}

graphene_simd4f_t
(graphene_simd4f_splat) (float v)
{
  graphene_simd4f_t s = { v, v, v, v };
  return s;
}

graphene_simd4f_t
(graphene_simd4f_splat_x) (graphene_simd4f_t v)
{
  graphene_simd4f_t s = { v.x, v.x, v.x, v.x };
  return s;
}

graphene_simd4f_t
(graphene_simd4f_splat_y) (graphene_simd4f_t v)
{
  graphene_simd4f_t s = { v.y, v.y, v.y, v.y };
  return s;
}

graphene_simd4f_t
(graphene_simd4f_splat_z) (graphene_simd4f_t v)
{
  graphene_simd4f_t s = { v.z, v.z, v.z, v.z };
  return s;
}

graphene_simd4f_t
(graphene_simd4f_splat_w) (graphene_simd4f_t v)
{
  graphene_simd4f_t s = { v.w, v.w, v.w, v.w };
  return s;
}

graphene_simd4f_t
(graphene_simd4f_reciprocal) (graphene_simd4f_t v)
{
  graphene_simd4f_t s = {
    1.0f / v.x,
    1.0f / v.y,
    1.0f / v.z,
    1.0f / v.w
  };
  return s;
}

graphene_simd4f_t
(graphene_simd4f_sqrt) (graphene_simd4f_t v)
{
  graphene_simd4f_t s = {
    sqrt (v.x),
    sqrt (v.y),
    sqrt (v.z),
    sqrt (v.w)
  };
  return s;
}

graphene_simd4f_t
(graphene_simd4f_rsqrt) (graphene_simd4f_t v)
{
  graphene_simd4f_t s = {
    1.0f / sqrt (v.x),
    1.0f / sqrt (v.y),
    1.0f / sqrt (v.z),
    1.0f / sqrt (v.w)
  };
  return s;
}

graphene_simd4f_t
(graphene_simd4f_add) (const graphene_simd4f_t a,
                       const graphene_simd4f_t b)
{
  graphene_simd4f_t s = {
    a.x + b.x,
    a.y + b.y,
    a.z + b.z,
    a.w + b.w
  };
  return s;
}

graphene_simd4f_t
(graphene_simd4f_sub) (const graphene_simd4f_t a,
                       const graphene_simd4f_t b)
{
  graphene_simd4f_t s = {
    a.x - b.x,
    a.y - b.y,
    a.z - b.z,
    a.w - b.w
  };
  return s;
}

graphene_simd4f_t
(graphene_simd4f_mul) (const graphene_simd4f_t a,
                       const graphene_simd4f_t b)
{
  graphene_simd4f_t s = {
    a.x * b.x,
    a.y * b.y,
    a.z * b.z,
    a.w * b.w
  };
  return s;
}

graphene_simd4f_t
(graphene_simd4f_div) (const graphene_simd4f_t a,
                       const graphene_simd4f_t b)
{
  graphene_simd4f_t s = {
    a.x / b.x,
    a.y / b.y,
    a.z / b.z,
    a.w / b.w
  };
  return s;
}

graphene_simd4f_t
(graphene_simd4f_cross3) (const graphene_simd4f_t a,
                          const graphene_simd4f_t b)
{
  return graphene_simd4f_init (a.y * b.z - a.z * b.y,
                               a.z * b.x - a.x * b.z,
                               a.x * b.y - a.y * b.x,
                               0.f);
}

graphene_simd4f_t
(graphene_simd4f_min) (const graphene_simd4f_t a,
                       const graphene_simd4f_t b)
{
  return graphene_simd4f_init (a.x < b.x ? a.x : b.x,
                               a.y < b.y ? a.y : b.y,
                               a.z < b.z ? a.z : b.z,
                               a.w < b.w ? a.w : b.w);
}

graphene_simd4f_t
(graphene_simd4f_max) (const graphene_simd4f_t a,
                       const graphene_simd4f_t b)
{
  return graphene_simd4f_init (a.x > b.x ? a.x : b.x,
                               a.y > b.y ? a.y : b.y,
                               a.z > b.z ? a.z : b.z,
                               a.w > b.w ? a.w : b.w);
}

graphene_simd4f_t
(graphene_simd4f_shuffle_wxyz) (const graphene_simd4f_t s)
{
  return graphene_simd4f_init (s.w, s.x, s.y, s.z);
}

graphene_simd4f_t
(graphene_simd4f_shuffle_zwxy) (const graphene_simd4f_t s)
{
  return graphene_simd4f_init (s.z, s.w, s.x, s.y);
}

graphene_simd4f_t
(graphene_simd4f_shuffle_yzwx) (const graphene_simd4f_t s)
{
  return graphene_simd4f_init (s.y, s.z, s.w, s.x);
}

graphene_simd4f_t
(graphene_simd4f_zero_w) (const graphene_simd4f_t s)
{
  return graphene_simd4f_init (s.x, s.y, s.z, 0.0f); 
}

graphene_simd4f_t
(graphene_simd4f_zero_zw) (const graphene_simd4f_t s)
{
  return graphene_simd4f_init (s.x, s.y, 0.0f, 0.0f);
}

graphene_simd4f_t
(graphene_simd4f_merge_high) (const graphene_simd4f_t a,
                              const graphene_simd4f_t b)
{
  return graphene_simd4f_init (a.z, a.w, b.z, b.w);
}

graphene_simd4f_t
(graphene_simd4f_flip_sign_0101) (const graphene_simd4f_t s)
{
  return graphene_simd4f_init (s.x, -s.y, s.z, -s.w);
}

graphene_simd4f_t
(graphene_simd4f_flip_sign_1010) (const graphene_simd4f_t s)
{
  return graphene_simd4f_init (-s.x, s.y, -s.z, s.w);
}

gboolean
(graphene_simd4f_cmp_eq) (const graphene_simd4f_t a,
                          const graphene_simd4f_t b)
{
  return a.x == b.x &&
         a.y == b.y &&
         a.z == b.z &&
         a.w == b.w;
}

gboolean
(graphene_simd4f_cmp_neq) (const graphene_simd4f_t a,
                           const graphene_simd4f_t b)
{
  return a.x != b.x &&
         a.y != b.y &&
         a.z != b.z &&
         a.w != b.w;
}

#endif /* GRAPHENE_USE_SCALAR */
