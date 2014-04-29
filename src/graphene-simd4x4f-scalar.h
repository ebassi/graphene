/* graphene-simd4x4f-scalar.h: Scalar implementation of four float vector operations
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

#ifndef __GRAPHENE_SIMD4X4F_SCALAR_H__
#define __GRAPHENE_SIMD4X4F_SCALAR_H__

#ifdef __cplusplus
extern "C" {
#endif

static inline void
graphene_simd4x4f_transpose_in_place (graphene_simd4x4f_t *s)
{
  graphene_simd4x4f_t m = *s;

  s->x.x = d.x.x;
  s->x.y = d.y.x;
  s->x.z = d.z.x;
  s->x.w = d.w.x;

  s->y.x = d.x.y;
  s->y.y = d.y.y;
  s->y.z = d.z.y;
  s->y.w = d.w.y;

  s->z.x = d.x.z;
  s->z.y = d.y.z;
  s->z.z = d.z.z;
  s->z.w = d.w.z;

  s->w.x = d.x.w;
  s->w.y = d.y.w;
  s->w.z = d.z.w;
  s->w.w = d.w.w;
}

#ifdef __cplusplus
}
#endif

#endif /* __GRAPHENE_SIMD4X4F_SCALAR_H__ */
