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

#include "graphene-config.h"
#include "graphene-simd4f.h"
#include <math.h>

#ifdef __cplusplus
extern "C" {
#endif

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
graphene_simd4x4f_init_identity (graphene_simd4x4f_t *m) {
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

static inline void
graphene_simd4x4f_to_float (const graphene_simd4x4f_t *m,
                            float                     *v)
{
  graphene_simd4f_dup_4f (m->x, v);
  graphene_simd4f_dup_4f (m->y, v +  4);
  graphene_simd4f_dup_4f (m->z, v +  8);
  graphene_simd4f_dup_4f (m->w, v + 12);
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

#include "graphene-simd4x4f-common.h"

#ifdef __cplusplus
}
#endif

#endif /* __GRAPHENE_SIMD4X4F_H__ */
