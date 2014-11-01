/* graphene-box.c: A box
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
 * SECTION:graphene-box
 * @Title: Box
 * @Short_Description: A box
 *
 * #graphene_box_t provides a representation of a box using the coordinates
 * of its minimum and maximum vertices.
 */

#include "graphene-private.h"

#include "graphene-box.h"
#include "graphene-vec3.h"
#include "graphene-simd4f.h"

#include <math.h>

/**
 * graphene_box_alloc: (constructor)
 *
 * Allocates a new #graphene_box_t.
 *
 * The contents of the returned structure are undefined.
 *
 * Returns: (transfer full): the newly allocated #graphene_box_t structure.
 *   Use graphene_box_free() to free the resources allocated by this function
 *
 * Since: 1.2
 */
graphene_box_t *
graphene_box_alloc (void)
{
  return calloc (1, sizeof (graphene_box_t));
}

/**
 * graphene_box_free:
 * @box: a #graphene_box_t
 *
 * Frees the resources allocated by graphene_box_alloc().
 *
 * Since: 1.2
 */
void
graphene_box_free (graphene_box_t *box)
{
  free (box);
}

/**
 * graphene_box_init:
 * @box: the #graphene_box_t to initialize
 * @min: the coordinates of the minimum vertex
 * @max: the coordinates of the maximum vertex
 *
 * Initializes the given #graphene_box_t with two vertices.
 *
 * Returns: (transfer none): the initialized #graphene_box_t
 *
 * Since: 1.2
 */
graphene_box_t *
graphene_box_init (graphene_box_t           *box,
                   const graphene_point3d_t *min,
                   const graphene_point3d_t *max)
{
  box->min = *min;
  box->max = *max;

  return box;
}

/**
 * graphene_box_init_from_points:
 * @box: the #graphene_box_t to initialize
 * @n_points: the number #graphene_point3d_t in the @points array
 * @points: (array length=n_points): an array of #graphene_point3d_t
 *
 * Initializes the given #graphene_box_t with the given array
 * of vertices.
 *
 * If @n_points is 0, the returned box is initialized with
 * graphene_box_empty().
 *
 * Returns: (transfer none): the initialized #graphene_box_t
 *
 * Since: 1.2
 */
graphene_box_t *
graphene_box_init_from_points (graphene_box_t            *box,
                               unsigned int               n_points,
                               const graphene_point3d_t **points)
{
  unsigned int i;

  graphene_box_init_from_box (box, graphene_box_empty ());

  for (i = 0; i < n_points; i++)
    graphene_box_expand (box, points[i], box);

  return box;
}

/**
 * graphene_box_init_from_box:
 * @box: the #graphene_box_t to initialize
 * @src: a #graphene_box_t
 *
 * Initializes the given #graphene_box_t with the vertices of
 * another #graphene_box_t.
 *
 * Returns: (transfer none): the initialized #graphene_box_t
 *
 * Since: 1.2
 */
graphene_box_t *
graphene_box_init_from_box (graphene_box_t       *box,
                            const graphene_box_t *src)
{
  box->min = src->min;
  box->max = src->max;

  return box;
}

/**
 * graphene_box_init_from_vec3:
 * @box: the #graphene_box_t to initialize
 * @min: the coordinates of the minimum vertex
 * @max: the coordinates of the maximum vertex
 *
 * Initializes the given #graphene_box_t with two vertices
 * stored inside #graphene_vec3_t.
 *
 * Returns: (transfer none): the initialized #graphene_box_t
 *
 * Since: 1.2
 */
graphene_box_t *
graphene_box_init_from_vec3 (graphene_box_t        *box,
                             const graphene_vec3_t *min,
                             const graphene_vec3_t *max)
{
  box->min.x = graphene_vec3_get_x (min);
  box->min.y = graphene_vec3_get_y (min);
  box->min.z = graphene_vec3_get_z (min);

  box->max.x = graphene_vec3_get_x (max);
  box->max.y = graphene_vec3_get_y (max);
  box->max.z = graphene_vec3_get_z (max);

  return box;
}

/**
 * graphene_box_expand:
 * @box: a #graphene_box_t
 * @point: the coordinates of the point to include
 * @res: (out caller-allocates): return location for the expanded box
 *
 * Expands @box to include the coordinates at @point.
 *
 * Since: 1.2
 */
void
graphene_box_expand (const graphene_box_t     *box,
                     const graphene_point3d_t *point,
                     graphene_box_t           *res)
{
  graphene_vec3_t min, max, p;

  graphene_vec3_init (&min, box->min.x, box->min.y, box->min.z);
  graphene_vec3_init (&max, box->max.x, box->max.y, box->max.z);
  graphene_vec3_init (&p, point->x, point->y, point->z);

  graphene_vec3_min (&min, &p, &min);
  graphene_vec3_max (&max, &p, &max);

  res->min.x = graphene_vec3_get_x (&min);
  res->min.y = graphene_vec3_get_y (&min);
  res->min.z = graphene_vec3_get_z (&min);

  res->max.x = graphene_vec3_get_x (&max);
  res->max.y = graphene_vec3_get_y (&max);
  res->max.z = graphene_vec3_get_z (&max);
}

/**
 * graphene_box_union:
 * @a: a #graphene_box_t
 * @b: the box to union to @a
 * @res: (out caller-allocates): return location for the result
 *
 * Unions the two given #graphene_box_t.
 *
 * Since: 1.2
 */
void
graphene_box_union (const graphene_box_t *a,
                    const graphene_box_t *b,
                    graphene_box_t       *res)
{
  graphene_box_expand (a, &b->min, res);
  graphene_box_expand (a, &b->max, res);
}

/**
 * graphene_box_intersection:
 * @a: a #graphene_box_t
 * @b: a #graphene_box_t
 * @res: (out caller-allocates) (optional): return location for the result
 *
 * Intersects the two given #graphene_box_t.
 *
 * If the two boxes do not intersect, @res will contain a degenerate box
 * initialized with graphene_box_empty().
 *
 * Returns: true if the two boxes intersect
 *
 * Since: 1.2
 */
bool
graphene_box_intersection (const graphene_box_t *a,
                           const graphene_box_t *b,
                           graphene_box_t       *res)
{
  graphene_vec3_t a_min, a_max;
  graphene_vec3_t b_min, b_max;
  graphene_vec3_t min, max;

  graphene_vec3_init (&a_min, a->min.x, a->min.y, a->min.z);
  graphene_vec3_init (&a_max, a->max.x, a->max.y, a->max.z);
  graphene_vec3_init (&b_min, b->min.x, b->min.y, b->min.z);
  graphene_vec3_init (&b_max, b->max.x, b->max.y, b->max.z);

  graphene_vec3_max (&a_min, &b_min, &min);
  graphene_vec3_min (&a_max, &b_max, &max);

  /* we cheat a bit, and access the SIMD field directly */
  if (graphene_simd4f_cmp_lt (min.value, max.value))
    {
      if (res != NULL)
        graphene_box_init_from_vec3 (res, &min, &max);

      return true;
    }

  if (res != NULL)
    graphene_box_init_from_box (res, graphene_box_empty ());

  return false;
}

/**
 * graphene_box_get_width:
 * @box: a #graphene_box_t
 *
 * Retrieves the size of the @box on the X axis.
 *
 * Returns: the width of the box
 *
 * Since: 1.2
 */
float
graphene_box_get_width (const graphene_box_t *box)
{
  return box->max.x - box->min.x;
}

/**
 * graphene_box_get_height:
 * @box: a #graphene_box_t
 *
 * Retrieves the size of the @box on the Y axis.
 *
 * Returns: the height of the box
 *
 * Since: 1.2
 */
float
graphene_box_get_height (const graphene_box_t *box)
{
  return box->max.y - box->min.y;
}

/**
 * graphene_box_get_depth:
 * @box: a #graphene_box_t
 *
 * Retrieves the size of the @box on the Z axis.
 *
 * Returns: the depth of the box
 *
 * Since: 1.2
 */
float
graphene_box_get_depth (const graphene_box_t *box)
{
  return box->max.z - box->min.z;
}

/**
 * graphene_box_get_center:
 * @box: a #graphene_box_t
 * @center: (out caller-allocates): return location for the coordinates of
 *   the center
 *
 * Retrieves the coordinates of the center of a #graphene_box_t.
 *
 * Since: 1.2
 */
void
graphene_box_get_center (const graphene_box_t *box,
                         graphene_point3d_t   *center)
{
  graphene_vec3_t min, max, res;

  graphene_vec3_init (&min, box->min.x, box->min.y, box->min.z);
  graphene_vec3_init (&max, box->max.x, box->max.y, box->max.z);

  graphene_vec3_subtract (&max, &min, &res);
  graphene_vec3_scale (&res, 0.5f, &res);

  center->x = graphene_vec3_get_x (&res);
  center->y = graphene_vec3_get_y (&res);
  center->z = graphene_vec3_get_z (&res);
}

/**
 * graphene_box_contains_point:
 * @box: a #graphene_box_t
 * @point: the coordinates to check
 *
 * Checks whether @box contains the given @point.
 *
 * Returns: %true if the point is contained in the given box
 *
 * Since: 1.2
 */
bool
graphene_box_contains_point (const graphene_box_t     *box,
                             const graphene_point3d_t *point)
{
  if (point->x < box->min.x ||
      point->y < box->min.y ||
      point->z < box->min.z)
    return false;

  if (point->x > box->max.x ||
      point->y > box->max.y ||
      point->z > box->max.z)
    return false;

  return true;
}

/**
 * graphene_box_contains_box:
 * @a: a #graphene_box_t
 * @b: a #graphene_box_t
 *
 * Checks whether the #graphene_box_t @a contains the given
 * #graphene_box_t @b.
 *
 * Returns: %true if the box is contained in the given box
 *
 * Since: 1.2
 */
bool
graphene_box_contains_box (const graphene_box_t *a,
                           const graphene_box_t *b)
{
  return graphene_box_contains_point (a, &b->min) &&
         graphene_box_contains_point (a, &b->max);
}

/**
 * graphene_box_equal:
 * @a: a #graphene_box_t
 * @b: a #graphene_box_t
 *
 * Checks whether the two given boxes are equal.
 *
 * Returns: %true if the boxes are equal
 *
 * Since: 1.2
 */
bool
graphene_box_equal (const graphene_box_t *a,
                    const graphene_box_t *b)
{
  if (a == b)
    return true;

  if (a == NULL || b == NULL)
    return false;

  return graphene_point3d_equal (&a->min, &b->min) &&
         graphene_point3d_equal (&a->max, &b->max);
}

enum {
  BOX_ZERO,
  BOX_ONE,
  BOX_MINUS_ONE,
  BOX_ONE_MINUS_ONE,
  BOX_INFINITY,
  BOX_EMPTY
};

static const graphene_box_t graphene_boxes[] = {
  [BOX_ZERO] = {
    /* .min = */ GRAPHENE_POINT3D_INIT_ZERO,
    /* .max = */ GRAPHENE_POINT3D_INIT_ZERO,
  },

  [BOX_ONE] = {
    /* .min = */ GRAPHENE_POINT3D_INIT_ZERO,
    /* .max = */ GRAPHENE_POINT3D_INIT (1.f, 1.f, 1.f),
  },

  [BOX_MINUS_ONE] = {
    /* .min = */ GRAPHENE_POINT3D_INIT (-1.f, -1.f, -1.f),
    /* .max = */ GRAPHENE_POINT3D_INIT_ZERO,
  },

  [BOX_ONE_MINUS_ONE] = {
    /* .min = */ GRAPHENE_POINT3D_INIT (-1.f, -1.f, -1.f),
    /* .max = */ GRAPHENE_POINT3D_INIT (1.f, 1.f, 1.f),
  },

  [BOX_INFINITY] = {
    /* .min = */ GRAPHENE_POINT3D_INIT (-INFINITY, -INFINITY, -INFINITY),
    /* .max = */ GRAPHENE_POINT3D_INIT (INFINITY, INFINITY, INFINITY),
  },

  [BOX_EMPTY] = {
    /* .min = */ GRAPHENE_POINT3D_INIT (INFINITY, INFINITY, INFINITY),
    /* .max = */ GRAPHENE_POINT3D_INIT (-INFINITY, -INFINITY, -INFINITY),
  },
};

/**
 * graphene_box_zero:
 *
 * A #graphene_box_t with both the minimum and maximum vertices set at (0, 0, 0).
 *
 * The returned value is owned by Graphene and should not be modified or freed.
 *
 * Returns: (transfer none): a #graphene_box_t
 *
 * Since: 1.2
 */
const graphene_box_t *
graphene_box_zero (void)
{
  return &(graphene_boxes[BOX_ZERO]);
}

/**
 * graphene_box_one:
 *
 * A #graphene_box_t with the minimum vertex set at (0, 0, 0) and the
 * maximum vertex set at (1, 1, 1).
 *
 * The returned value is owned by Graphene and should not be modified or freed.
 *
 * Returns: (transfer none): a #graphene_box_t
 *
 * Since: 1.2
 */
const graphene_box_t *
graphene_box_one (void)
{
  return &(graphene_boxes[BOX_ONE]);
}

/**
 * graphene_box_minus_one:
 *
 * A #graphene_box_t with the minimum vertex set at (-1, -1, -1) and the
 * maximum vertex set at (0, 0, 0).
 *
 * The returned value is owned by Graphene and should not be modified or freed.
 *
 * Returns: (transfer none): a #graphene_box_t
 *
 * Since: 1.2
 */
const graphene_box_t *
graphene_box_minus_one (void)
{
  return &(graphene_boxes[BOX_MINUS_ONE]);
}

/**
 * graphene_box_one_minus_one:
 *
 * A #graphene_box_t with the minimum vertex set at (-1, -1, -1) and the
 * maximum vertex set at (1, 1, 1).
 *
 * The returned value is owned by Graphene and should not be modified or freed.
 *
 * Returns: (transfer none): a #graphene_box_t
 *
 * Since: 1.2
 */
const graphene_box_t *
graphene_box_one_minus_one (void)
{
  return &(graphene_boxes[BOX_ONE_MINUS_ONE]);
}

/**
 * graphene_box_infinite:
 *
 * A degenerate #graphene_box_t that cannot be expanded.
 *
 * The returned value is owned by Graphene and should not be modified or freed.
 *
 * Returns: (transfer none): a #graphene_box_t
 *
 * Since: 1.2
 */
const graphene_box_t *
graphene_box_infinite (void)
{
  return &(graphene_boxes[BOX_INFINITY]);
}

/**
 * graphene_box_empty:
 *
 * A degenerate #graphene_box_t that can only be expanded.
 *
 * The returned value is owned by Graphene and should not be modified or freed.
 *
 * Returns: (transfer none): a #graphene_box_t
 *
 * Since: 1.2
 */
const graphene_box_t *
graphene_box_empty (void)
{
  return &(graphene_boxes[BOX_EMPTY]);
}
