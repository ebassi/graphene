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
