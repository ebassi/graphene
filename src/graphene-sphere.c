/* graphene-sphere.c: A sphere
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
 * SECTION:graphene-sphere
 * @Title: Sphere
 * @Short_Description: A sphere
 *
 * #graphene_sphere_t provides a representation of a sphere using its center
 * and radius.
 */

#include "graphene-private.h"

#include "graphene-sphere.h"
#include "graphene-point3d.h"
#include "graphene-box.h"

#include <math.h>

/**
 * graphene_sphere_alloc: (constructor)
 *
 * Allocates a new #graphene_sphere_t.
 *
 * The contents of the newly allocated structure are undefined.
 *
 * Returns: (transfer full): the newly allocated #graphene_sphere_t. Use
 *   graphene_sphere_free() to free the resources allocated by this function
 *
 * Since: 1.2
 */
graphene_sphere_t *
graphene_sphere_alloc (void)
{
  return calloc (1, sizeof (graphene_sphere_t));
}

/**
 * graphene_sphere_free:
 * @s: a #graphene_sphere_t
 *
 * Frees the resources allocated by graphene_sphere_alloc().
 *
 * Since: 1.2
 */
void
graphene_sphere_free (graphene_sphere_t *s)
{
  free (s);
}

/**
 * graphene_sphere_init:
 * @s: the #graphene_sphere_t to initialize
 * @center: the coordinates of the center of the sphere
 * @radius: the radius of the sphere
 *
 * Initializes the given #graphene_sphere_t with the give @center and @radius.
 *
 * Returns: (transfer none): the initialized #graphene_sphere_t
 *
 * Since: 1.2
 */
graphene_sphere_t *
graphene_sphere_init (graphene_sphere_t        *s,
                      const graphene_point3d_t *center,
                      float                     radius)
{
  graphene_vec3_init (&s->center, center->x, center->y, center->z);
  s->radius = radius;

  return s;
}

static float
distance_sq (const graphene_vec3_t *p1,
             const graphene_vec3_t *p2)
{
  graphene_vec3_t delta;

  graphene_vec3_subtract (p1, p2, &delta);
  return graphene_vec3_dot (&delta, &delta);
}

/**
 * graphene_sphere_init_from_points:
 * @s: the #graphene_sphere_t to initialize
 * @n_points: the number of #graphene_point3d_t in the @points array
 * @points: (array length=n_points): an arrat of #graphene_point3d_t
 * @center: (optional): the center of the sphere
 *
 * Initializes the given #graphene_sphere_t using the given array
 * of 3D coordinates so that the sphere includes them.
 *
 * The center of the sphere can either be specified, or will be center
 * of the 3D volume that encompasses all @points.
 *
 * Returns: (transfer none): the initialized #graphene_sphere_t
 *
 * Since: 1.2
 */
graphene_sphere_t *
graphene_sphere_init_from_points (graphene_sphere_t         *s,
                                  unsigned int               n_points,
                                  const graphene_point3d_t **points,
                                  const graphene_point3d_t  *center)
{
  float max_radius_sq = 0.f;
  unsigned int i;

  if (center != NULL)
    graphene_vec3_init (&s->center, center->x, center->y, center->z);
  else
    {
      graphene_box_t box;
      graphene_point3d_t c;

      graphene_box_init_from_points (&box, n_points, points);
      graphene_box_get_center (&box, &c);
      graphene_vec3_init (&s->center, c.x, c.y, c.z);
    }

  for (i = 0; i < n_points; i++)
    {
      const graphene_point3d_t *point = points[i];
      graphene_vec3_t p;

      graphene_vec3_init (&p, point->x, point->y, point->z);

      max_radius_sq = fmaxf (max_radius_sq, distance_sq (&s->center, &p));
    }

  s->radius = sqrtf (max_radius_sq);

  return s;
}
