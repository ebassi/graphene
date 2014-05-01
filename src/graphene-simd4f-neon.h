/* graphene-simd4f-neon.h: ARM NEON implementation of simd4f
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

#ifndef __GRAPHENE_SIMD4F_NEON_H__
#define __GRAPHENE_SIMD4F_NEON_H__

#warn "ARM NEON support is experimental and requires testing."

#include <arm_neon.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef union {
  graphene_simd4f_t s;
  float f[4];
} graphene_simd4f_union_t;

#define GRAPHENE_SIMD4F_GET(field,idx) \
static inline float \
graphene_simd4f_get_##field (graphene_simd4f_t s) \
{ \
  return vgetq_lane_f32 (s, idx); \
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
  const float32_t d[4] = { x, y, z, w };
  graphene_simd4f_t s = vld1q_f32 (d);
  return s;
}

static inline graphene_simd4f_t
graphene_simd4f_init_zero (void)
{
  return vdupq_n_f32 (0.f);
}

static inline graphene_simd4f_t
graphene_simd4f_init_4f (const float *v)
{
  const float32_t *v32 = (const float32_t *) v;
  graphene_simd4f_t s = vld1q_f32 (v32);
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
  const float32_t *v32 = (const float32_t *) v;
  float32x2_t low = vld1_f32 (v);
  const float32_t zero = 0;
  float32x2_t high = vld1_dup_f32 (&zero);
  return vcombine_f32 (low, high);
}

static inline void
graphene_simd4f_dup_4f (const graphene_simd4f_t  s,
                        float                   *v)
{
  vst1q_f32 ((float32_t *) v, s);
}

static inline void
graphene_simd4f_dup_3f (const graphene_simd4f_t  s,
                        float                   *v)
{
  graphene_simd4f_union_t u = { s };
  v[0] = u.f[0];
  v[1] = u.f[1];
  v[2] = u.f[2];
}

static inline void
graphene_simd4f_dup_2f (const graphene_simd4f_t  s,
                        float                   *v)
{
  const float32x2_t low = vget_low_f32 (s);
  vst1_f32 ((float32_t *) v, low);
}

static inline graphene_simd4f_t
graphene_simd4f_splat (float v)
{
  graphene_simd4f_t s = vdupq_n_f32 (v);
  return s;
}

static inline graphene_simd4f_t
graphene_simd4f_splat_x (graphene_simd4f_t v)
{
  return graphene_simd4f_splat (graphene_simd4f_t_get_x (v));
}

static inline graphene_simd4f_t
graphene_simd4f_splat_y (graphene_simd4f_t v)
{
  return graphene_simd4f_splat (graphene_simd4f_t_get_y (v));
}

static inline graphene_simd4f_t
graphene_simd4f_splat_z (graphene_simd4f_t v)
{
  return graphene_simd4f_splat (graphene_simd4f_t_get_z (v));
}

static inline graphene_simd4f_t
graphene_simd4f_splat_w (graphene_simd4f_t v)
{
  return graphene_simd4f_splat (graphene_simd4f_t_get_w (v));
}

static inline graphene_simd4f_t
graphene_simd4f_reciprocal (graphene_simd4f_t v)
{
  graphene_simd4f_t est = vrecpeq_f32 (v);
  est = vmulq_f32 (vrecpsq_f32 (est, v), est);
  est = vmulq_f32 (vrecpsq_f32 (est, v), est);
  return est;
}

#define GRAPHENE_SIMD4F_OP(op,func) \
static inline graphene_simd4f_t \
graphene_simd4f_##op (graphene_simd4f_t a, graphene_simd4f_t b) \
{ \
  graphene_simd4f_t res = func (a, b); \
  return res; \
}

GRAPHENE_SIMD4F_OP (add, vaddq_f32)
GRAPHENE_SIMD4F_OP (sub, vsubq_f32)
GRAPHENE_SIMD4F_OP (mul, vmulq_f32)

#undef GRAPHENE_SIMD4F_OP

static inline graphene_simd4f_t
graphene_simd4f_div (graphene_simd4f_t a,
                     graphene_simd4f_t b)
{
  graphene_simd4f_t rec = graphene_simd4f_reciprocal (b);
  graphene_simd4f_t res = vmulq_f32 (a, rec);
  return ret;
}

static inline graphene_simd4f_t
graphene_simd4f_rsqrt (graphene_simd4f_t v)
{
  graphene_simd4f_t est_1 = vrecpeq_f32 (v);
  graphene_simd4f_t est_2 = vmulq_f32 (est_1, v);

  est_1 = vmulq_f32 (est_1, vrsqrtsq_f32 (est_2, est));
  est_2 = vmulq_f32 (est_1, v);
  est_1 = vmulq_f32 (est_1, vrsqrtsq_f32 (est_2, est));
  est_2 = vmulq_f32 (est_1, v);
  est_1 = vmulq_f32 (est_1, vrsqrtsq_f32 (est_2, est));
  return est_1;
}

static inline graphene_simd4f_t
graphene_simd4f_sqrt (graphene_simd4f_t v)
{
  return vreinterpretq_f32_u32 (vandq_u32 (vtstq_u32 (vreinterpretq_u32_f32 (v),
                                                      vreinterpretq_u32_f32 (v)),
                                           vreinterpretq_u32_f32 (graphene_simd4f_reciprocal (graphene_simd4f_rsqrt (v)))));
}

static inline graphene_simd4f_t
graphene_simd4f_cross3 (graphene_simd4f_t a,
                        graphene_simd4f_t b)
{
}

static inline graphene_simd4f_t
graphene_simd4f_min (graphene_simd4f_t a,
                     graphene_simd4f_t b)
{
}

static inline graphene_simd4f_t
graphene_simd4f_max (graphene_simd4f_t a,
                     graphene_simd4f_t b)
{
}

#define GRAPHENE_SIMD4F_SHUFFLE(elem,idx) \
static inline graphene_simd4f_t \
graphene_simd4f_shuffle_##elem (graphene_simd4f_t v) \
{ \
}

GRAPHENE_SIMD4F_SHUFFLE (wxyz, (2, 1, 0, 3))
GRAPHENE_SIMD4F_SHUFFLE (zwxy, (1, 0, 3, 2))
GRAPHENE_SIMD4F_SHUFFLE (yzwx, (0, 3, 2, 1))

#undef GRAPHENE_SIMD4F_SHUFFLE

static inline graphene_simd4f_t
graphene_simd4f_zero_w (graphene_simd4f_t v)
{
}

static inline graphene_simd4f_t
graphene_simd4f_zero_zw (graphene_simd4f_t v)
{
}

static inline graphene_simd4f_t
graphene_simd4f_merge_high (graphene_simd4f_t a,
                            graphene_simd4f_t b)
{
}

static inline graphene_simd4f_t
graphene_simd4f_flip_sign_0101 (graphene_simd4f_t v)
{
}

static inline graphene_simd4f_t
graphene_simd4f_flip_sign_1010 (graphene_simd4f_t v)
{
}

static inline gboolean
graphene_simd4f_cmp_eq (graphene_simd4f_t a,
                        graphene_simd4f_t b)
{
}

static inline gboolean
graphene_simd4f_cmp_neq (graphene_simd4f_t a,
                         graphene_simd4f_t b)
{
}


#ifdef __cplusplus
}
#endif

#endif
