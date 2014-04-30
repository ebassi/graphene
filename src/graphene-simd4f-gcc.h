/* graphene-simd4f-gcc.h: GCC implementation of simd4f
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

#ifndef __GRAPHENE_SIMD4F_GCC_H__
#define __GRAPHENE_SIMD4F_GCC_H__

#include <math.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef union {
  graphene_simd4f_t s;
  float f[4];
} graphene_simd4f_union_t;

#define GRAPHENE_SIMD4F_GET(field,i) \
static inline float \
graphene_simd4f_get_##field (graphene_simd4f_t s) \
{ \
  graphene_simd4f_union_t u = { s }; \
  return u.f[i]; \
}

GRAPHENE_SIMD4F_GET (x, 0)
GRAPHENE_SIMD4F_GET (y, 1)
GRAPHENE_SIMD4F_GET (z, 2)
GRAPHENE_SIMD4F_GET (w, 3)

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
  graphene_simd4f_t s = graphene_simd4f_init (v, v, v, v);
  return s;
}

static inline graphene_simd4f_t
graphene_simd4f_splat_x (graphene_simd4f_t v)
{
  float val = graphene_simd4f_get_x (v);
  graphene_simd4f_t s = graphene_simd4f_splat (val);
  return s;
}

static inline graphene_simd4f_t
graphene_simd4f_splat_y (graphene_simd4f_t v)
{
  float val = graphene_simd4f_get_y (v);
  graphene_simd4f_t s = graphene_simd4f_splat (val);
  return s;
}

static inline graphene_simd4f_t
graphene_simd4f_splat_z (graphene_simd4f_t v)
{
  float val = graphene_simd4f_get_z (v);
  graphene_simd4f_t s = graphene_simd4f_splat (val);
  return s;
}

static inline graphene_simd4f_t
graphene_simd4f_splat_w (graphene_simd4f_t v)
{
  float val = graphene_simd4f_get_w (v);
  graphene_simd4f_t s = graphene_simd4f_splat (val);
  return s;
}

static inline graphene_simd4f_t
graphene_simd4f_reciprocal (graphene_simd4f_t v)
{
  return graphene_simd4f_splat (1.0f) / v;
}

static inline graphene_simd4f_t
graphene_simd4f_sqrt (graphene_simd4f_t v)
{
  graphene_simd4f_t ret = {
    sqrt (graphene_simd4f_get_x (v)),
    sqrt (graphene_simd4f_get_y (v)),
    sqrt (graphene_simd4f_get_z (v)),
    sqrt (graphene_simd4f_get_w (v))
  };
  return ret;
}

static inline graphene_simd4f_t
graphene_simd4f_rsqrt (graphene_simd4f_t v)
{
  return graphene_simd4f_splat (1.0f) / graphene_simd4f_sqrt (v);
}

#define GRAPHENE_SIMD4F_OP(name,op) \
static inline graphene_simd4f_t \
graphene_simd4f_##name (graphene_simd4f_t a, graphene_simd4f_t b) \
{ \
  graphene_simd4f_t res = a op b; \
  return res; \
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
  graphene_simd4f_union_t ua = { a };
  graphene_simd4f_union_t ub = { b };

  return graphene_simd4f_init (ua.f[1] * ub.f[2] - ua.f[2] * ub.f[1],
                               ua.f[2] * ub.f[0] - ua.f[0] * ub.f[2],
                               ua.f[0] * ub.f[1] - ua.f[1] * ub.f[0],
                               0.f);
}

static inline graphene_simd4f_t
graphene_simd4f_min (graphene_simd4f_t a,
                     graphene_simd4f_t b)
{
  graphene_simd4f_union_t ua = { a };
  graphene_simd4f_union_t ub = { b };

  return graphene_simd4f_init (ua.f[0] < ub.f[0] ? ua.f[0] : ub.f[0],
                               ua.f[1] < ub.f[1] ? ua.f[1] : ub.f[1],
                               ua.f[2] < ub.f[2] ? ua.f[2] : ub.f[2],
                               ua.f[3] < ub.f[3] ? ua.f[3] : ub.f[3]);
}

static inline graphene_simd4f_t
graphene_simd4f_max (graphene_simd4f_t a,
                     graphene_simd4f_t b)
{
  graphene_simd4f_union_t ua = { a };
  graphene_simd4f_union_t ub = { b };

  return graphene_simd4f_init (ua.f[0] > ub.f[0] ? ua.f[0] : ub.f[0],
                               ua.f[1] > ub.f[1] ? ua.f[1] : ub.f[1],
                               ua.f[2] > ub.f[2] ? ua.f[2] : ub.f[2],
                               ua.f[3] > ub.f[3] ? ua.f[3] : ub.f[3]);
}

static inline graphene_simd4f_t
graphene_simd4f_shuffle_wxyz (graphene_simd4f_t v)
{
  graphene_simd4f_union_t u = { v };
  return graphene_simd4f_init (u.f[3], u.f[0], u.f[1], u.f[2]);
}

static inline graphene_simd4f_t
graphene_simd4f_shuffle_zwxy (graphene_simd4f_t v)
{
  graphene_simd4f_union_t u = { v };
  return graphene_simd4f_init (u.f[2], u.f[3], u.f[0], u.f[1]);
}

static inline graphene_simd4f_t
graphene_simd4f_shuffle_yzwx (graphene_simd4f_t v)
{
  graphene_simd4f_union_t u = { v };
  return graphene_simd4f_init (u.f[1], u.f[2], u.f[3], u.f[0]);
}

static inline graphene_simd4f_t
graphene_simd4f_zero_w (graphene_simd4f_t v)
{
  graphene_simd4f_union_t u = { v };
  return graphene_simd4f_init (u.f[0], u.f[1], u.f[2], 0.f);
}

static inline graphene_simd4f_t
graphene_simd4f_zero_zw (graphene_simd4f_t v)
{
  graphene_simd4f_union_t u = { v };
  return graphene_simd4f_init (u.f[0], u.f[1], 0.f, 0.f);
}

static inline graphene_simd4f_t
graphene_simd4f_merge_high (graphene_simd4f_t a,
                            graphene_simd4f_t b)
{
  graphene_simd4f_union_t u_a = { a };
  graphene_simd4f_union_t u_b = { b };
  return graphene_simd4f_init (u_a.f[2], u_a.f[3], u_b.f[2], u_b.f[3]);
}

static inline graphene_simd4f_t
graphene_simd4f_flip_sign_0101 (graphene_simd4f_t v)
{
  graphene_simd4f_union_t u = { v };
  return graphene_simd4f_init (u.f[0], -u.f[1], u.f[2], -u.f[3]);
}

static inline graphene_simd4f_t
graphene_simd4f_flip_sign_1010 (graphene_simd4f_t v)
{
  graphene_simd4f_union_t u = { v };
  return graphene_simd4f_init (-u.f[0], u.f[1], -u.f[2], u.f[3]);
}

static inline gboolean
graphene_simd4f_cmp_eq (graphene_simd4f_t a)
{
  graphene_simd4f_union_t u_a = { a };
  graphene_simd4f_union_t u_b = { b };

  return u_a.f[0] == u_b.f[0] &&
         u_a.f[1] == u_b.f[1] &&
         u_a.f[2] == u_b.f[2] &&
         u_a.f[3] == u_b.f[3];
}

static inline gboolean
graphene_simd4f_cmp_neq (graphene_simd4f_t a)
{
  graphene_simd4f_union_t u_a = { a };
  graphene_simd4f_union_t u_b = { b };

  return u_a.f[0] != u_b.f[0] &&
         u_a.f[1] != u_b.f[1] &&
         u_a.f[2] != u_b.f[2] &&
         u_a.f[3] != u_b.f[3];
}

#ifdef __cplusplus
}
#endif

#endif /* __GRAPHENE_SIMD4F_GCC_H__ */
