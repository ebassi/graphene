/* graphene-frustum.c: A 3D field of view
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
 * SECTION:graphene-frustum
 * @Title: Frustum
 * @Short_Description: A 3D field of view
 *
 * A #graphene_frustum_t represents a volume of space delimited by planes. It
 * is usually employed to represent the field of view of a camera, and can be
 * used to determine whether an object falls within that view, to efficiently
 * remove invisible objects from the render process.
 */

#include "graphene-private.h"

#include "graphene-frustum.h"

#include "graphene-alloc-private.h"
#include "graphene-matrix.h"
#include "graphene-sphere.h"
#include "graphene-point3d.h"
#include "graphene-vec4.h"

#define N_CLIP_PLANES 6

/**
 * graphene_frustum_alloc: (constructor)
 *
 * Allocates a new #graphene_frustum_t structure.
 *
 * The contents of the returned structure are undefined.
 *
 * Returns: (transfer full): the newly allocated #graphene_frustum_t
 *   structure. Use graphene_frustum_free() to free the resources
 *   allocated by this function.
 *
 * Since: 1.2
 */
graphene_frustum_t *
graphene_frustum_alloc (void)
{
  return calloc (1, sizeof (graphene_frustum_t));
}

/**
 * graphene_frustum_free:
 * @f: a #graphene_frustum_t
 *
 * Frees the resources allocated by graphene_frustum_alloc().
 *
 * Since: 1.2
 */
void
graphene_frustum_free (graphene_frustum_t *f)
{
  free (f);
}

/**
 * graphene_frustum_init:
 * @f: the #graphene_frustum_t to initialize
 * @p0: a clipping plane
 * @p1: a clipping plane
 * @p2: a clipping plane
 * @p3: a clipping plane
 * @p4: a clipping plane
 * @p5: a clipping plane
 *
 * Initializes the given #graphene_frustum_t using the provided
 * clipping planes.
 *
 * Returns: (transfer none): the initialized frustum
 *
 * Since: 1.2
 */
graphene_frustum_t *
graphene_frustum_init (graphene_frustum_t     *f,
                       const graphene_plane_t *p0,
                       const graphene_plane_t *p1,
                       const graphene_plane_t *p2,
                       const graphene_plane_t *p3,
                       const graphene_plane_t *p4,
                       const graphene_plane_t *p5)
{
  graphene_plane_init_from_plane (&f->planes[0], p0);
  graphene_plane_init_from_plane (&f->planes[1], p1);
  graphene_plane_init_from_plane (&f->planes[2], p2);
  graphene_plane_init_from_plane (&f->planes[3], p3);
  graphene_plane_init_from_plane (&f->planes[4], p4);
  graphene_plane_init_from_plane (&f->planes[5], p5);

  return f;
}

/**
 * graphene_frustum_init_from_frustum:
 * @f: the #graphene_frustum_t to initialize
 * @src: a #graphene_frustum_t
 *
 * Initializes the given #graphene_frustum_t using the clipping
 * planes of another #graphene_frustum_t.
 *
 * Returns: (transfer none): the initialized frustum
 *
 * Since: 1.2
 */
graphene_frustum_t *
graphene_frustum_init_from_frustum (graphene_frustum_t       *f,
                                    const graphene_frustum_t *src)
{
  int i;

  for (i = 0; i < N_CLIP_PLANES; i++)
    graphene_plane_init_from_plane (&f->planes[i], &src->planes[i]);

  return f;
}

/**
 * graphene_frustum_init_from_matrix:
 * @f: a #graphene_frustum_t
 * @matrix: a #graphene_matrix_t
 *
 * Initializes a #graphene_frustum_t using the given @matrix.
 *
 * Returns: (transfer none): the initialized frustum
 *
 * Since: 1.2
 */
graphene_frustum_t *
graphene_frustum_init_from_matrix (graphene_frustum_t      *f,
                                   const graphene_matrix_t *matrix)
{
  graphene_vec4_t r1, r2, r3, r4, t;
  graphene_plane_t p;
  graphene_matrix_t m;

  graphene_matrix_transpose (matrix, &m);

  graphene_matrix_get_row (&m, 0, &r1);
  graphene_matrix_get_row (&m, 1, &r2);
  graphene_matrix_get_row (&m, 2, &r3);
  graphene_matrix_get_row (&m, 3, &r4);

  graphene_vec4_subtract (&r4, &r1, &t);
  graphene_plane_init_from_vec4 (&p, &t);
  graphene_plane_normalize (&p, &f->planes[0]);

  graphene_vec4_add (&r4, &r1, &t);
  graphene_plane_init_from_vec4 (&p, &t);
  graphene_plane_normalize (&p, &f->planes[1]);

  graphene_vec4_subtract (&r4, &r2, &t);
  graphene_plane_init_from_vec4 (&p, &t);
  graphene_plane_normalize (&p, &f->planes[2]);

  graphene_vec4_add (&r4, &r2, &t);
  graphene_plane_init_from_vec4 (&p, &t);
  graphene_plane_normalize (&p, &f->planes[3]);

  graphene_vec4_subtract (&r4, &r3, &t);
  graphene_plane_init_from_vec4 (&p, &t);
  graphene_plane_normalize (&p, &f->planes[4]);

  graphene_vec4_add (&r4, &r3, &t);
  graphene_plane_init_from_vec4 (&p, &t);
  graphene_plane_normalize (&p, &f->planes[5]);

  return f;
}

/**
 * graphene_frustum_contains_point:
 * @f: a #graphene_frustum_t
 * @point: a #graphene_point3d_t
 *
 * Checks whether a point is inside the volume defined by the given
 * #graphene_frustum_t.
 *
 * Returns: `true` if the point is inside the frustum
 *
 * Since: 1.2
 */
bool
graphene_frustum_contains_point (const graphene_frustum_t *f,
                                 const graphene_point3d_t *point)
{
  unsigned int i;

  if (point == NULL)
    return false;

  for (i = 0; i < N_CLIP_PLANES; i++)
    {
      const graphene_plane_t *p = &f->planes[i];

      if (graphene_plane_distance (p, point) < 0)
        return false;
    }

  return true;
}

/**
 * graphene_frustum_intersects_sphere:
 * @f: a #graphene_frustum_t
 * @sphere: a #graphene_sphere_t
 *
 * Checks whether the given @sphere intersects a plane of
 * a #graphene_frustum_t.
 *
 * Returns: `true` if the sphere intersects the frustum
 *
 * Since: 1.2
 */
bool
graphene_frustum_intersects_sphere (const graphene_frustum_t *f,
                                    const graphene_sphere_t  *sphere)
{
  graphene_point3d_t center;
  unsigned int i;

  graphene_point3d_init_from_vec3 (&center, &sphere->center);

  for (i = 0; i < N_CLIP_PLANES; i++)
    {
      float distance = graphene_plane_distance (&f->planes[i], &center);

      if (distance < -sphere->radius)
        return false;
    }

  return true;
}
