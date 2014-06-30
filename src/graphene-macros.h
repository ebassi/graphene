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

#ifndef _GRAPHENE_PUBLIC
#define _GRAPHENE_PUBLIC        extern
#endif

#define GRAPHENE_FLOAT_EPSILON  (1e-15)

#define GRAPHENE_PI             3.1415926535897932384626434f
#define GRAPHENE_PI_2           1.5707963267948966192313217f

#ifdef GRAPHENE_COMPILATION
# define GRAPHENE_PRIVATE_FIELD(type,name)      type name
#else
# define GRAPHENE_PRIVATE_FIELD(type,name)      type __graphene_private_##name
#endif

#if defined(__GNUC__)
# define GRAPHENE_ALIGN16  __attribute__((aligned(16)))
#elif defined(_MSC_VER)
# define GRAPHENE_ALIGN16  __declspec(align(16))
#else
# define GRAPHENE_ALIGN16
#endif

#ifdef __cplusplus
# define GRAPHENE_BEGIN_DECLS   extern "C" {
# define GRAPHENE_END_DECLS     }
#else
# define GRAPHENE_BEGIN_DECLS
# define GRAPHENE_END_DECLS
#endif

#endif /* __GRAPHENE_MACROS_H__ */
