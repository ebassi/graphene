/* graphene-size.c: Size
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

/**
 * SECTION:graphene-size
 * @Title: Size
 * @Short_Description: Size representation
 *
 * ...
 */

#include "config.h"

#include <math.h>

#include "graphene-point.h"

graphene_size_t *
graphene_size_alloc (void)
{
  return g_slice_new (graphene_size_t);
}

void
graphene_size_free (graphene_size_t *s)
{
  if (G_LIKELY (s != NULL))
    g_slice_free (graphene_size_t, s);
}

graphene_size_t *
graphene_size_init (graphene_size_t *s,
                    float            width,
                    float            height)
{
  g_return_val_if_fail (s != NULL, NULL);

  s->width = width;
  s->height = height;

  return s;
}

graphene_size_t *
graphene_size_init_from_size (graphene_size_t       *s,
                              const graphene_size_t *src)
{
  g_return_val_if_fail (s != NULL, NULL);
  g_return_val_if_fail (src != NULL, s);

  *s = *src;

  return s;
}

gboolean
graphene_size_equal (const graphene_size_t *a,
                     const graphene_size_t *b)
{
  if (a == b)
    return TRUE;

  if (a == NULL || b == NULL)
    return FALSE;

  return fabsf (a->width - b->width) < GRAPHENE_FLOAT_EPSILON &&
         fabsf (a->height - b->height) < GRAPHENE_FLOAT_EPSILON;
}

void
graphene_size_scale (const graphene_size_t *s,
                     float                  factor,
                     graphene_size_t       *res)
{
  g_return_if_fail (s != NULL);
  g_return_if_fail (res != NULL);

  *res = *s;

  res->width *= factor;
  res->height *= factor;
}

void
graphene_size_interpolate (const graphene_size_t *a,
                           const graphene_size_t *b,
                           double                 factor,
                           graphene_size_t       *res)
{
  g_return_if_fail (a != NULL && b != NULL);
  g_return_if_fail (res != NULL);

  res->width = a->width + (b->width - a->width) * factor;
  res->height = a->height + (b->height - a->height) * factor;
}

static const graphene_size_t _graphene_size_zero = GRAPHENE_SIZE_INIT_ZERO;

const graphene_size_t *
graphene_size_zero (void)
{
#ifdef GRAPHENE_ENABLE_DEBUG
  g_assert (_graphene_size_zero.width == 0.f);
  g_assert (_graphene_size_zero.height == 0.f);
#endif

  return &_graphene_size_zero;
}
