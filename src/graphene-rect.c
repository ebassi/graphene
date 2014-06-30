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

/**
 * SECTION:graphene-rect
 * @title: Rectangle
 * @short_description: Rectangular shape type
 *
 * #graphene_rect_t is a type representing a rectangle through an origin
 * #graphene_point_t point and a #graphene_size_t size.
 *
 * Operations on a #graphene_rect_t will normalize the rectangle, to
 * ensure that the origin is always the top-left corner of the rectangle
 * and that the size is always positive.
 */

#include "graphene-private.h"
#include "graphene-rect.h"

#include <math.h>

static void
graphene_rect_normalize_internal (graphene_rect_t *r)
{
  if (likely (r->size.width >= 0.f && r->size.height >= 0.f))
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

/**
 * graphene_rect_alloc:
 *
 * Allocates a new #graphene_rect_t.
 *
 * The contents of the returned rectangle are undefined.
 *
 * Returns: (transfer full): the newly allocated rectangle
 *
 * Since: 1.0
 */
graphene_rect_t *
graphene_rect_alloc (void)
{
  return calloc (1, sizeof (graphene_rect_t));
}

/**
 * graphene_rect_free:
 * @r: a #graphene_rect_t
 *
 * Frees the resources allocated by graphene_rect_alloc().
 *
 * Since: 1.0
 */
void
graphene_rect_free (graphene_rect_t *r)
{
  free (r);
}

/**
 * graphene_rect_init:
 * @r: a #graphene_rect_t
 * @x: the X coordinate of the @graphene_rect_t.origin
 * @y: the Y coordinate of the @graphene_rect_t.origin
 * @width: the width of the @graphene_rect_t.size
 * @height: the height of the @graphene_rect_t.size
 *
 * Initializes the given #graphene_rect_t with the given values.
 *
 * This function will implicitly normalize the #graphene_rect_t
 * before returning.
 *
 * Returns: (transfer none): the initialized rectangle
 *
 * Since: 1.0
 */
graphene_rect_t *
graphene_rect_init (graphene_rect_t *r,
                    float            x,
                    float            y,
                    float            width,
                    float            height)
{
  r->origin.x = x;
  r->origin.y = y;
  r->size.width = width;
  r->size.height = height;

  graphene_rect_normalize_internal (r);

  return r;
}

/**
 * graphene_rect_init_from_rect:
 * @r: a #graphene_rect_t
 * @src: a #graphene_rect_t
 *
 * Initializes @r using the given @src rectangle.
 *
 * This function will implicitly normalize the #graphene_rect_t
 * before returning.
 *
 * Returns: (transfer none): the initialized rectangle
 *
 * Since: 1.0
 */
graphene_rect_t *
graphene_rect_init_from_rect (graphene_rect_t       *r,
                              const graphene_rect_t *src)
{
  *r = *src;

  graphene_rect_normalize_internal (r);

  return r;
}

/**
 * graphene_rect_equal:
 * @a: a #graphene_rect_t
 * @b: a #graphene_rect_t
 *
 * Checks whether the two given rectangle are equal.
 *
 * Returns: %true if the rectangles are equal
 *
 * Since: 1.0
 */
bool
graphene_rect_equal (const graphene_rect_t *a,
                     const graphene_rect_t *b)
{
  graphene_rect_t r_a, r_b;

  if (a == b)
    return true;

  if (a == NULL || b == NULL)
    return false;

  r_a = *a;
  r_b = *b;

  graphene_rect_normalize_internal (&r_a);
  graphene_rect_normalize_internal (&r_b);

  return graphene_point_equal (&r_a.origin, &r_b.origin) &&
         graphene_size_equal (&r_a.size, &r_b.size);
}

/**
 * graphene_rect_normalize:
 * @r: a #graphene_rect_t
 *
 * Normalizes the passed rectangle.
 *
 * This function ensures that the size of the rectangle is made of
 * positive values, and that the origin is the top-left corner of
 * the rectangle.
 *
 * Returns: (transfer none): the normalized rectangle
 *
 * Since: 1.0
 */
graphene_rect_t *
graphene_rect_normalize (graphene_rect_t *r)
{
  graphene_rect_normalize_internal (r);

  return r;
}

/**
 * graphene_rect_get_center:
 * @r: a #graphene_rect_t
 * @p: (out caller-allocates): return location for a #graphene_point_t
 *
 * Retrieves the coordinates of the center of the given rectangle.
 *
 * Since: 1.0
 */
void
graphene_rect_get_center (const graphene_rect_t  *r,
                          graphene_point_t       *p)
{
  graphene_rect_t rr;

  rr = *r;
  graphene_rect_normalize_internal (&rr);

  graphene_point_init (p,
                       rr.origin.x + (rr.size.width / 2.f),
                       rr.origin.y + (rr.size.height / 2.f));
}

/**
 * graphene_rect_get_top_left:
 * @r: a #graphene_rect_t
 * @p: (out caller-allocates): return location for a #graphene_point_t
 *
 * Retrieves the coordinates of the top-left corner of the given rectangle.
 *
 * Since: 1.0
 */
void
graphene_rect_get_top_left (const graphene_rect_t  *r,
                            graphene_point_t       *p)
{
  graphene_rect_t rr;

  rr = *r;
  graphene_rect_normalize_internal (&rr);

  graphene_point_init_from_point (p, &rr.origin);
}

/**
 * graphene_rect_get_top_right:
 * @r: a #graphene_rect_t
 * @p: (out caller-allocates): return location for a #graphene_point_t
 *
 * Retrieves the coordinates of the top-right corner of the given rectangle.
 *
 * Since: 1.0
 */
void
graphene_rect_get_top_right (const graphene_rect_t *r,
                             graphene_point_t      *p)
{
  graphene_rect_t rr;

  rr = *r;
  graphene_rect_normalize_internal (&rr);

  graphene_point_init (p, rr.origin.x + rr.size.width, rr.origin.y);
}

/**
 * graphene_rect_get_bottom_left:
 * @r: a #graphene_rect_t
 * @p: (out caller-allocates): return location for a #graphene_point_t
 *
 * Retrieves the coordinates of the bottom-left corner of the given rectangle.
 *
 * Since: 1.0
 */
void
graphene_rect_get_bottom_left (const graphene_rect_t *r,
                               graphene_point_t      *p)
{
  graphene_rect_t rr;

  rr = *r;
  graphene_rect_normalize_internal (&rr);

  graphene_point_init (p, rr.origin.x, rr.origin.y + rr.size.height);
}

/**
 * graphene_rect_get_bottom_right:
 * @r: a #graphene_rect_t
 * @p: (out caller-allocates): return location for a #graphene_point_t
 *
 * Retrieves the coordinates of the bottom-right corner of the given rectangle.
 *
 * Since: 1.0
 */
void
graphene_rect_get_bottom_right (const graphene_rect_t  *r,
                                graphene_point_t       *p)
{
  graphene_rect_t rr;

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
  rr = *r; \
  graphene_rect_normalize_internal (&rr); \
\
  return rr.part.field; \
}

/**
 * graphene_rect_get_x:
 * @r: a #graphene_rect_t
 *
 * Retrieves the X coordinate of the origin of the given
 * rectangle.
 *
 * Returns: the normalized X coordinate of the origin
 *
 * Since: 1.0
 */
GRAPHENE_RECT_GET (origin, x)

/**
 * graphene_rect_get_y:
 * @r: a #graphene_rect_t
 *
 * Retrieves the normalized Y coordinate of the origin of the given
 * rectangle.
 *
 * Returns: the Y coordinate of the origin
 *
 * Since: 1.0
 */
GRAPHENE_RECT_GET (origin, y)

/**
 * graphene_rect_get_width:
 * @r: a #graphene_rect_t
 *
 * Retrieves the normalized width of the given rectangle.
 *
 * Returns: the width
 *
 * Since: 1.0
 */
GRAPHENE_RECT_GET (size, width)

/**
 * graphene_rect_get_height:
 * @r: a #graphene_rect_t
 *
 * Retrieves the normalized height of the given rectangle.
 *
 * Returns: the height
 *
 * Since: 1.0
 */
GRAPHENE_RECT_GET (size, height)

#undef GRAPHENE_RECT_GET

/**
 * graphene_rect_union:
 * @a: a #graphene_rect_t
 * @b: a #graphene_rect_t
 * @res: (out caller-allocates): return location for a #graphene_rect_t
 *
 * Computes the union of the two given rectangles.
 *
 * Since: 1.0
 */
void
graphene_rect_union (const graphene_rect_t *a,
                     const graphene_rect_t *b,
                     graphene_rect_t       *res)
{
  graphene_rect_t ra, rb;

  ra = *a;
  rb = *b;

  graphene_rect_normalize_internal (&ra);
  graphene_rect_normalize_internal (&rb);

  res->origin.x = MIN (ra.origin.x, rb.origin.x);
  res->origin.y = MIN (ra.origin.y, rb.origin.y);

  res->size.width = MAX (ra.size.width, rb.size.width);
  res->size.height = MAX (ra.size.height, rb.size.height);
}

/**
 * graphene_rect_intersection:
 * @a: a #graphene_rect_t
 * @b: a #graphene_rect_t
 * @res: (out caller-allocates) (optional): return location for
 *   a #graphene_rect_t
 *
 * Computes the intersection of the two given rectangles.
 *
 * If the two rectangles do not intersect, @res will contain
 * a degenerate rectangle with origin in (0, 0) and a size of 0.
 *
 * Returns: %true if the two rectangles intersect
 *
 * Since: 1.0
 */
bool
graphene_rect_intersection (const graphene_rect_t *a,
                            const graphene_rect_t *b,
                            graphene_rect_t       *res)
{
  graphene_rect_t ra, rb;
  float x_1, y_1, x_2, y_2;

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

      return false;
    }

  if (res != NULL)
    graphene_rect_init (res, x_1, y_1, x_2 - x_1, y_2 - y_1);

  return true;
}

/**
 * graphene_rect_contains_point:
 * @r: a #graphene_rect_t
 * @p: a #graphene_point_t
 *
 * Checks whether a #graphene_rect_t contains the given coordinates.
 *
 * Returns: %true if the rectangle contains the point
 *
 * Since: 1.0
 */
bool
graphene_rect_contains_point (const graphene_rect_t  *r,
                              const graphene_point_t *p)
{
  graphene_rect_t rr;

  rr = *r;
  graphene_rect_normalize_internal (&rr);

  return p->x >= rr.origin.x &&
         p->y >= rr.origin.y &&
         p->x <= (rr.origin.x + rr.size.width) &&
         p->y <= (rr.origin.y + rr.size.height);
}

/**
 * graphene_rect_contains_rect:
 * @a: a #graphene_rect_t
 * @b: a #graphene_rect_t
 *
 * Checks whether a #graphene_rect_t fully contains the given
 * rectangle.
 *
 * Returns: %true if the rectangle @a fully contains @b
 *
 * Since: 1.0
 */
bool
graphene_rect_contains_rect (const graphene_rect_t *a,
                             const graphene_rect_t *b)
{
  graphene_rect_t res;

  graphene_rect_union (a, b, &res);

  return graphene_rect_equal (a, &res);
}

/**
 * graphene_rect_offset:
 * @r: a #graphene_rect_t
 * @d_x: the horizontal offset
 * @d_y: the vertical offset
 *
 * Offsets the origin by @d_x and @d_y.
 *
 * The size of the rectangle is unchanged.
 *
 * Returns: (transfer none): the offset rectangle
 *
 * Since: 1.0
 */
graphene_rect_t *
graphene_rect_offset (graphene_rect_t *r,
                      float            d_x,
                      float            d_y)
{
  graphene_rect_normalize_internal (r);

  r->origin.x += d_x;
  r->origin.y += d_y;

  return r;
}

/**
 * graphene_rect_inset:
 * @r: a #graphene_rect_t
 * @d_x: the horizontal inset
 * @d_y: the vertical inset
 *
 * Changes the given rectangle to be smaller, or larger depending on the
 * given inset parameters.
 *
 * To create an inset rectangle, use positive @d_x or @d_y values; to
 * create a larger, encompassing rectangle, use negative @d_x or @d_y
 * values.
 *
 * The origin of the rectangle is offset by @d_x and @d_y, while the size
 * is adjusted by (2 * @d_x, 2 * @d_y). If @d_x and @d_y are positive
 * values, the size of the rectangle is decreased; if @d_x and @d_y are
 * negative values, the size of the rectangle is increased.
 *
 * If the size of the resulting inset rectangle has a negative width or
 * height then the size will be set to zero.
 *
 * Returns: (transfer none): the inset rectangle
 *
 * Since: 1.0
 */
graphene_rect_t *
graphene_rect_inset (graphene_rect_t *r,
                     float            d_x,
                     float            d_y)
{
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

  return r;
}

/**
 * graphene_rect_round_to_pixel:
 * @r: a #graphene_rect_t
 *
 * Rounds the origin and the size of the given rectangle to
 * their nearest integer values; the rounding is guaranteed
 * to be large enough to contain the original rectangle.
 *
 * Returns: (transfer none): the pixel-aligned rectangle.
 *
 * Since: 1.0
 */
graphene_rect_t *
graphene_rect_round_to_pixel (graphene_rect_t *r)
{
  graphene_rect_normalize_internal (r);

  r->origin.x = floorf (r->origin.x);
  r->origin.y = floorf (r->origin.y);

  r->size.width = ceilf (r->size.width);
  r->size.height = ceilf (r->size.height);

  return r;
}

/**
 * graphene_rect_interpolate:
 * @a: a #graphene_rect_t
 * @b: a #graphene_rect_t
 * @factor: the linear interpolation factor
 * @res: (out caller-allocates): return location for the
 *   interpolated rectangle
 *
 * Linearly interpolates the origin and size of the two given
 * rectangles.
 *
 * Since: 1.0
 */
void
graphene_rect_interpolate (const graphene_rect_t *a,
                           const graphene_rect_t *b,
                           double                 factor,
                           graphene_rect_t       *res)
{
  graphene_rect_t ra, rb;

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
