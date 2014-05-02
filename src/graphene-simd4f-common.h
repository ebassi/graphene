/* graphene-simd4f-common.h: Four float vector operations, common section
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

#ifndef __GRAPHENE_SIMD4F_COMMON_H__
#define __GRAPHENE_SIMD4F_COMMON_H__

#ifdef __cplusplus
extern "C" {
#endif

static inline graphene_simd4f_t
graphene_simd4f_madd (graphene_simd4f_t m1,
                      graphene_simd4f_t m2,
                      graphene_simd4f_t a)
{
  return graphene_simd4f_add (graphene_simd4f_mul (m1, m2), a);
}

static inline graphene_simd4f_t
graphene_simd4f_sum (graphene_simd4f_t v)
{
  const graphene_simd4f_t s0 = graphene_simd4f_splat_x (v);
  const graphene_simd4f_t s1 = graphene_simd4f_add (s0, graphene_simd4f_splat_y (v));
  const graphene_simd4f_t s2 = graphene_simd4f_add (s1, graphene_simd4f_splat_z (v));
  const graphene_simd4f_t s3 = graphene_simd4f_add (s2, graphene_simd4f_splat_w (v));
  return s3;
}

static inline graphene_simd4f_t
graphene_simd4f_dot4 (graphene_simd4f_t a,
                      graphene_simd4f_t b)
{
  return graphene_simd4f_sum (graphene_simd4f_mul (a, b));
}

static inline graphene_simd4f_t
graphene_simd4f_dot3 (graphene_simd4f_t a,
                      graphene_simd4f_t b)
{
  const graphene_simd4f_t m = graphene_simd4f_mul (a, b);
  const graphene_simd4f_t s1 = graphene_simd4f_add (graphene_simd4f_splat_x (m),
                                                    graphene_simd4f_splat_y (m));
  const graphene_simd4f_t s2 = graphene_simd4f_add (s1,
                                                    graphene_simd4f_splat_z (m));
  return s2;
}

static inline graphene_simd4f_t
graphene_simd4f_dot2 (graphene_simd4f_t a,
                      graphene_simd4f_t b)
{
  const graphene_simd4f_t m = graphene_simd4f_mul (a, b);
  const graphene_simd4f_t s1 = graphene_simd4f_add (graphene_simd4f_splat_x (m),
                                                    graphene_simd4f_splat_y (m));
  return s1;
}

static inline graphene_simd4f_t
graphene_simd4f_length4 (graphene_simd4f_t v)
{
  return graphene_simd4f_sqrt (graphene_simd4f_dot4 (v, v));
}

static inline graphene_simd4f_t
graphene_simd4f_length3 (graphene_simd4f_t v)
{
  return graphene_simd4f_sqrt (graphene_simd4f_dot3 (v, v));
}

static inline graphene_simd4f_t
graphene_simd4f_length2 (graphene_simd4f_t v)
{
  return graphene_simd4f_sqrt (graphene_simd4f_dot2 (v, v));
}

static inline graphene_simd4f_t
graphene_simd4f_normalize4 (graphene_simd4f_t v)
{
  graphene_simd4f_t invlen = graphene_simd4f_rsqrt (graphene_simd4f_dot4 (v, v));
  return graphene_simd4f_mul (v, invlen);
}

static inline graphene_simd4f_t
graphene_simd4f_normalize3 (graphene_simd4f_t v)
{
  graphene_simd4f_t invlen = graphene_simd4f_rsqrt (graphene_simd4f_dot3 (v, v));
  return graphene_simd4f_mul (v, invlen);
}

static inline graphene_simd4f_t
graphene_simd4f_normalize2 (graphene_simd4f_t v)
{
  graphene_simd4f_t invlen = graphene_simd4f_rsqrt (graphene_simd4f_dot2 (v, v));
  return graphene_simd4f_mul (v, invlen);
}

static inline gboolean
graphene_simd4f_is_zero4 (graphene_simd4f_t v)
{
  graphene_simd4f_t zero = graphene_simd4f_init_zero ();
  return graphene_simd4f_cmp_eq (v, zero);
}

static inline gboolean
graphene_simd4f_is_zero3 (graphene_simd4f_t v)
{
  return graphene_simd4f_get_x (v) == 0.f &&
         graphene_simd4f_get_y (v) == 0.f &&
         graphene_simd4f_get_z (v) == 0.f;
}

static inline gboolean
graphene_simd4f_is_zero2 (graphene_simd4f_t v)
{
  return graphene_simd4f_get_x (v) == 0.f &&
         graphene_simd4f_get_y (v) == 0.f;
}

static inline graphene_simd4f_t
graphene_simd4f_interpolate (graphene_simd4f_t a,
                             graphene_simd4f_t b,
                             double            f)
{
  return graphene_simd4f_add (a,
                              graphene_simd4f_mul (graphene_simd4f_sub (b, a),
                                                   graphene_simd4f_splat (f)));
}

#ifdef __cplusplus
}
#endif

#endif /* __GRAPHENE_SIMD4F_COMMON_H__ */
