/* graphene-simd4f-scala.h: Scalar implementation of simd4f
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

#ifndef __GRAPHENE_SIMD4F_SCALAR_H__
#define __GRAPHENE_SIMD4F_SCALAR_H__

#include <math.h>
#include <string.h>

/* don't want to include glib.h just for G_BEGIN_DECLS/G_END_DECLS */
#ifdef __cplusplus
extern "C" {
#endif

#define GRAPHENE_SIMD4F_GET(f) \
static inline float \
graphene_simd4f_get_##f (const graphene_simd4f_t s) \
{ \
  return s.f; \
}

GRAPHENE_SIMD4F_GET (x)
GRAPHENE_SIMD4F_GET (y)
GRAPHENE_SIMD4F_GET (z)
GRAPHENE_SIMD4F_GET (w)

#undef GRAPHENE_SIMD4F_GET

static inline graphene_simd4f_t
graphene_simd4f_init (float x,
                      float y,
                      float z,
                      float w)
{
  graphene_simd4f_t s = { x, y, z, w };
  return s;
}

static inline graphene_simd4f_t
graphene_simd4f_init_zero (void)
{
  graphene_simd4f_t s = graphene_simd4f_init (0.f, 0.f, 0.f, 0.f);
  return s;
}

static inline graphene_simd4f_t
graphene_simd4f_init_4f (const float *v)
{
  graphene_simd4f_t s = graphene_simd4f_init (v[0], v[1], v[2], v[3]);
  return s;
}

static inline graphene_simd4f_t
graphene_simd4f_init_3f (const float *v)
{
  graphene_simd4f_t s = graphene_simd4f_init (v[0], v[1], v[2], 0.f);
  return s;
}

static inline graphene_simd4f_t
graphene_simd4f_init_2f (const float *v)
{
  graphene_simd4f_t s = graphene_simd4f_init (v[0], v[1], 0.f, 0.f);
  return s;
}

static inline void
graphene_simd4f_dup_4f (const graphene_simd4f_t  s,
                        float                   *v)
{
  memcpy (v, &s, sizeof (float) * 4);
}

static inline void
graphene_simd4f_dup_3f (const graphene_simd4f_t  s,
                        float                   *v)
{
  memcpy (v, &s, sizeof (float) * 3);
}

static inline void
graphene_simd4f_dup_2f (const graphene_simd4f_t  s,
                        float                   *v)
{
  memcpy (v, &s, sizeof (float) * 2);
}

static inline graphene_simd4f_t
graphene_simd4f_splat (float v)
{
  graphene_simd4f_t s = { v, v, v, v };
  return s;
}

static inline graphene_simd4f_t
graphene_simd4f_splat_x (graphene_simd4f_t v)
{
  graphene_simd4f_t s = { v.x, v.x, v.x, v.x };
  return s;
}

static inline graphene_simd4f_t
graphene_simd4f_splat_y (graphene_simd4f_t v)
{
  graphene_simd4f_t s = { v.y, v.y, v.y, v.y };
  return s;
}

static inline graphene_simd4f_t
graphene_simd4f_splat_z (graphene_simd4f_t v)
{
  graphene_simd4f_t s = { v.z, v.z, v.z, v.z };
  return s;
}

static inline graphene_simd4f_t
graphene_simd4f_splat_w (graphene_simd4f_t v)
{
  graphene_simd4f_t s = { v.w, v.w, v.w, v.w };
  return s;
}

static inline graphene_simd4f_t
graphene_simd4f_reciprocal (graphene_simd4f_t v)
{
  graphene_simd4f_t s = {
    1.0f / v.x,
    1.0f / v.y,
    1.0f / v.z,
    1.0f / v.w
  };
  return s;
}

static inline graphene_simd4f_t
graphene_simd4f_sqrt (graphene_simd4f_t v)
{
  graphene_simd4f_t s = {
    sqrt (v.x),
    sqrt (v.y),
    sqrt (v.z),
    sqrt (v.w)
  };
  return s;
}

static inline graphene_simd4f_t
graphene_simd4f_rsqrt (graphene_simd4f_t v)
{
  graphene_simd4f_t s = {
    1.0f / sqrt (v.x),
    1.0f / sqrt (v.y),
    1.0f / sqrt (v.z),
    1.0f / sqrt (v.w)
  };
  return s;
}

#define GRAPHENE_SIMD4F_OP(name,op) \
static inline graphene_simd4f_t \
graphene_simd4f_##name (graphene_simd4f_t a, graphene_simd4f_t b) \
{ \
  graphene_simd4f_t s = { \
    a.x op b.x, \
    a.y op b.y, \
    a.z op b.z, \
    a.w op b.w  \
  }; \
  return s; \
}

GRAPHENE_SIMD4F_OP (add, +)
GRAPHENE_SIMD4F_OP (sub, -)
GRAPHENE_SIMD4F_OP (mul, *)
GRAPHENE_SIMD4F_OP (div, /)

#undef GRAPHENE_SIMD4F_OP

static inline graphene_simd4f_t
graphene_simd4f_cross3 (graphene_simd4f_t a,
                        graphene_simd4f_t b)
{
  return graphene_simd4f_init (a.y * b.z - a.z * b.y,
                               a.z * b.x - a.x * b.z,
                               a.x * b.y - a.y * b.x,
                               0.f);
}

static inline graphene_simd4f_t
graphene_simd4f_min (graphene_simd4f_t a,
                     graphene_simd4f_t b)
{
  return graphene_simd4f_init (a.x < b.x ? a.x : b.x,
                               a.y < b.y ? a.y : b.y,
                               a.z < b.z ? a.z : b.z,
                               a.w < b.w ? a.w : b.w);
}

static inline graphene_simd4f_t
graphene_simd4f_max (graphene_simd4f_t a,
                     graphene_simd4f_t b)
{
  return graphene_simd4f_init (a.x > b.x ? a.x : b.x,
                               a.y > b.y ? a.y : b.y,
                               a.z > b.z ? a.z : b.z,
                               a.w > b.w ? a.w : b.w);
}

#define GRAPHENE_SIMD4F_SHUFFLE(a,b,c,d) \
static inline graphene_simd4f_t \
graphene_simd4f_shuffle_##a##b##c##d (graphene_simd4f_t s) \
{ \
  return graphene_simd4f_init (s.a, s.b, s.c, s.d); \
}

GRAPHENE_SIMD4F_SHUFFLE (w, x, y, z)
GRAPHENE_SIMD4F_SHUFFLE (z, w, x, y)
GRAPHENE_SIMD4F_SHUFFLE (y, z, w, x)

#undef GRAPHENE_SIMD4F_SHUFFLE

static inline graphene_simd4f_t
graphene_simd4f_zero_w (graphene_simd4f_t s)
{
  return graphene_simd4f_init (s.x, s.y, s.z, 0.0f); 
}

static inline graphene_simd4f_t
graphene_simd4f_zero_zw (graphene_simd4f_t s)
{
  return graphene_simd4f_init (s.x, s.y, 0.0f, 0.0f);
}

static inline graphene_simd4f_t
graphene_simd4f_merge_high (graphene_simd4f_t a,
                            graphene_simd4f_t b)
{
  return graphene_simd4f_init (a.z, a.w, b.z, b.w);
}

static inline graphene_simd4f_t
graphene_simd4f_flip_sign_0101 (graphene_simd4f_t s)
{
  return graphene_simd4f_init (s.x, -s.y, s.z, -s.w);
}

static inline graphene_simd4f_t
graphene_simd4f_flip_sign_1010 (graphene_simd4f_t s)
{
  return graphene_simd4f_init (-s.x, s.y, -s.z, s.w);
}

static inline gboolean
graphene_simd4f_cmp_eq (graphene_simd4f_t a,
                        graphene_simd4f_t b)
{
  return a.x == b.x &&
         a.y == b.y &&
         a.z == b.z &&
         a.w == b.w;
}

static inline gboolean
graphene_simd4f_cmp_neq (graphene_simd4f_t a,
                         graphene_simd4f_t b)
{
  return a.x != b.x &&
         a.y != b.y &&
         a.z != b.z &&
         a.w != b.w;
}

#ifdef __cplusplus
}
#endif

#endif
