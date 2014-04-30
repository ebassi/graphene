/* graphene-matrix.h: 4x4 matrix
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

#ifndef __GRAPHENE_MATRIX_H__
#define __GRAPHENE_MATRIX_H__

#include "graphene-types.h"

G_BEGIN_DECLS

/**
 * graphene_matrix_t:
 *
 * A structure capable of holding a 4x4 matrix.
 *
 * The contents of the #graphene_matrix_t structure are private and
 * should never be accessed directly.
 */
struct _graphene_matrix_t
{
  /*< private >*/
  GRAPHENE_PRIVATE_FIELD (graphene_simd4x4f_t, value);
};

GRAPHENE_AVAILABLE_IN_1_0
graphene_matrix_t *     graphene_matrix_alloc           (void);
GRAPHENE_AVAILABLE_IN_1_0
void                    graphene_matrix_free            (graphene_matrix_t *m);

GRAPHENE_AVAILABLE_IN_1_0
graphene_matrix_t *     graphene_matrix_init_identity   (graphene_matrix_t *m);
GRAPHENE_AVAILABLE_IN_1_0
graphene_matrix_t *     graphene_matrix_init_from_float (graphene_matrix_t *m,
                                                         const float       *v);

GRAPHENE_AVAILABLE_IN_1_0
void                    graphene_matrix_to_float        (graphene_matrix_t *m,
                                                         float             *v);

G_END_DECLS

#endif /* __GRAPHENE_MATRIX_H__ */
