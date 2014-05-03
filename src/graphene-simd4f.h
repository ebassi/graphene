/* graphene-simd4f.h: Four float vector operations
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

#ifndef __GRAPHENE_SIMD4F_H__
#define __GRAPHENE_SIMD4F_H__

#include "graphene-macros.h"
#include "graphene-config.h"

#if defined(GRAPHENE_USE_SSE)
# include "graphene-simd4f-sse.h"
#elif defined(GRAPHENE_USE_ARM_NEON)
# include "graphene-simd4f-neon.h" /* include graphene-simd4f-neon.h */
#elif defined(GRAPHENE_USE_GCC)
# include "graphene-simd4f-gcc.h"
#elif defined(GRAPHENE_USE_SCALAR)
# include "graphene-simd4f-scalar.h"
#else
# error "Unsupported simd4f implementation."
#endif

#include "graphene-simd4f-common.h"

#ifdef GRAPHENE_ENABLE_DEBUG
# define GRAPHENE_SIMD4F_DEBUG(vec)             G_STMT_START { \
  g_debug ("simd4f(%.6f,%.6f,%.6f,%.6f)", \
           graphene_simd4f_get_x (vec), \
           graphene_simd4f_get_y (vec), \
           graphene_simd4f_get_z (vec), \
           graphene_simd4f_get_w (vec));        } G_STMT_END
#else
# define GRAPHENE_SIMD4F_DEBUG(vec)
#endif

#endif /* __GRAPHENE_SIMD4F_H__ */
