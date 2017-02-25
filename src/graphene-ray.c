/* graphene-ray.c: A ray emitted from an origin in a given direction
 *
 * Copyright 2015  Emmanuele Bassi
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
 * SECTION:graphene-ray
 * @Title: Ray
 * @Short_Description: A ray emitted from an origin in a given direction
 *
 * #graphene_ray_t is a structure representing a ray emitted by an origin,
 * identified by a point in 3D space, in a given direction, identified
 * by a vector with 3 components.
 */

#include "graphene-private.h"

#include "graphene-ray.h"

#include "graphene-alloc-private.h"
#include "graphene-plane.h"
#include "graphene-point3d.h"
#include "graphene-vec3.h"

#include <math.h>
#include <float.h>

/**
 * graphene_ray_alloc: (constructor)
 *
 * Allocates a new #graphene_ray_t structure.
 *
 * The contents of the returned structure are undefined.
 *
 * Returns: (transfer full): the newly allocated #graphene_ray_t.
 *   Use graphene_ray_free() to free the resources allocated by
 *   this function
 *
 * Since: 1.4
 */
graphene_ray_t *
graphene_ray_alloc (void)
{
  return graphene_aligned_alloc (sizeof (graphene_ray_t), 1, 16);
}

/**
 * graphene_ray_free:
 * @r: a #graphene_ray_t
 *
 * Frees the resources allocated by graphene_ray_alloc().
 *
 * Since: 1.4
 */
void
graphene_ray_free (graphene_ray_t *r)
{
  graphene_aligned_free (r);
}

/**
 * graphene_ray_init:
 * @r: the #graphene_ray_t to initialize
 * @origin: (nullable): the origin of the ray
 * @direction: (nullable): the direction vector
 *
 * Initializes the given #graphene_ray_t using the given @origin
 * and @direction values.
 *
 * Returns: (transfer none): the initialized ray
 *
 * Since: 1.4
 */
graphene_ray_t *
graphene_ray_init (graphene_ray_t           *r,
                   const graphene_point3d_t *origin,
                   const graphene_vec3_t    *direction)
{
  if (origin != NULL)
    graphene_point3d_to_vec3 (origin, &r->origin);
  else
    graphene_vec3_init_from_vec3 (&r->origin, graphene_vec3_zero ());

  if (direction != NULL)
    graphene_vec3_normalize (direction, &r->direction);
  else
    graphene_vec3_init_from_vec3 (&r->direction, graphene_vec3_zero ());

  return r;
}

/**
 * graphene_ray_init_from_ray:
 * @r: the #graphene_ray_t to initialize
 * @src: a #graphene_ray_t
 *
 * Initializes the given #graphene_ray_t using the origin and direction
 * values of another #graphene_ray_t.
 *
 * Returns: (transfer none): the initialized ray
 *
 * Since: 1.4
 */
graphene_ray_t *
graphene_ray_init_from_ray (graphene_ray_t       *r,
                            const graphene_ray_t *src)
{
  return graphene_ray_init_from_vec3 (r, &src->origin, &src->direction);
}

/**
 * graphene_ray_init_from_vec3:
 * @r: the #graphene_ray_t to initialize
 * @origin: (nullable): a #graphene_vec3_t
 * @direction: (nullable): a #graphene_vec3_t
 *
 * Initializes the given #graphene_ray_t using the given vectors.
 *
 * Returns: (transfer none): the initialized ray
 *
 * Since: 1.4
 */
graphene_ray_t *
graphene_ray_init_from_vec3 (graphene_ray_t        *r,
                             const graphene_vec3_t *origin,
                             const graphene_vec3_t *direction)
{
  if (origin != NULL)
    graphene_vec3_init_from_vec3 (&r->origin, origin);
  else
    graphene_vec3_init_from_vec3 (&r->origin, graphene_vec3_zero ());

  if (direction != NULL)
    graphene_vec3_normalize (direction, &r->direction);
  else
    graphene_vec3_init_from_vec3 (&r->direction, graphene_vec3_zero ());

  return r;
}

/**
 * graphene_ray_get_origin:
 * @r: a #graphene_ray_t
 * @origin: (out caller-allocates): return location for the origin
 *
 * Retrieves the origin of the given #graphene_ray_t.
 *
 * Since: 1.4
 */
void
graphene_ray_get_origin (const graphene_ray_t *r,
                         graphene_point3d_t   *origin)
{
  graphene_point3d_init_from_vec3 (origin, &r->origin);
}

/**
 * graphene_ray_get_direction:
 * @r: a #graphene_ray_t
 * @direction: (out caller-allocates): return location for the direction
 *
 * Retrieves the direction of the given #graphene_ray_t.
 *
 * Since: 1.4
 */
void
graphene_ray_get_direction (const graphene_ray_t *r,
                            graphene_vec3_t      *direction)
{
  graphene_vec3_init_from_vec3 (direction, &r->direction);
}

/**
 * graphene_ray_get_position_at:
 * @r: a #graphene_ray_t
 * @t: the distance along the ray
 * @position: (out caller-allocates): return location for the position
 *
 * Retrieves the coordinates of a point at the distance @t along the
 * given #graphene_ray_t.
 *
 * Since: 1.4
 */
void
graphene_ray_get_position_at (const graphene_ray_t *r,
                              float                 t,
                              graphene_point3d_t   *position)
{
  graphene_vec3_t res;

  graphene_vec3_scale (&r->direction, t, &res);
  graphene_vec3_add (&res, &r->origin, &res);

  graphene_point3d_init_from_vec3 (position, &res);
}

/**
 * graphene_ray_get_distance_to_point:
 * @r: a #graphene_ray_t
 * @p: a #graphene_point3d_t
 *
 * Computes the distance from the origin of the given ray to the given point.
 *
 * Returns: the distance of the point
 *
 * Since: 1.4
 */
float
graphene_ray_get_distance_to_point (const graphene_ray_t     *r,
                                    const graphene_point3d_t *p)
{
  graphene_vec3_t point;
  graphene_vec3_t tmp;
  float distance;

  graphene_point3d_to_vec3 (p, &point);

  graphene_vec3_subtract (&point, &r->origin, &tmp);
  distance = graphene_vec3_dot (&tmp, &r->direction);

  /* the point is behind the ray */
  if (distance < 0)
    {
      graphene_vec3_subtract (&r->origin, &point, &tmp);
      return graphene_vec3_length (&tmp);
    }

  /* get the position on the ray at the given distance */
  graphene_vec3_scale (&r->direction, distance, &tmp);
  graphene_vec3_add (&tmp, &r->origin, &tmp);

  graphene_vec3_subtract (&tmp, &point, &tmp);

  return graphene_vec3_length (&tmp);
}

/**
 * graphene_ray_get_distance_to_plane:
 * @r: a #graphene_ray_t
 * @p: a #graphene_plane_t
 *
 * Computes the distance of the origin of the given #graphene_ray_t from the
 * given plane.
 *
 * If the ray does not intersect the plane, this function returns `INFINITY`.
 *
 * Returns: the distance of the origin of the ray from the plane
 *
 * Since: 1.4
 */
float
graphene_ray_get_distance_to_plane (const graphene_ray_t   *r,
                                    const graphene_plane_t *p)
{
  float denom, t;

  denom = graphene_vec3_dot (&p->normal, &r->direction);
  if (fabsf (denom) < GRAPHENE_FLOAT_EPSILON)
    {
      graphene_point3d_t tmp;

      /* If the ray is coplanar, return 0 */
      graphene_point3d_init_from_vec3 (&tmp, &r->origin);
      if (fabsf (graphene_plane_distance (p, &tmp)) < GRAPHENE_FLOAT_EPSILON)
        return 0.f;

      return INFINITY;
    }

  t = -1.f * (graphene_vec3_dot (&r->origin, &p->normal) + p->constant) / denom;
  if (t >= 0.f)
    return t;

  return INFINITY;
}

static bool
ray_equal (const void *p1,
           const void *p2)
{
  const graphene_ray_t *a = p1;
  const graphene_ray_t *b = p2;

  return graphene_vec3_equal (&a->origin, &b->origin) &&
         graphene_vec3_equal (&a->direction, &b->direction);
}

/**
 * graphene_ray_equal:
 * @a: a #graphene_ray_t
 * @b: a #graphene_ray_t
 *
 * Checks whether the two given #graphene_ray_t are equal.
 *
 * Returns: `true` if the given rays are equal
 *
 * Since: 1.4
 */
bool
graphene_ray_equal (const graphene_ray_t *a,
                    const graphene_ray_t *b)
{
  return graphene_pointer_equal (a, b, ray_equal);
}

/**
 * graphene_ray_get_closest_point_to_point:
 * @r: a #graphene_ray_t
 * @p: a #graphene_point3d_t
 * @res: (out caller-allocates): return location for the closest point3d
 *
 * Computes the point on the given #graphene_ray_t that is closest to the
 * given point @p.
 *
 * Since: 1.4
 */
void
graphene_ray_get_closest_point_to_point (const graphene_ray_t     *r,
                                         const graphene_point3d_t *p,
                                         graphene_point3d_t       *res)
{
  graphene_vec3_t point, result;
  float distance;

  graphene_point3d_to_vec3 (p, &point);
  graphene_vec3_subtract (&point, &r->origin, &result);

  distance = graphene_vec3_dot (&result, &r->direction);
  if (distance < 0)
    graphene_vec3_init_from_vec3 (&result, &r->origin);
  else
    {
      graphene_vec3_scale (&r->direction, distance, &result);
      graphene_vec3_add (&result, &r->origin, &result);
    }

  graphene_point3d_init_from_vec3 (res, &result);
}
