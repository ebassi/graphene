/* graphene-macros.h: Public macros
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

#ifndef __GRAPHENE_MACROS_H__
#define __GRAPHENE_MACROS_H__

#if !defined(GRAPHENE_H_INSIDE) && !defined(GRAPHENE_COMPILATION)
#error "Only graphene.h can be included directly."
#endif

#if defined(GRAPHENE_HAS_SSE)
# define GRAPHENE_USE_SSE
#elif defined(__GNUC__) && !defined(__arm__)
# define GRAPHENE_USE_GCC
#else
# define GRAPHENE_USE_SCALAR
#endif

#if defined(__GNUC__) 
# define GRAPHENE_SIMD4F_ALIGNED16 __attribute__((aligned (16)))
#elif defined(_WIN32)
# define GRAPHENE_SIMD4F_ALIGNED16 __declspec(align(16))
#else
# define GRAPHENE_SIMD4F_ALIGNED16
#endif

#ifndef _GRAPHENE_PUBLIC
#define _GRAPHENE_PUBLIC        extern
#endif

#define GRAPHENE_VERSION_1_0            G_ENCODE_VERSION (1, 0)
#define GRAPHENE_AVAILABLE_IN_1_0       _GRAPHENE_PUBLIC

#endif /* __GRAPHENE_MACROS_H__ */
