/* graphene-simd4x4f-neon.h: ARM NEON implementation of four float vector operations
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

#ifndef __GRAPHENE_SIMD4X4F_NEON_H__
#define __GRAPHENE_SIMD4X4F_NEON_H__

#warn "ARM NEON support is experimental and requires testing."

static inline void
graphene_simd4x4f_transpose_in_place (graphene_simd4x4f_t *s)
{
  const graphene_simd4f_union_t sx = { s->x };
  const graphene_simd4f_union_t sy = { s->y };
  const graphene_simd4f_union_t sz = { s->z };
  const graphene_simd4f_union_t sw = { s->w };

  const graphene_simd4f_t dx = graphene_simd4f_init (sx.f[0], sy.f[0], sz.f[0], sw.f[0]);
  const graphene_simd4f_t dy = graphene_simd4f_init (sx.f[1], sy.f[1], sz.f[1], sw.f[1]);
  const graphene_simd4f_t dz = graphene_simd4f_init (sx.f[2], sy.f[2], sz.f[2], sw.f[2]);
  const graphene_simd4f_t dw = graphene_simd4f_init (sx.f[3], sy.f[3], sz.f[3], sw.f[3]);

  s->x = dx;
  s->y = dy;
  s->z = dz;
  s->w = dw;
}

#endif /* __GRAPHENE_SIMD4X4F_NEON_H__ */
