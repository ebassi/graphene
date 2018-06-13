/* graphene-vec3.h: 3-coords vector
 *
 * Copyright 2014  Emmanuele Bassi
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

#ifndef __GRAPHENE_VEC3_H__
#define __GRAPHENE_VEC3_H__

#if !defined(GRAPHENE_H_INSIDE) && !defined(GRAPHENE_COMPILATION)
#error "Only graphene.h can be included directly."
#endif

#include "graphene-types.h"

GRAPHENE_BEGIN_DECLS

/**
 * graphene_vec3_t:
 *
 * A structure capable of holding a vector with three dimensions: x, y, and z.
 *
 * The contents of the #graphene_vec3_t structure are private and should
 * never be accessed directly.
 */
struct _graphene_vec3_t
{
  /*< private >*/
  GRAPHENE_ALIGN16 GRAPHENE_PRIVATE_FIELD (graphene_simd4f_t, value);
};

GRAPHENE_AVAILABLE_IN_1_0
graphene_vec3_t *       graphene_vec3_alloc             (void);
GRAPHENE_AVAILABLE_IN_1_0
void                    graphene_vec3_free              (graphene_vec3_t       *v);
GRAPHENE_AVAILABLE_IN_1_0
graphene_vec3_t *       graphene_vec3_init              (graphene_vec3_t       *v,
                                                         float                  x,
                                                         float                  y,
                                                         float                  z);
GRAPHENE_AVAILABLE_IN_1_0
graphene_vec3_t *       graphene_vec3_init_from_vec3    (graphene_vec3_t       *v,
                                                         const graphene_vec3_t *src);
GRAPHENE_AVAILABLE_IN_1_0
graphene_vec3_t *       graphene_vec3_init_from_float   (graphene_vec3_t       *v,
                                                         const float           *src);
GRAPHENE_AVAILABLE_IN_1_0
void                    graphene_vec3_to_float          (const graphene_vec3_t *v,
                                                         float                 *dest);
GRAPHENE_AVAILABLE_IN_1_0
void                    graphene_vec3_add               (const graphene_vec3_t *a,
                                                         const graphene_vec3_t *b,
                                                         graphene_vec3_t       *res);
GRAPHENE_AVAILABLE_IN_1_0
void                    graphene_vec3_subtract          (const graphene_vec3_t *a,
                                                         const graphene_vec3_t *b,
                                                         graphene_vec3_t       *res);
GRAPHENE_AVAILABLE_IN_1_0
void                    graphene_vec3_multiply          (const graphene_vec3_t *a,
                                                         const graphene_vec3_t *b,
                                                         graphene_vec3_t       *res);
GRAPHENE_AVAILABLE_IN_1_0
void                    graphene_vec3_divide            (const graphene_vec3_t *a,
                                                         const graphene_vec3_t *b,
                                                         graphene_vec3_t       *res);
GRAPHENE_AVAILABLE_IN_1_0
void                    graphene_vec3_cross             (const graphene_vec3_t *a,
                                                         const graphene_vec3_t *b,
                                                         graphene_vec3_t       *res);
GRAPHENE_AVAILABLE_IN_1_0
float                   graphene_vec3_dot               (const graphene_vec3_t *a,
                                                         const graphene_vec3_t *b);
GRAPHENE_AVAILABLE_IN_1_0
float                   graphene_vec3_length            (const graphene_vec3_t *v);
GRAPHENE_AVAILABLE_IN_1_0
void                    graphene_vec3_normalize         (const graphene_vec3_t *v,
                                                         graphene_vec3_t       *res);
GRAPHENE_AVAILABLE_IN_1_2
void                    graphene_vec3_scale             (const graphene_vec3_t *v,
                                                         float                  factor,
                                                         graphene_vec3_t       *res);
GRAPHENE_AVAILABLE_IN_1_2
void                    graphene_vec3_negate            (const graphene_vec3_t *v,
                                                         graphene_vec3_t       *res);
GRAPHENE_AVAILABLE_IN_1_2
bool                    graphene_vec3_equal             (const graphene_vec3_t *v1,
                                                         const graphene_vec3_t *v2);
GRAPHENE_AVAILABLE_IN_1_2
bool                    graphene_vec3_near              (const graphene_vec3_t *v1,
                                                         const graphene_vec3_t *v2,
                                                         float                  epsilon);
GRAPHENE_AVAILABLE_IN_1_0
void                    graphene_vec3_min               (const graphene_vec3_t *a,
                                                         const graphene_vec3_t *b,
                                                         graphene_vec3_t       *res);
GRAPHENE_AVAILABLE_IN_1_0
void                    graphene_vec3_max               (const graphene_vec3_t *a,
                                                         const graphene_vec3_t *b,
                                                         graphene_vec3_t       *res);

GRAPHENE_AVAILABLE_IN_1_0
float                   graphene_vec3_get_x             (const graphene_vec3_t *v);
GRAPHENE_AVAILABLE_IN_1_0
float                   graphene_vec3_get_y             (const graphene_vec3_t *v);
GRAPHENE_AVAILABLE_IN_1_0
float                   graphene_vec3_get_z             (const graphene_vec3_t *v);

GRAPHENE_AVAILABLE_IN_1_0
void                    graphene_vec3_get_xy            (const graphene_vec3_t *v,
                                                         graphene_vec2_t       *res);
GRAPHENE_AVAILABLE_IN_1_0
void                    graphene_vec3_get_xy0           (const graphene_vec3_t *v,
                                                         graphene_vec3_t       *res);
GRAPHENE_AVAILABLE_IN_1_0
void                    graphene_vec3_get_xyz0          (const graphene_vec3_t *v,
                                                         graphene_vec4_t       *res);
GRAPHENE_AVAILABLE_IN_1_0
void                    graphene_vec3_get_xyz1          (const graphene_vec3_t *v,
                                                         graphene_vec4_t       *res);
GRAPHENE_AVAILABLE_IN_1_0
void                    graphene_vec3_get_xyzw          (const graphene_vec3_t *v,
                                                         float                  w,
                                                         graphene_vec4_t       *res);

GRAPHENE_AVAILABLE_IN_1_0
const graphene_vec3_t * graphene_vec3_zero              (void);
GRAPHENE_AVAILABLE_IN_1_0
const graphene_vec3_t * graphene_vec3_one               (void);
GRAPHENE_AVAILABLE_IN_1_0
const graphene_vec3_t * graphene_vec3_x_axis            (void);
GRAPHENE_AVAILABLE_IN_1_0
const graphene_vec3_t * graphene_vec3_y_axis            (void);
GRAPHENE_AVAILABLE_IN_1_0
const graphene_vec3_t * graphene_vec3_z_axis            (void);

GRAPHENE_END_DECLS

#endif /* __GRAPHENE_VEC3_H__ */
