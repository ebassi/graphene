/* graphene-rect.c: Rectangular type
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

#include <math.h>

#include "graphene-rect.h"

static void
graphene_rect_normalize_internal (graphene_rect_t *r)
{
  if (G_LIKELY (r->size.width >= 0.f && r->size.height >= 0.f))
    return;

  if (r->size.width < 0.f)
    {
      float size = fabsf (r->size.width);

      r->origin.x -= size;
      r->size.width = size;
    }

  if (r->size.height < 0.f)
    {
      float size = fabsf (r->size.height);

      r->origin.y -= size;
      r->size.height = size;
    }
}

graphene_rect_t *
graphene_rect_alloc (void)
{
  return g_slice_new (graphene_rect_t);
}

void
graphene_rect_free (graphene_rect_t *r)
{
  if (G_LIKELY (r != NULL))
    g_slice_free (graphene_rect_t, r);
}

graphene_rect_t *
graphene_rect_init (graphene_rect_t *r,
                    float            x,
                    float            y,
                    float            width,
                    float            height)
{
  g_return_val_if_fail (r != NULL, NULL);

  r->origin.x = x;
  r->origin.y = y;
  r->size.width = width;
  r->size.height = height;

  return r;
}

graphene_rect_t *
graphene_rect_init_with_rect (graphene_rect_t       *r,
                              const graphene_rect_t *src)
{
  g_return_val_if_fail (r != NULL, NULL);
  g_return_val_if_fail (src != NULL, r);

  *r = *src;

  return r;
}

gboolean
graphene_rect_equal (const graphene_rect_t *a,
                     const graphene_rect_t *b)
{
  graphene_rect_t r_a = *a;
  graphene_rect_t r_b = *b;

  graphene_rect_normalize_internal (&r_a);
  graphene_rect_normalize_internal (&r_b);

  return graphene_point_equal (&r_a.origin, &r_b.origin) &&
         graphene_size_equal (&r_a.size, &r_b.size);
}

graphene_rect_t *
graphene_rect_normalize (graphene_rect_t *r)
{
  g_return_val_if_fail (r != NULL, NULL);

  graphene_rect_normalize_internal (r);

  return r;
}

void
graphene_rect_get_center (graphene_rect_t  *r,
                          graphene_point_t *p)
{
  graphene_rect_t rr;

  g_return_if_fail (r != NULL);
  g_return_if_fail (p != NULL);

  rr = *r;
  graphene_rect_normalize_internal (&rr);

  p->x = rr.origin.x + (rr.size.width / 2.f);
  p->y = rr.origin.y + (rr.size.height / 2.f);
}
