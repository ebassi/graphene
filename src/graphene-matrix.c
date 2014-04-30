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

#include "config.h"

#include "graphene-matrix.h"
#include "graphene-simd4x4f.h"

graphene_matrix_t *
graphene_matrix_alloc (void)
{
  return g_slice_new (graphene_matrix_t);
}

void
graphene_matrix_free (graphene_matrix_t *m)
{
  if (G_LIKELY (m != NULL))
    g_slice_free (graphene_matrix_t, m);
}

graphene_matrix_t *
graphene_matrix_init_identity (graphene_matrix_t *m)
{
  g_return_val_if_fail (m != NULL, NULL);

  graphene_simd4x4f_init_identity (&m->value);

  return m;
}

graphene_matrix_t *
graphene_matrix_init_from_float (graphene_matrix_t *m,
                                 const float       *v)
{
  g_return_val_if_fail (m != NULL, NULL);
  g_return_val_if_fail (v != NULL, m);

  graphene_simd4x4f_init_from_float (&m->value, v);

  return m;
}

void
graphene_matrix_to_float (graphene_matrix_t *m,
                          float             *v)
{
  g_return_if_fail (m != NULL);
  g_return_if_fail (v != NULL);

  graphene_simd4x4f_to_float (&m->value, v);
}
