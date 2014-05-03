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

  graphene_rect_normalize_internal (r);

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
graphene_rect_get_center (const graphene_rect_t  *r,
                          graphene_point_t       *p)
{
  graphene_rect_t rr;

  g_return_if_fail (r != NULL);
  g_return_if_fail (p != NULL);

  rr = *r;
  graphene_rect_normalize_internal (&rr);

  graphene_point_init (p,
                       rr.origin.x + (rr.size.width / 2.f),
                       rr.origin.y + (rr.size.height / 2.f));
}

void
graphene_rect_get_top_left (const graphene_rect_t  *r,
                            graphene_point_t       *p)
{
  graphene_rect_t rr;

  g_return_if_fail (r != NULL);
  g_return_if_fail (p != NULL);

  rr = *r;
  graphene_rect_normalize_internal (&rr);

  graphene_point_init_from_point (p, &rr.origin);
}

void
graphene_rect_get_top_right (const graphene_rect_t *r,
                             graphene_point_t      *p)
{
  graphene_rect_t rr;

  g_return_if_fail (r != NULL);
  g_return_if_fail (p != NULL);

  rr = *r;
  graphene_rect_normalize_internal (&rr);

  graphene_point_init (p, rr.origin.x + rr.size.width, rr.origin.y);
}

void
graphene_rect_get_bottom_left (const graphene_rect_t *r,
                               graphene_point_t      *p)
{
  graphene_rect_t rr;

  g_return_if_fail (r != NULL);
  g_return_if_fail (p != NULL);

  rr = *r;
  graphene_rect_normalize_internal (&rr);

  graphene_point_init (p, rr.origin.x, rr.origin.y + rr.size.height);
}

void
graphene_rect_get_bottom_right (const graphene_rect_t  *r,
                                graphene_point_t       *p)
{
  graphene_rect_t rr;

  g_return_if_fail (r != NULL);
  g_return_if_fail (p != NULL);

  rr = *r;
  graphene_rect_normalize_internal (&rr);

  graphene_point_init (p,
                       rr.origin.x + rr.size.width,
                       rr.origin.y + rr.size.height);
}

#define GRAPHENE_RECT_GET(part,field) \
float \
graphene_rect_get_##field (const graphene_rect_t *r) \
{ \
  graphene_rect_t rr; \
\
  g_return_val_if_fail (r != NULL, 0.f); \
\
  rr = *r; \
  graphene_rect_normalize_internal (&rr); \
\
  return rr.part.field; \
}

GRAPHENE_RECT_GET (origin, x)
GRAPHENE_RECT_GET (origin, y)
GRAPHENE_RECT_GET (size, width)
GRAPHENE_RECT_GET (size, height)

#undef GRAPHENE_RECT_GET

void
graphene_rect_union (const graphene_rect_t *a,
                     const graphene_rect_t *b,
                     graphene_rect_t       *res)
{
  graphene_rect_t ra, rb;

  g_return_if_fail (a != NULL && b != NULL);
  g_return_if_fail (res != NULL);

  ra = *a;
  rb = *b;

  graphene_rect_normalize_internal (&ra);
  graphene_rect_normalize_internal (&rb);

  res->origin.x = MIN (ra.origin.x, rb.origin.x);
  res->origin.y = MIN (ra.origin.y, rb.origin.y);

  res->size.width = MAX (ra.size.width, rb.size.width);
  res->size.height = MAX (ra.size.height, rb.size.height);
}

gboolean
graphene_rect_intersection (const graphene_rect_t *a,
                            const graphene_rect_t *b,
                            graphene_rect_t       *res)
{
  graphene_rect_t ra, rb;
  float x_1, y_1, x_2, y_2;

  g_return_val_if_fail (a != NULL && b != NULL, FALSE);

  ra = *a;
  rb = *b;

  graphene_rect_normalize_internal (&ra);
  graphene_rect_normalize_internal (&rb);

  x_1 = MAX (ra.origin.x, rb.origin.x);
  y_1 = MAX (ra.origin.y, rb.origin.y);
  x_2 = MIN (ra.origin.x + ra.size.width, rb.origin.x + rb.size.width);
  y_2 = MIN (ra.origin.y + ra.size.height, rb.origin.x + rb.size.height);

  if (x_1 >= x_2 || y_1 >= y_2)
    {
      if (res != NULL)
        graphene_rect_init (res, 0.0f, 0.0f, 0.0f, 0.0f);

      return FALSE;
    }

  if (res != NULL)
    graphene_rect_init (res, x_1, y_1, x_2 - x_1, y_2 - y_1);

  return TRUE;
}

gboolean
graphene_rect_contains_point (const graphene_rect_t  *r,
                              const graphene_point_t *p)
{
  graphene_rect_t rr;

  g_return_val_if_fail (r != NULL && p != NULL, FALSE);

  rr = *r;
  graphene_rect_normalize_internal (&rr);

  return p->x >= rr.origin.x &&
         p->y >= rr.origin.y &&
         p->x <= (rr.origin.x + rr.size.width) &&
         p->y <= (rr.origin.y + rr.size.height);
}

gboolean
graphene_rect_contains_rect (const graphene_rect_t *a,
                             const graphene_rect_t *b)
{
  graphene_rect_t res;

  g_return_val_if_fail (a != NULL && b != NULL, FALSE);

  graphene_rect_union (a, b, &res);

  return graphene_rect_equal (a, &res);
}

void
graphene_rect_offset (graphene_rect_t *r,
                      float            d_x,
                      float            d_y)
{
  g_return_if_fail (r != NULL);

  graphene_rect_normalize_internal (r);

  r->origin.x += d_x;
  r->origin.y += d_y;
}

void
graphene_rect_inset (graphene_rect_t *r,
                     float            d_x,
                     float            d_y)
{
  g_return_if_fail (r != NULL);

  graphene_rect_normalize_internal (r);

  r->origin.x += d_x;
  r->origin.y += d_y;

  if (d_x >= 0.f)
    r->size.width -= (d_x * 2.f);
  else
    r->size.width += (d_x * -2.f);

  if (d_y >= 0.f)
    r->size.height -= (d_y * 2.f);
  else
    r->size.height += (d_y * -2.f);

  if (r->size.width < 0.f)
    r->size.width = 0.f;

  if (r->size.height < 0.f)
    r->size.height = 0.f;
}

void
graphene_rect_round_to_pixel (graphene_rect_t *r)
{
  g_return_if_fail (r != NULL);

  graphene_rect_normalize_internal (r);

  r->origin.x = floorf (r->origin.x);
  r->origin.y = floorf (r->origin.y);

  r->size.width = ceilf (r->size.width);
  r->size.height = ceilf (r->size.height);
}

void
graphene_rect_interpolate (const graphene_rect_t *a,
                           const graphene_rect_t *b,
                           double                 factor,
                           graphene_rect_t       *res)
{
  graphene_rect_t ra, rb;

  g_return_if_fail (a != NULL && b != NULL);
  g_return_if_fail (res != NULL);

  ra = *a;
  graphene_rect_normalize_internal (&ra);

  rb = *b;
  graphene_rect_normalize_internal (&rb);

#define INTERPOLATE(r_a,r_b,member,field,p) \
  (r_a).member.field + (((r_b).member.field - ((r_a).member.field)) * (p));

  res->origin.x = INTERPOLATE (ra, rb, origin, x, factor)
  res->origin.y = INTERPOLATE (ra, rb, origin, y, factor)
  res->size.width = INTERPOLATE (ra, rb, size, width, factor)
  res->size.height = INTERPOLATE (ra, rb, size, height, factor)

#undef INTERPOLATE
}
