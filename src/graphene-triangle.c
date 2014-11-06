/* graphene-triangle.c: A triangle
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
 * SECTION:graphene-triangle
 * @Title: Triangle
 * @Short_description: A triangle described by 3D points
 *
 * #graphene_triangle_t represents a triangle in 3D space.
 *
 */

#include "graphene-private.h"

#include "graphene-triangle.h"

#include "graphene-plane.h"
#include "graphene-point3d.h"

#include <math.h>

/**
 * graphene_triangle_alloc: (constructor)
 *
 * Allocates a new #graphene_triangle_t.
 *
 * The contents of the returned structure are undefined.
 *
 * Returns: (transfer full): the newly allocated #graphene_triangle_t
 *   structure. Use graphene_triangle_free() to free the resources
 *   allocated by this function
 *
 * Since: 1.2
 */
graphene_triangle_t *
graphene_triangle_alloc (void)
{
  return calloc (1, sizeof (graphene_triangle_t));
}

/**
 * graphene_triangle_free:
 * @t: a #graphene_triangle_t
 *
 * Frees the resources allocated by graphene_triangle_alloc().
 *
 * Since: 1.2
 */
void
graphene_triangle_free (graphene_triangle_t *t)
{
  free (t);
}

/**
 * graphene_triangle_init_from_point3d:
 * @t: the #graphene_triangle_t to initialize
 * @a: a #graphene_point3d_t
 * @b: a #graphene_point3d_t
 * @c: a #graphene_point3d_t
 *
 * Initializes a #graphene_triangle_t using the three given 3D points.
 *
 * Returns: (transfer none): the initialized #graphene_triangle_t
 *
 * Since: 1.2
 */
graphene_triangle_t *
graphene_triangle_init_from_point3d (graphene_triangle_t      *t,
                                     const graphene_point3d_t *a,
                                     const graphene_point3d_t *b,
                                     const graphene_point3d_t *c)
{
  graphene_point3d_to_vec3 (a, &t->a);
  graphene_point3d_to_vec3 (b, &t->b);
  graphene_point3d_to_vec3 (c, &t->c);

  return t;
}

/**
 * graphene_triangle_init_from_vec3:
 * @t: the #graphene_triangle_t to initialize
 * @a: a #graphene_vec3_t
 * @b: a #graphene_vec3_t
 * @c: a #graphene_vec3_t
 *
 * Initializes a #graphene_triangle_t using the three given vectors.
 *
 * Returns: (transfer none): the initialized #graphene_triangle_t
 *
 * Since: 1.2
 */
graphene_triangle_t *
graphene_triangle_init_from_vec3 (graphene_triangle_t   *t,
                                  const graphene_vec3_t *a,
                                  const graphene_vec3_t *b,
                                  const graphene_vec3_t *c)
{
  t->a = *a;
  t->b = *b;
  t->c = *c;

  return t;
}

/**
 * graphene_triangle_get_points:
 * @t: a #graphene_triangle_t
 * @a: (out caller-allocates) (optional): return location for the coordinates
 *   of the first vertex
 * @b: (out caller-allocates) (optional): return location for the coordinates
 *   of the second vertex
 * @c: (out caller-allocates) (optional): return location for the coordinates
 *   of the third vertex
 *
 * Retrieves the three vertices of the given #graphene_triangle_t and returns
 * their coordinates as #graphene_point3d_t.
 *
 * Since: 1.2
 */
void
graphene_triangle_get_points (const graphene_triangle_t *t,
                              graphene_point3d_t        *a,
                              graphene_point3d_t        *b,
                              graphene_point3d_t        *c)
{
  if (a != NULL)
    graphene_point3d_init_from_vec3 (a, &t->a);
  if (b != NULL)
    graphene_point3d_init_from_vec3 (b, &t->b);
  if (c != NULL)
    graphene_point3d_init_from_vec3 (c, &t->c);
}

/**
 * graphene_triangle_get_vertices:
 * @t: a #graphene_triangle_t
 * @a: (out caller-allocates) (optional): return location for the first vertex
 * @b: (out caller-allocates) (optional): return location for the second vertex
 * @c: (out caller-allocates) (optional): return location for the third vertex
 *
 * Retrieves the three vertices of the given #graphene_triangle_t.
 *
 * Since: 1.2
 */
void
graphene_triangle_get_vertices (const graphene_triangle_t *t,
                                graphene_vec3_t           *a,
                                graphene_vec3_t           *b,
                                graphene_vec3_t           *c)
{
  if (a != NULL)
    *a = t->a;
  if (b != NULL)
    *b = t->b;
  if (c != NULL)
    *c = t->c;
}

/**
 * graphene_triangle_get_area:
 * @t: a #graphene_triangle_t
 *
 * Computes the area of the given #graphene_triangle_t.
 *
 * Returns: the area of the triangle
 *
 * Since: 1.2
 */
float
graphene_triangle_get_area (const graphene_triangle_t *t)
{
  graphene_vec3_t v1, v2, tmp;

  graphene_vec3_subtract (&t->c, &t->b, &v1);
  graphene_vec3_subtract (&t->a, &t->b, &v2);

  graphene_vec3_cross (&v1, &v2, &tmp);

  return graphene_vec3_length (&tmp) * .5f;
}

/**
 * graphene_triangle_get_midpoint:
 * @t: a #graphene_triangle_t
 * @res: (out caller-allocates): return location for the coordinates of
 *   the midpoint
 *
 * Computes the coordinates of the midpoint of the given #graphene_triangle_t.
 *
 * Since: 1.2
 */
void
graphene_triangle_get_midpoint (const graphene_triangle_t *t,
                                graphene_point3d_t        *res)
{
  graphene_vec3_t tmp;

  graphene_vec3_add (&t->a, &t->b, &tmp);
  graphene_vec3_add (&tmp, &t->c, &tmp);
  graphene_vec3_scale (&tmp, (1.f / 3.f), &tmp);

  graphene_point3d_init_from_vec3 (res, &tmp);
}

/**
 * graphene_triangle_get_normal:
 * @t: a #graphene_triangle_t
 * @res: (out caller-allocates): return location for the normal vector
 *
 * Computes the normal vector of the given #graphene_triangle_t.
 *
 * Since: 1.2
 */
void
graphene_triangle_get_normal (const graphene_triangle_t *t,
                              graphene_vec3_t           *res)
{
  graphene_vec3_t v1, v2, tmp;
  float length_sq;

  graphene_vec3_subtract (&t->c, &t->b, &v1);
  graphene_vec3_subtract (&t->a, &t->b, &v2);

  graphene_vec3_cross (&v1, &v2, &tmp);

  length_sq = graphene_vec3_dot (&tmp, &tmp);
  if (length_sq > 0)
    graphene_vec3_scale (&tmp, 1.f / sqrtf (length_sq), res);
  else
    graphene_vec3_init_from_vec3 (res, graphene_vec3_zero ());
}

/**
 * graphene_triangle_get_plane:
 * @t: a #graphene_triangle_t
 * @res: (out caller-allocates): return location for the plane
 *
 * Computes the plane based on the vertices of the given #graphene_triangle_t.
 *
 * Since: 1.2
 */
void
graphene_triangle_get_plane (const graphene_triangle_t *t,
                             graphene_plane_t          *res)
{
  graphene_point3d_t a, b, c;

  graphene_point3d_init_from_vec3 (&a, &t->a);
  graphene_point3d_init_from_vec3 (&b, &t->b);
  graphene_point3d_init_from_vec3 (&c, &t->c);

  graphene_plane_init_from_points (res, &a, &b, &c);
}

/**
 * graphene_triangle_contains_point:
 * @t: a #graphene_triangle_t
 * @p: a #graphene_point3d_t
 *
 * Checks whether the given triangle @t contains the point @p.
 *
 * Returns: `true` if the point is inside the triangle
 *
 * Since: 1.2
 */
bool
graphene_triangle_contains_point (const graphene_triangle_t *t,
                                  const graphene_point3d_t  *p)
{
  graphene_vec3_t point, v1, v2, v3;
  float dot_11, dot_12, dot_13;
  float dot_22, dot_23;
  float inv_denom, u, v;

  graphene_point3d_to_vec3 (p, &point);

  /* we use the barycoordinates from the given point to check
   * if the point is inside the triangle.
   *
   * see: http://www.blackpawn.com/texts/pointinpoly/default.html
   */
  graphene_vec3_subtract (&t->c, &t->a, &v1);
  graphene_vec3_subtract (&t->b, &t->a, &v2);
  graphene_vec3_subtract (&point, &t->a, &v3);

  dot_11 = graphene_vec3_dot (&v1, &v1);
  dot_12 = graphene_vec3_dot (&v1, &v2);
  dot_13 = graphene_vec3_dot (&v1, &v3);
  dot_22 = graphene_vec3_dot (&v2, &v2);
  dot_23 = graphene_vec3_dot (&v2, &v3);

  inv_denom = 1.f / (dot_11 * dot_22 - dot_12 * dot_12);
  u = (dot_22 * dot_13 - dot_12 * dot_23) * inv_denom;
  v = (dot_11 * dot_23 - dot_12 * dot_13) * inv_denom;

  return (u >= 0.f) && (v >= 0.f) && (u + v < 1.f);
}

/**
 * graphene_triangle_equal:
 * @a: a #graphene_triangle_t
 * @b: a #graphene_triangle_t
 *
 * Checks whether the two given #graphene_triangle_t are equal.
 *
 * Returns: `true` if the triangles are equal
 *
 * Since: 1.2
 */
bool
graphene_triangle_equal (const graphene_triangle_t *a,
                         const graphene_triangle_t *b)
{
  if (a == b)
    return true;

  if (a == NULL || b == NULL)
    return false;

  return graphene_vec3_equal (&a->a, &b->a) &&
         graphene_vec3_equal (&a->b, &b->b) &&
         graphene_vec3_equal (&a->c, &b->c);
}
