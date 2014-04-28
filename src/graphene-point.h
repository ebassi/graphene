/* graphene-point.h: Point and Size
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

#ifndef __GRAPHENE_POINT_H__
#define __GRAPHENE_POINT_H__

#if !defined(GRAPHENE_H_INSIDE) && !defined(GRAPHENE_COMPILATION)
#error "Only graphene.h can be included directly."
#endif

#include "graphene-types.h"

G_BEGIN_DECLS

#define GRAPHENE_POINT_INIT(x,y)        { x, y }
#define GRAPHENE_POINT_INIT_ZERO        GRAPHENE_POINT_INIT (0.f, 0.f)

struct _graphene_point_t
{
  float x;
  float y;
};

GRAPHENE_AVAILABLE_IN_1_0
graphene_point_t *              graphene_point_alloc            (void);
GRAPHENE_AVAILABLE_IN_1_0
void                            graphene_point_free             (graphene_point_t       *p);

GRAPHENE_AVAILABLE_IN_1_0
graphene_point_t *              graphene_point_init             (graphene_point_t       *p,
                                                                 float                   x,
                                                                 float                   y);
GRAPHENE_AVAILABLE_IN_1_0
graphene_point_t *              graphene_point_init_from_point  (graphene_point_t *p,
                                                                 const graphene_point_t *src);
GRAPHENE_AVAILABLE_IN_1_0
gboolean                        graphene_point_equal            (const graphene_point_t *a,
                                                                 const graphene_point_t *b);

GRAPHENE_AVAILABLE_IN_1_0
float                           graphene_point_distance         (const graphene_point_t *a,
                                                                 const graphene_point_t *b,
                                                                 float                  *d_x,
                                                                 float                  *d_y);

GRAPHENE_AVAILABLE_IN_1_0
gboolean                        graphene_point_near             (const graphene_point_t *a,
                                                                 const graphene_point_t *b,
                                                                 float                   epsilon);

GRAPHENE_AVAILABLE_IN_1_0
void                            graphene_point_interpolate      (const graphene_point_t *a,
                                                                 const graphene_point_t *b,
                                                                 double                  factor,
                                                                 graphene_point_t       *res);

GRAPHENE_AVAILABLE_IN_1_0
const graphene_point_t *        graphene_point_zero             (void);

#define GRAPHENE_SIZE_INIT(w,h)         { w, h }
#define GRAPHENE_SIZE_INIT_ZERO         GRAPHENE_SIZE_INIT (0.f, 0.f)

struct _graphene_size_t
{
  float width;
  float height;
};

GRAPHENE_AVAILABLE_IN_1_0
graphene_size_t *               graphene_size_alloc             (void);
GRAPHENE_AVAILABLE_IN_1_0
void                            graphene_size_free              (graphene_size_t        *s);
GRAPHENE_AVAILABLE_IN_1_0
graphene_size_t *               graphene_size_init              (graphene_size_t        *s,
                                                                 float                   width,
                                                                 float                   height);
GRAPHENE_AVAILABLE_IN_1_0
graphene_size_t *               graphene_size_init_from_size    (graphene_size_t        *s,
                                                                 const graphene_size_t  *src);
GRAPHENE_AVAILABLE_IN_1_0
gboolean                        graphene_size_equal             (const graphene_size_t  *a,
                                                                 const graphene_size_t  *b);

GRAPHENE_AVAILABLE_IN_1_0
void                            graphene_size_scale             (const graphene_size_t  *s,
                                                                 float                   factor,
                                                                 graphene_size_t        *res);
GRAPHENE_AVAILABLE_IN_1_0
void                            graphene_size_interpolate       (const graphene_size_t  *a,
                                                                 const graphene_size_t  *b,
                                                                 double                  factor,
                                                                 graphene_size_t        *res);

const graphene_size_t *         graphene_size_zero              (void);

G_END_DECLS

#endif /* __GRAPHENE_POINT_H__ */
