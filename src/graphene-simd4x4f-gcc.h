/* graphene-simd4x4f-gcc.h: GCC implementation of four float vector operations
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

#ifndef __GRAPHENE_SIMD4X4F_GCC_H__
#define __GRAPHENE_SIMD4X4F_GCC_H__

#include "graphene-simd4f-gcc.h"

#ifdef __cplusplus
extern "C" {
#endif

static inline void
graphene_simd4x4f_transpose_in_place (graphene_simd4x4f_t *s)
{
  const graphene_simd4f_union_t s_x = { s->x };
  const graphene_simd4f_union_t s_y = { s->y };
  const graphene_simd4f_union_t s_z = { s->z };
  const graphene_simd4f_union_t s_w = { s->w };

  s->x = graphene_simd4f_init (s_x.f[0], s_y.f[0], s_z.f[0], s_w.f[0]);
  s->y = graphene_simd4f_init (s_x.f[1], s_y.f[1], s_z.f[1], s_w.f[1]);
  s->z = graphene_simd4f_init (s_x.f[2], s_y.f[2], s_z.f[2], s_w.f[2]);
  s->w = graphene_simd4f_init (s_x.f[3], s_y.f[3], s_z.f[3], s_w.f[3]);
}

#ifdef __cplusplus
}
#endif

#endif /* __GRAPHENE_SIMD4X4F_GCC_H__ */
