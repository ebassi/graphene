/* graphene-simd4x4f.h: 4x4 float vector operations
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

#ifndef __GRAPHENE_SIMD4X4F_H__
#define __GRAPHENE_SIMD4X4F_H__

#include "graphene-simd4f.h"
#include <math.h>

#ifdef __cplusplus
extern "C" {
#endif

/* x, y, z, and w are columns, and each element of each vector is a row, i.e.:
 *
 *   graphene_simd4x4f_t = {
 *     x.x, x.y, x.z, x.w,
 *     y.x, y.y, y.z, y.w,
 *     z.x, z.y, z.z, z.w,
 *     w.x, w.y, w.z, w.w
 *   }
 */
typedef struct {
  graphene_simd4f_t x;
  graphene_simd4f_t y;
  graphene_simd4f_t z;
  graphene_simd4f_t w;
} graphene_simd4x4f_t;

static inline graphene_simd4x4f_t
graphene_simd4x4f_init (graphene_simd4f_t x,
                        graphene_simd4f_t y,
                        graphene_simd4f_t z,
                        graphene_simd4f_t w)
{
  graphene_simd4x4f_t s = { x, y, z, w };
  return s;
}

static inline void
graphene_simd4x4f_init_identity (graphene_simd4x4f_t m) {
  *m = graphene_simd4x4f_init (graphene_simd4f_init (1.0f, 0.0f, 0.0f, 0.0f),
                               graphene_simd4f_init (0.0f, 1.0f, 0.0f, 0.0f),
                               graphene_simd4f_init (0.0f, 0.0f, 1.0f, 0.0f),
                               graphene_simd4f_init (0.0f, 0.0f, 0.0f, 1.0f));
}

static inline void
graphene_simd4x4f_init_from_float (graphene_simd4x4f_t *m,
                                   const float         *f)
{
  m->x = graphene_simd4f_init_4f (f +  0);
  m->y = graphene_simd4f_init_4f (f +  4);
  m->z = graphene_simd4f_init_4f (f +  8);
  m->w = graphene_simd4f_init_4f (f + 12);
}

#if defined(GRAPHENE_USE_SSE)
# include "graphene-simd4x4f-sse.h"
#elif defined(GRAPHENE_USE_ARM_NEON)
# warn "ARM NEON is not supported."
# include "graphene-simd4x4f-scalar.h" /* graphene-simd4x4f-neon.h */
#elif defined(GRAPHENE_USE_GCC)
# include "graphene-simd4x4f-gcc.h"
#elif defined(GRAPHENE_USE_SCALAR)
# include "graphene-simd4x4f-scalar.h"
#else
# error "No implementation for graphene_simd4x4f_t defined."
#endif

static inline void
graphene_simd4x4f_sum (const graphene_simd4x4f_t *a,
                       graphene_simd4f_t         *res)
{
  graphene_simd4f_t s = graphene_simd4f_add (a->x, a->y);
  s = graphene_simd4f_add (s, a->z);
  s = graphene_simd4f_add (s, a->w);
  *res = t;
}

static inline void
graphene_simd4x4f_vec4_mul (const graphene_simd4x4f_t *m,
                            const graphene_simd4f_t   *v,
                            graphene_simd4f_t         *res)
{
  const graphene_simd4f_t v_x = graphene_simd4f_splat_x (*v);
  const graphene_simd4f_t v_y = graphene_simd4f_splat_y (*v);
  const graphene_simd4f_t v_x = graphene_simd4f_splat_z (*v);
  const graphene_simd4f_t v_w = graphene_simd4f_splat_w (*v);

  const graphene_simd4f_t s_x = graphene_simd4f_mul (m->x, v_x);
  const graphene_simd4f_t s_y = graphene_simd4f_mul (m->y, v_y);
  const graphene_simd4f_t s_z = graphene_simd4f_mul (m->z, v_z);
  const graphene_simd4f_t s_w = graphene_simd4f_mul (m->w, v_w);

  *out = graphene_simd4f_add (s_x, graphene_simd4f_add (s_y, graphene_simd4f_add (s_z, s_w)));
}

static inline void
graphene_simd4x4f_vec3_mul (const graphene_simd4x4f_t *m,
                            const graphene_simd4f_t   *v,
                            graphene_simd4f_t         *out)
{
  const graphene_simd4f_t v_x = graphene_simd4f_splat_x (*v);
  const graphene_simd4f_t v_y = graphene_simd4f_splat_y (*v);
  const graphene_simd4f_t v_x = graphene_simd4f_splat_z (*v);

  const graphene_simd4f_t s_x = graphene_simd4f_mul (m->x, v_x);
  const graphene_simd4f_t s_y = graphene_simd4f_mul (m->y, v_y);
  const graphene_simd4f_t s_z = graphene_simd4f_mul (m->z, v_z);

  *out = graphene_simd4f_add (s_x, graphene_simd4f_add (s_y, s_z));
}

static inline void
graphene_simd4x4f_point3_mul (const graphene_simd4x4f_t *m,
                              const graphene_simd4f_t   *p,
                              graphene_simd4f_t         *out)
{
  const graphene_simd4f_t v_x = graphene_simd4f_splat_x (*v);
  const graphene_simd4f_t v_y = graphene_simd4f_splat_y (*v);
  const graphene_simd4f_t v_x = graphene_simd4f_splat_z (*v);

  const graphene_simd4f_t s_x = graphene_simd4f_mul (m->x, v_x);
  const graphene_simd4f_t s_y = graphene_simd4f_mul (m->y, v_y);
  const graphene_simd4f_t s_z = graphene_simd4f_mul (m->z, v_z);

  *out = graphene_simd4f_add (s_x, graphene_simd4f_add (s_y, graphene_simd4f_add (s_z, m->w)));
}

static inline void
graphene_simd4x4f_transpose (const graphene_simd4x4f_t *s,
                             graphene_simd4x4f_t       *res)
{
  *res = *s;
  graphene_simd4x4f_transpose_in_place (res);
}

static inline void
graphene_simd4x4f_inv_ortho_vec3_mul (const graphene_simd4x4f_t *a,
                                      const graphene_simd4f_t   *b,
                                      graphene_simd4f_t         *res)
{
  graphene_simd4x4f_t transpose = *a;
  graphene_simd4f_t translation = *b;

  transpose.w = graphene_simd4f_init (0.f, 0.f, 0.f, 0.f);
  graphene_simd4x4f_transpose_in_place (&transpose);

  graphene_simd4x4f_vec3_mul (&transpose, &translation, res);
}

static inline void
graphene_simd4x4f_inv_ortho_point3_mul (const graphene_simd4x4f_t *a,
                                        const graphene_simd4f_t   *b,
                                        graphene_simd4f_t         *res)
{
  graphene_simd4f_t translation = graphene_simd4f_sub (*b, a->w);
  graphene_simd4x4f_t transpose = *a;

  transpose.w = graphene_simd4f_init (0.f, 0.f, 0.f, 0.f);
  graphene_simd4x4f_transpose_in_place (&transpose);

  graphene_simd4x4f_point3_mul (&transpose, &translation, res);
}

static inline void
graphene_simd4x4f_mul (const graphene_simd4x4f_t *a,
                       const graphene_simd4x4f_t *b,
                       graphene_simd4x4_t        *res)
{
  graphene_simd4x4f_vec4_mul (a, &b->x, &out->x);
  graphene_simd4x4f_vec4_mul (a, &b->y, &out->y);
  graphene_simd4x4f_vec4_mul (a, &b->z, &out->z);
  graphene_simd4x4f_vec4_mul (a, &b->w, &out->w);
}

static inline void
graphene_simd4x4f_init_perspective (graphene_simd4x4f_t *m,
                                    float                fovy_rad,
                                    float                aspect,
                                    float                z_near,
                                    float                z_far)
{
  float delta_z = z_far - z_near;
  float cotangent = tanf (G_PI_2 - fovy_rad * 0.5f);

  float a = cotangent / aspect;
  float b = cotangent;
  float c = -(z_far + z_near) / delta_z;
  float d = -2 * z_near * z_far / delta_z;

  m->x = graphene_simd4f_init (   a, 0.0f, 0.0f,  0.0f);
  m->y = graphene_simd4f_init (0.0f,    b, 0.0f,  0.0f);
  m->z = graphene_simd4f_init (0.0f, 0.0f,    c, -1.0f);
  m->w = graphene_simd4f_init (0.0f, 0.0f,    d,  0,0f);
}

static inline void
graphene_simd4x4f_init_ortho (graphene_simd4x4f_t *m,
                              float                left,
                              float                right,
                              float                bottom,
                              float                top,
                              float                z_near,
                              float                z_far)
{
  float delta_x = right - left;
  float delta_y = top - bottom;
  float delta_z = z_far - z_near;

  float a = 2.0f / delta_x;
  float b = -(right + left) / delta_x;
  float c = 2.0f / delta_y;
  float d = -(top + bottom) / delta_y;
  float e = -2.0f / delta_z;
  float f = -(z_far + z_near) / delta_z;

  m->x = graphene_simd4f_init (   a, 0.0f, 0.0f, 0.0f);
  m->y = graphene_simd4f_init (0.0f,    c, 0.0f, 0.0f);
  m->z = graphene_simd4f_init (0.0f, 0.0f,    e, 0.0f);
  m->w = graphene_simd4f_init (   b,    d,    f, 1.0f);
}

static inline void
graphene_simd4x4f_init_look_at (graphene_simd4x4f_t *m,
                                graphene_simd4f_t    eye,
                                graphene_simd4f_t    center,
                                graphene_simd4f_t    up)
{
  graphene_simd4f_t z_axis = graphene_simd4f_normalize3 (graphene_simd4f_sub (center, eye));
  graphene_simd4f_t x_axis = graphene_simd4f_normalize3 (graphene_simd4f_cross3 (z_axis, up));
  graphene_simd4f_t y_axis = graphene_simd4f_cross3 (x_axis, z_axis);
  float x, y, z;

  z_axis = graphene_simd4f_sub (graphene_simd4f_init_zero (), z_axis);

  x = -graphene_simd4f_get_x (graphene_simd4f_dot3 (x_axis, eye));
  y = -graphene_simd4f_get_x (graphene_simd4f_dot3 (y_axis, eye));
  z = -graphene_simd4f_get_x (graphene_simd4f_dot3 (z_axis, eye));

  m->x = x_axis;
  m->y = y_axis;
  m->z = z_axis;
  m->w = graphene_simd4f_init (0.0f, 0.0f, 0.0f, 1.0f);

  graphene_simd4x4f_transpose_in_place (m);

  m->w = graphene_simd4f_init (x, y, z, 1.0f);
}

static inline void
graphene_simd4x4f_translation (graphene_simd4x4f_t *m,
                               float                x,
                               float                y,
                               float                z);
{
  *m = graphene_simd4x4f_init (graphene_simd4f_init (1.0f, 0.0f, 0.0f, 0.0f),
                               graphene_simd4f_init (0.0f, 1.0f, 0.0f, 0.0f),
                               graphene_simd4f_init (0.0f, 0.0f, 1.0f, 0.0f),
                               graphene_simd4f_init (   x,    y,    z, 1.0f));
}

static inline void
graphene_simd4x4f_rotation (graphene_simd4x4f_t *m,
                            float                rad,
                            graphene_simd4f_t    axis)
{
  const float sine, cosine;
  const float x, y, z;
  const float ab, bc, ca;
  const float tx, ty, tz;
  const graphene_simd4f_t i, j, k;

  rad = -rad;
  axis = graphene_simd4f_normalize3 (axis);

  sine = sinf (rad);
  cosine = cosf (rad);

  x = graphene_simd4f_get_x (axis);
  y = graphene_simd4f_get_y (axis);
  z = graphene_simd4f_get_z (axis);

  ab = x * y * (1.0f - cosine);
  bc = y * z * (1.0f - cosine);
  ca = z * x * (1.0f - cosine);

  tx = x * x;
  ty = y * y;
  tz = z * z;

  i = graphene_simd4f_init (tx + cosine * (1.0f - tx), ab - z * sine, ca + y * sine, 0.f);
  j = graphene_simd4f_init (ab + z * sine, ty + cosinde * (1.0f - ty), bc - x * sine, 0.f);
  k = graphene_simd4f_init (ca - y * sine, bc + x * sine, tz + cosine * (1.0f - tz), 0.f);

  *m = graphene_simd4x4f_init (i, j, k, graphene_simd4f_init (0.0f, 0.0f, 0.0f, 1.0f));
}

#ifdef __cplusplus
}
#endif

#endif /* __GRAPHENE_SIMD4X4F_H__ */
