/* graphene-box2d.c: An axis aligned 2D bounding box
 *
 * SPDX-License-Identifier: MIT
 * SPDX-FileCopyrightText: 2023  Emmanuele Bassi
 */

/**
 * SECTION:graphene-box2d
 * @Title: Box 2D
 * @Short_Description: Axis-aligned bounding box
 *
 * #graphene_box2d_t provides a representation of an axis aligned minimum
 * bounding box in two dimensions using the coordinates of its minimum and
 * maximum vertices.
 *
 * #graphene_box2d_t is available since Graphene 1.2.
 */

#include "graphene-private.h"

#include "graphene-box2d.h"

#include "graphene-alloc-private.h"
#include "graphene-point.h"
#include "graphene-rect.h"
#include "graphene-simd4f.h"

#include <math.h>
#include <stdio.h>
#include <string.h>

#ifdef HAVE_PTHREAD
#include <pthread.h>
#include <errno.h>
#endif

/**
 * graphene_box2d_alloc: (constructor)
 *
 * Allocates a new #graphene_box2d_t.
 *
 * The contents of the returned structure are undefined.
 *
 * Returns: (transfer full): the newly allocated #graphene_box2d_t structure.
 *   Use graphene_box2d_free() to free the resources allocated by this function
 *
 * Since: 1.12
 */
graphene_box2d_t *
graphene_box2d_alloc (void)
{
  return graphene_aligned_alloc0 (sizeof (graphene_box2d_t), 1, 16);
}

/**
 * graphene_box2d_free:
 * @box: (transfer full): a #graphene_box2d_t
 *
 * Frees the resources allocated by graphene_box2d_alloc().
 *
 * Since: 1.12
 */
void
graphene_box2d_free (graphene_box2d_t *box)
{
  graphene_aligned_free (box);
}

/**
 * graphene_box2d_init:
 * @box: the #graphene_box2d_t to initialize
 * @min: (nullable): the coordinates of the minimum vertex
 * @max: (nullable): the coordinates of the maximum vertex
 *
 * Initializes the given #graphene_box2d_t with two vertices.
 *
 * Returns: (transfer none): the initialized #graphene_box2d_t
 *
 * Since: 1.12
 */
graphene_box2d_t *
graphene_box2d_init (graphene_box2d_t       *box,
                     const graphene_point_t *min,
                     const graphene_point_t *max)
{
  graphene_vec4_init (&box->minmax,
                      min != NULL ? min->x : 0.f,
                      min != NULL ? min->y : 0.f,
                      max != NULL ? max->x : 0.f,
                      max != NULL ? max->y : 0.f);
  return box;
}

/**
 * graphene_box2d_init_from_points:
 * @box: the #graphene_box2d_t to initialize
 * @n_points: the number #graphene_point_t in the @points array
 * @points: (array length=n_points): an array of #graphene_point_t
 *
 * Initializes the given #graphene_box2d_t with the given array
 * of vertices.
 *
 * If @n_points is 0, the returned box is initialized with
 * graphene_box2d_empty().
 *
 * Returns: (transfer none): the initialized #graphene_box2d_t
 *
 * Since: 1.12
 */
graphene_box2d_t *
graphene_box2d_init_from_points (graphene_box2d_t       *box,
                                 unsigned int            n_points,
                                 const graphene_point_t *points)
{
  graphene_box2d_init_from_box (box, graphene_box2d_empty ());

  for (unsigned int i = 0; i < n_points; i++)
    {
      graphene_vec2_t v;

      graphene_point_to_vec2 (&points[i], &v);
      graphene_box2d_expand_vec2 (box, &v, box);
    }

  return box;
}

/**
 * graphene_box2d_init_from_vectors:
 * @box: the #graphene_box2d_t to initialize
 * @n_vectors: the number #graphene_vec2_t in the @vectors array
 * @vectors: (array length=n_vectors): an array of #graphene_vec2_t
 *
 * Initializes the given #graphene_box2d_t with the given array
 * of vertices.
 *
 * If @n_vectors is 0, the returned box is initialized with
 * graphene_box2d_empty().
 *
 * Returns: (transfer none): the initialized #graphene_box2d_t
 *
 * Since: 1.12
 */
graphene_box2d_t *
graphene_box2d_init_from_vectors (graphene_box2d_t      *box,
                                  unsigned int           n_vectors,
                                  const graphene_vec2_t *vectors)
{
  graphene_box2d_init_from_box (box, graphene_box2d_empty ());

  for (unsigned int i = 0; i < n_vectors; i++)
    graphene_box2d_expand_vec2 (box, &vectors[i], box);

  return box;
}

/**
 * graphene_box2d_init_from_box:
 * @box: the #graphene_box2d_t to initialize
 * @src: a #graphene_box2d_t
 *
 * Initializes the given #graphene_box2d_t with the vertices of
 * another #graphene_box2d_t.
 *
 * Returns: (transfer none): the initialized #graphene_box2d_t
 *
 * Since: 1.12
 */
graphene_box2d_t *
graphene_box2d_init_from_box (graphene_box2d_t       *box,
                              const graphene_box2d_t *src)
{
  box->minmax = src->minmax;

  return box;
}

static inline graphene_box2d_t *
graphene_box2d_init_from_simd4f (graphene_box2d_t        *box,
                                 const graphene_simd4f_t  min,
                                 const graphene_simd4f_t  max)
{
  box->minmax.value = graphene_simd4f_merge_low (min, max);

  return box;
}

/**
 * graphene_box2d_init_from_vec2:
 * @box: the #graphene_box2d_t to initialize
 * @min: (nullable): the coordinates of the minimum vertex
 * @max: (nullable): the coordinates of the maximum vertex
 *
 * Initializes the given #graphene_box2d_t with two vertices
 * stored inside #graphene_vec2_t.
 *
 * Returns: (transfer none): the initialized #graphene_box2d_t
 *
 * Since: 1.12
 */
graphene_box2d_t *
graphene_box2d_init_from_vec2 (graphene_box2d_t      *box,
                               const graphene_vec2_t *min,
                               const graphene_vec2_t *max)
{
  return graphene_box2d_init_from_simd4f (box,
                                          min != NULL ? min->value : graphene_simd4f_init_zero (),
                                          max != NULL ? max->value : graphene_simd4f_init_zero ());
}

/**
 * graphene_box2d_init_from_rect:
 * @box: the #graphene_box2d_t to initialize
 * @src: a #graphene_rect_t
 *
 * Initializes the given #graphene_box2d_t with the origin and
 * size of a #graphene_rect_t.
 *
 * Returns: (transfer none): the initialized #graphene_box2d_t
 *
 * Since: 1.12
 */
graphene_box2d_t *
graphene_box2d_init_from_rect (graphene_box2d_t      *box,
                               const graphene_rect_t *src)
{
  box->minmax.value =
    graphene_simd4f_init (src->origin.x,
                          src->origin.y,
                          src->origin.x + src->size.width,
                          src->origin.y + src->size.height);

  return box;
}

static inline void
graphene_box2d_expand_simd4f (const graphene_box2d_t  *box,
                              const graphene_simd4f_t  v,
                              graphene_box2d_t        *res)
{
  graphene_simd4f_t min_v =
    graphene_simd4f_merge_low (box->minmax.value, graphene_simd4f_init_zero ());
  graphene_simd4f_t max_v =
    graphene_simd4f_merge_high (box->minmax.value, graphene_simd4f_init_zero ());

  min_v = graphene_simd4f_min (min_v, v);
  max_v = graphene_simd4f_max (max_v, v);

  graphene_box2d_init_from_simd4f (res, min_v, max_v);
}

/**
 * graphene_box2d_expand_vec2:
 * @box: a #graphene_box2d_t
 * @vec: the coordinates of the point to include, as a #graphene_vec2_t
 * @res: (out caller-allocates): return location for the expanded box
 *
 * Expands the dimensions of @box to include the coordinates of the
 * given vector.
 *
 * Since: 1.12
 */
void
graphene_box2d_expand_vec2 (const graphene_box2d_t *box,
                            const graphene_vec2_t  *vec,
                            graphene_box2d_t       *res)
{
  graphene_box2d_expand_simd4f (box, vec->value, res);
}

/**
 * graphene_box2d_expand:
 * @box: a #graphene_box2d_t
 * @point: the coordinates of the point to include
 * @res: (out caller-allocates): return location for the expanded box
 *
 * Expands the dimensions of @box to include the coordinates at @point.
 *
 * Since: 1.12
 */
void
graphene_box2d_expand (const graphene_box2d_t *box,
                       const graphene_point_t *point,
                       graphene_box2d_t       *res)
{
  graphene_simd4f_t v = graphene_simd4f_init (point->x, point->y, 0.f, 0.f);

  graphene_box2d_expand_simd4f (box, v, res);
}

/**
 * graphene_box2d_expand_scalar:
 * @box: a #graphene_box2d_t
 * @scalar: a scalar value
 * @res: (out caller-allocates): return location for the expanded box
 *
 * Expands the dimensions of @box by the given @scalar value.
 *
 * If @scalar is positive, the #graphene_box2d_t will grow; if @scalar is
 * negative, the #graphene_box2d_t will shrink.
 *
 * Since: 1.12
 */
void
graphene_box2d_expand_scalar (const graphene_box2d_t *box,
                              float                   scalar,
                              graphene_box2d_t       *res)
{
  graphene_simd4f_t zero = graphene_simd4f_init_zero ();
  graphene_simd4f_t min_v = graphene_simd4f_merge_low (box->minmax.value, zero);
  graphene_simd4f_t max_v = graphene_simd4f_merge_high (box->minmax.value, zero);

  float min = scalar * -1.f;
  float max = scalar;

  min_v = graphene_simd4f_add (min_v, graphene_simd4f_splat (min));
  max_v = graphene_simd4f_add (max_v, graphene_simd4f_splat (max));

  graphene_box2d_init_from_simd4f (res, min_v, max_v);
}

/**
 * graphene_box2d_scale_offset:
 * @box: a #graphene_box2d_t
 * @scale: (nullable): a vector with two scaling factors to be applied to the box
 * @offset: (nullable): the offset to apply to the box
 * @res: (out caller-allocates): the transformed box
 *
 * Applies a scale and an offset to the vertices of the given box.
 *
 * If @scale is %NULL, the box will be scaled by (1.0, 1.0).
 *
 * If @offset is %NULL, the box will be offset by (0.0, 0.0).
 *
 * Since: 1.12
 */
void
graphene_box2d_scale_offset (const graphene_box2d_t *box,
                             const graphene_vec2_t  *scale,
                             const graphene_point_t *offset,
                             graphene_box2d_t       *res)
{
  graphene_simd4f_t scale_full;
  graphene_simd4f_t offset_full;

  if (scale != NULL)
    {
      /* swizzle scale from (x, y, 0, 0) to (0, 0, x, y) */
      graphene_simd4f_t scale_zwxy = graphene_simd4f_shuffle_zwxy (scale->value);
      /* scale is now (x, y, x, y) */
      scale_full = graphene_simd4f_add (scale->value, scale_zwxy);
    }
  else
    scale_full = graphene_simd4f_init (1.f, 1.f, 1.f, 1.f);

  if (offset != NULL)
    offset_full = graphene_simd4f_init (offset->x, offset->y, offset->x, offset->y);
  else
    offset_full = graphene_simd4f_init_zero ();

  res->minmax.value = graphene_simd4f_madd (box->minmax.value, scale_full, offset_full);
}

/**
 * graphene_box2d_to_float:
 * @box: a #graphene_box2d_t
 * @v: (out caller-allocates) (array fixed-size=4): return location
 *   for an array of floating point values with at least 4 elements
 *
 * Stores the minimum and maximum vertices of the given #graphene_box2d_t
 * into an array.
 *
 * The array layout is:
 *
 * - min_x
 * - min_y
 * - max_x
 * - max_y
 *
 * Since: 1.12
 */
void
graphene_box2d_to_float (const graphene_box2d_t *box,
                         float                   v[4])
{
  graphene_simd4f_dup_4f (box->minmax.value, v);
}

/**
 * graphene_box2d_to_rect:
 * @box: a #graphene_box2d_t
 * @rect: (out caller-allocates): the rectangle to initialize
 *
 * Stores the minimum and maximum vertices of the given #graphene_box2d_t
 * into a rectangle of equivalent origin and size.
 *
 * Since: 1.12
 */
void
graphene_box2d_to_rect (const graphene_box2d_t *box,
                        graphene_rect_t        *rect)
{
  graphene_point_t min, max;

  graphene_box2d_get_minmax (box, &min, &max);

  rect->origin.x = min.x;
  rect->origin.y = min.y;
  rect->size.width = max.x - min.x;
  rect->size.height = max.y - min.y;
}

/**
 * graphene_box2d_union:
 * @a: a #graphene_box2d_t
 * @b: the box to union to @a
 * @res: (out caller-allocates): return location for the result
 *
 * Unions the two given #graphene_box2d_t.
 *
 * Since: 1.12
 */
void
graphene_box2d_union (const graphene_box2d_t *a,
                      const graphene_box2d_t *b,
                      graphene_box2d_t       *res)
{
  graphene_simd4f_t zero = graphene_simd4f_init_zero ();

  graphene_simd4f_t v_a, v_b;

  v_a = graphene_simd4f_merge_low (a->minmax.value, zero);
  v_b = graphene_simd4f_merge_low (b->minmax.value, zero);
  graphene_simd4f_t min_v = graphene_simd4f_min (v_a, v_b);

  v_a = graphene_simd4f_merge_high (a->minmax.value, zero);
  v_b = graphene_simd4f_merge_high (b->minmax.value, zero);
  graphene_simd4f_t max_v = graphene_simd4f_max (v_a, v_b);

  graphene_box2d_init_from_simd4f (res, min_v, max_v);
}

/**
 * graphene_box2d_intersection:
 * @a: a #graphene_box2d_t
 * @b: a #graphene_box2d_t
 * @res: (out caller-allocates) (optional): return location for the result
 *
 * Intersects the two given #graphene_box2d_t.
 *
 * If the two boxes do not intersect, @res will contain a degenerate box
 * initialized with graphene_box2d_empty().
 *
 * Returns: true if the two boxes intersect
 *
 * Since: 1.12
 */
bool
graphene_box2d_intersection (const graphene_box2d_t *a,
                             const graphene_box2d_t *b,
                             graphene_box2d_t       *res)
{
  const graphene_simd4f_t zero = graphene_simd4f_init_zero ();
  graphene_simd4f_t v_a, v_b;
  graphene_simd4f_t min_v, max_v;

  v_a = graphene_simd4f_merge_low (a->minmax.value, zero);
  v_b = graphene_simd4f_merge_low (b->minmax.value, zero);
  min_v = graphene_simd4f_max (v_a, v_b);

  v_a = graphene_simd4f_merge_high (a->minmax.value, zero);
  v_b = graphene_simd4f_merge_high (b->minmax.value, zero);
  max_v = graphene_simd4f_min (v_a, v_b);

  if (!graphene_simd4f_cmp_le (min_v, max_v))
    {
      if (res != NULL)
        graphene_box2d_init_from_box (res, graphene_box2d_empty ());

      return false;
    }

  if (res != NULL)
    graphene_box2d_init_from_simd4f (res, min_v, max_v);

  return true;
}

/**
 * graphene_box2d_intersects:
 * @a: a #graphene_box2d_t
 * @b: a #graphene_box2d_t
 *
 * Checks whether two boxes intersect.
 *
 * See also: graphene_box2d_intersection()
 *
 * Returns: true if the boxes intersect, and false otherwise
 *
 * Since: 1.12
 */
bool
(graphene_box2d_intersects) (const graphene_box2d_t *a,
                             const graphene_box2d_t *b)
{
  return graphene_box2d_intersects_inline (a, b);
}

/**
 * graphene_box2d_get_width:
 * @box: a #graphene_box2d_t
 *
 * Retrieves the size of the @box on the X axis.
 *
 * Returns: the width of the box
 *
 * Since: 1.12
 */
float
graphene_box2d_get_width (const graphene_box2d_t *box)
{
  graphene_simd4f_t zero = graphene_simd4f_init_zero ();
  graphene_simd4f_t min_v = graphene_simd4f_merge_low (box->minmax.value, zero);
  graphene_simd4f_t max_v = graphene_simd4f_merge_high (box->minmax.value, zero);
  graphene_simd4f_t width = graphene_simd4f_sub (max_v, min_v);
  float res = graphene_simd4f_get_x (width);

  return fabsf (res);
}

/**
 * graphene_box2d_get_height:
 * @box: a #graphene_box2d_t
 *
 * Retrieves the size of the @box on the Y axis.
 *
 * Returns: the height of the box
 *
 * Since: 1.12
 */
float
graphene_box2d_get_height (const graphene_box2d_t *box)
{
  graphene_simd4f_t zero = graphene_simd4f_init_zero ();
  graphene_simd4f_t min_v = graphene_simd4f_merge_low (box->minmax.value, zero);
  graphene_simd4f_t max_v = graphene_simd4f_merge_high (box->minmax.value, zero);
  graphene_simd4f_t height = graphene_simd4f_sub (max_v, min_v);
  float res = graphene_simd4f_get_y (height);

  return fabsf (res);
}

static inline bool
graphene_box2d_is_empty (const graphene_box2d_t *box)
{
#ifdef HAVE_ISINFF
  graphene_simd4f_t zero = graphene_simd4f_init_zero ();
  graphene_simd4f_t min_v = graphene_simd4f_merge_low (box->minmax.value, zero);
  graphene_simd4f_t max_v = graphene_simd4f_merge_high (box->minmax.value, zero);
  float vmin[2], vmax[2];

  graphene_simd4f_dup_2f (min_v, vmin);
  graphene_simd4f_dup_2f (max_v, vmax);

  return (isinff (vmin[0]) == 1 && isinff (vmin[1]) == 1) &&
         (isinff (vmax[0]) == -1 && isinff (vmax[1]) == -1);
#else
  graphene_simd4f_t zero = graphene_simd4f_init_zero ();
  graphene_simd4f_t min_v = graphene_simd4f_merge_low (box->minmax.value, zero);
  graphene_simd4f_t max_v = graphene_simd4f_merge_high (box->minmax.value, zero);
  graphene_simd4f_t neg_inf = graphene_simd4f_init (-INFINITY, -INFINITY, 0.f, 0.f);
  graphene_simd4f_t pos_inf = graphene_simd4f_init (INFINITY, INFINITY, 0.f, 0.f);

  /* This is only every going to be valid for boxes that we have
   * initialized ourselves, because we use the same values; the
   * bitwise comparison will not hold for infinities generated by
   * other operations
   */
  int min_cmp = memcmp (&min_v, &pos_inf, sizeof (graphene_simd4f_t));
  int max_cmp = memcmp (&max_v, &neg_inf, sizeof (graphene_simd4f_t));

  return min_cmp == 0 && max_cmp == 0;
#endif
}

static inline bool
graphene_box2d_is_infinity (const graphene_box2d_t *box)
{
#ifdef HAVE_ISINFF
  graphene_simd4f_t zero = graphene_simd4f_init_zero ();
  graphene_simd4f_t min_v = graphene_simd4f_merge_low (box->minmax.value, zero);
  graphene_simd4f_t max_v = graphene_simd4f_merge_high (box->minmax.value, zero);
  float vmin[2], vmax[2];

  graphene_simd4f_dup_2f (min_v, vmin);
  graphene_simd4f_dup_2f (max_v, vmax);

  return (isinff (vmin[0]) == -1 && isinff (vmin[1]) == -1) &&
         (isinff (vmax[0]) == 1 && isinff (vmax[1]) == 1);
#else
  graphene_simd4f_t zero = graphene_simd4f_init_zero ();
  graphene_simd4f_t min_v = graphene_simd4f_merge_low (box->minmax.value, zero);
  graphene_simd4f_t max_v = graphene_simd4f_merge_high (box->minmax.value, zero);
  graphene_simd4f_t neg_inf = graphene_simd4f_init (-INFINITY, -INFINITY, 0.f, 0.f);
  graphene_simd4f_t pos_inf = graphene_simd4f_init (INFINITY, INFINITY, 0.f, 0.f);

  /* This is only every going to be valid for boxes that we have
   * initialized ourselves, because we use the same values; the
   * bitwise comparison will not hold for infinities generated by
   * other operations
   */
  int min_cmp = memcmp (&min_v, &neg_inf, sizeof (graphene_simd4f_t));
  int max_cmp = memcmp (&max_v, &pos_inf, sizeof (graphene_simd4f_t));

  return min_cmp == 0 && max_cmp == 0;
#endif
}

/**
 * graphene_box2d_get_size:
 * @box: a #graphene_box2d_t
 * @size: (out caller-allocates): return location for the size
 *
 * Retrieves the size of the box on all three axes, and stores
 * it into the given @size vector.
 *
 * Since: 1.12
 */
void
graphene_box2d_get_size (const graphene_box2d_t *box,
                         graphene_vec2_t        *size)
{
  if (graphene_box2d_is_empty (box))
    size->value = graphene_simd4f_init_zero ();
  else if (graphene_box2d_is_infinity (box))
    size->value = graphene_simd4f_init (INFINITY, INFINITY, 0.f, 0.f);
  else
    {
      graphene_simd4f_t zero = graphene_simd4f_init_zero ();
      graphene_simd4f_t min_v = graphene_simd4f_merge_low (box->minmax.value, zero);
      graphene_simd4f_t max_v = graphene_simd4f_merge_high (box->minmax.value, zero);

      size->value = graphene_simd4f_sub (max_v, min_v);
    }
}

/**
 * graphene_box2d_get_center:
 * @box: a #graphene_box2d_t
 * @center: (out caller-allocates): return location for the coordinates of
 *   the center
 *
 * Retrieves the coordinates of the center of a #graphene_box2d_t.
 *
 * Since: 1.12
 */
void
graphene_box2d_get_center (const graphene_box2d_t *box,
                           graphene_point_t       *center)
{
  if (graphene_box2d_is_empty (box) || graphene_box2d_is_infinity (box))
    {
      graphene_point_init (center, 0.f, 0.f);
      return;
    }

  graphene_simd4f_t zero = graphene_simd4f_init_zero ();
  graphene_simd4f_t min_v = graphene_simd4f_merge_low (box->minmax.value, zero);
  graphene_simd4f_t max_v = graphene_simd4f_merge_high (box->minmax.value, zero);

  graphene_simd4f_t res =
    graphene_simd4f_mul (graphene_simd4f_add (min_v, max_v),
                         graphene_simd4f_splat (0.5f));

  graphene_simd4f_dup_2f (res, &(center->x));
}

/**
 * graphene_box2d_get_min:
 * @box: a #graphene_box2d_t
 * @min: (out caller-allocates): return location for the minimum point
 *
 * Retrieves the coordinates of the minimum point of the given
 * #graphene_box2d_t.
 *
 * Since: 1.12
 */
void
graphene_box2d_get_min (const graphene_box2d_t *box,
                        graphene_point_t       *min)
{
  graphene_box2d_get_minmax (box, min, NULL);
}

/**
 * graphene_box2d_get_max:
 * @box: a #graphene_box2d_t
 * @max: (out caller-allocates): return location for the maximum point
 *
 * Retrieves the coordinates of the maximum point of the given
 * #graphene_box2d_t.
 *
 * Since: 1.12
 */
void
graphene_box2d_get_max (const graphene_box2d_t *box,
                        graphene_point_t       *max)
{
  graphene_box2d_get_minmax (box, NULL, max);
}

/**
 * graphene_box2d_get_minmax:
 * @box: a #graphene_box2d_t
 * @min: (out caller-allocates): return location for the minimum point
 * @max: (out caller-allocates): return location for the maximum point
 *
 * Retrieves the coordinates of the minimum and maximum points of the
 * given #graphene_box2d_t.
 *
 * Since: 1.12
 */
void
graphene_box2d_get_minmax (const graphene_box2d_t *box,
                           graphene_point_t       *min,
                           graphene_point_t       *max)
{
  graphene_simd4f_t zero = graphene_simd4f_init_zero ();
  graphene_simd4f_t min_v = graphene_simd4f_merge_low (box->minmax.value, zero);
  graphene_simd4f_t max_v = graphene_simd4f_merge_high (box->minmax.value, zero);

  if (min != NULL)
    graphene_simd4f_dup_2f (min_v, &(min->x));
  if (max != NULL)
    graphene_simd4f_dup_2f (max_v, &(max->x));
}

/**
 * graphene_box2d_get_vertices:
 * @box: a #graphene_box2d_t
 * @vertices: (out) (array fixed-size=4): return location for an array
 *   of 4 #graphene_vec2_t
 *
 * Computes the vertices of the given #graphene_box2d_t.
 *
 * Since: 1.12
 */
void
graphene_box2d_get_vertices (const graphene_box2d_t *box,
                             graphene_vec2_t         vertices[])
{
  graphene_point_t min, max;

  graphene_box2d_get_minmax (box, &min, &max);

  graphene_vec2_init (&vertices[0], min.x, min.y);
  graphene_vec2_init (&vertices[1], min.x, max.y);
  graphene_vec2_init (&vertices[2], max.x, min.y);
  graphene_vec2_init (&vertices[3], max.x, max.y);
}

/**
 * graphene_box2d_contains_point:
 * @box: a #graphene_box2d_t
 * @point: the coordinates to check
 *
 * Checks whether @box contains the given @point.
 *
 * Returns: `true` if the point is contained in the given box
 *
 * Since: 1.12
 */
bool
graphene_box2d_contains_point (const graphene_box2d_t     *box,
                               const graphene_point_t *point)
{
  if (graphene_box2d_is_empty (box))
    return false;

  if (graphene_box2d_is_infinity (box))
    return true;

  graphene_simd4f_t p = graphene_simd4f_init (point->x, point->y, 0.f, 0.f);
  graphene_simd4f_t zero = graphene_simd4f_init_zero ();
  graphene_simd4f_t min_v = graphene_simd4f_merge_low (box->minmax.value, zero);
  graphene_simd4f_t max_v = graphene_simd4f_merge_high (box->minmax.value, zero);

  if (graphene_simd4f_cmp_ge (p, min_v) &&
      graphene_simd4f_cmp_le (p, max_v))
    return true;

  return false;
}

/**
 * graphene_box2d_contains_box:
 * @a: a #graphene_box2d_t
 * @b: a #graphene_box2d_t
 *
 * Checks whether the #graphene_box2d_t @a contains the given
 * #graphene_box2d_t @b.
 *
 * Returns: `true` if the box is contained in the given box
 *
 * Since: 1.12
 */
bool
graphene_box2d_contains_box (const graphene_box2d_t *a,
                           const graphene_box2d_t *b)
{
  if (graphene_box2d_is_empty (a) || graphene_box2d_is_infinity (b))
    return false;

  if (graphene_box2d_is_infinity (a) || graphene_box2d_is_empty (b))
    return true;

  /* we cheat a bit and access the SIMD directly */
  graphene_simd4f_t min_a = graphene_simd4f_merge_low (a->minmax.value, graphene_simd4f_init_zero ());
  graphene_simd4f_t min_b = graphene_simd4f_merge_low (b->minmax.value, graphene_simd4f_init_zero ());
  if (graphene_simd4f_cmp_ge (min_b, min_a))
    {
      graphene_simd4f_t max_a = graphene_simd4f_merge_high (a->minmax.value, graphene_simd4f_init_zero ());
      graphene_simd4f_t max_b = graphene_simd4f_merge_high (b->minmax.value, graphene_simd4f_init_zero ());
      if (graphene_simd4f_cmp_le (max_b, max_a))
        return true;
    }

  return false;
}

/**
 * graphene_box2d_contains_rect:
 * @box: a #graphene_box2d_t
 * @rect: the rectangle to check
 *
 * Checks whether @box contains the given @rect.
 *
 * Returns: `true` if the rectangle is contained in the given box
 *
 * Since: 1.12
 */
bool
graphene_box2d_contains_rect (const graphene_box2d_t *box,
                              const graphene_rect_t  *rect)
{
  if (graphene_box2d_is_empty (box))
    return false;

  if (graphene_box2d_is_infinity (box))
    return true;

  if (!graphene_box2d_contains_point (box, &rect->origin))
    return false;

  graphene_point_t tmp;
  graphene_point_init (&tmp,
                       rect->origin.x + rect->size.width,
                       rect->origin.y + rect->size.height);
  if (!graphene_box2d_contains_point (box, &tmp))
    return false;

  return true;
}

static bool
box_equal (const void *p1,
           const void *p2)
{
  const graphene_box2d_t *a = p1;
  const graphene_box2d_t *b = p2;

  if (graphene_box2d_is_empty (a) && graphene_box2d_is_empty (b))
    return true;
  else if (graphene_box2d_is_empty (a) || graphene_box2d_is_empty (b))
    return false;

  if (graphene_box2d_is_infinity (a) && graphene_box2d_is_infinity (b))
    return true;
  else if (graphene_box2d_is_infinity (a) || graphene_box2d_is_infinity (b))
    return false;

  return graphene_vec4_equal (&a->minmax, &b->minmax);
}

/**
 * graphene_box2d_equal:
 * @a: a #graphene_box2d_t
 * @b: a #graphene_box2d_t
 *
 * Checks whether the two given boxes are equal.
 *
 * Returns: `true` if the boxes are equal
 *
 * Since: 1.12
 */
bool
graphene_box2d_equal (const graphene_box2d_t *a,
                      const graphene_box2d_t *b)
{
  return graphene_pointer_equal (a, b, box_equal);
}

enum {
  BOX_ZERO = 0,
  BOX_ONE,
  BOX_MINUS_ONE,
  BOX_ONE_MINUS_ONE,
  BOX_INFINITY,
  BOX_EMPTY,

  N_STATIC_BOX
};

static graphene_box2d_t static_box[N_STATIC_BOX];

static void
init_static_box_once (void)
{
  static_box[BOX_ZERO].minmax.value = graphene_simd4f_init_zero ();

  static_box[BOX_ONE].minmax.value = graphene_simd4f_init (0.f, 0.f, 1.f, 1.f);

  static_box[BOX_MINUS_ONE].minmax.value = graphene_simd4f_init (-1.f, -1.f, 0.f, 0.f);

  static_box[BOX_ONE_MINUS_ONE].minmax.value = graphene_simd4f_init (-1.f, -1.f, 1.f, 1.f);

  static_box[BOX_INFINITY].minmax.value = graphene_simd4f_init (-INFINITY, -INFINITY, INFINITY, INFINITY);

  static_box[BOX_EMPTY].minmax.value = graphene_simd4f_init (INFINITY, INFINITY, -INFINITY, -INFINITY);
}

#ifdef HAVE_PTHREAD
static pthread_once_t static_box_once = PTHREAD_ONCE_INIT;

static inline void
init_static_box (void)
{
  int status = pthread_once (&static_box_once, init_static_box_once);

  if (status < 0)
    {
      int saved_errno = errno;

      fprintf (stderr, "pthread_once failed: %s (errno:%d)\n",
               strerror (saved_errno),
               saved_errno);
    }
}

#elif defined(HAVE_INIT_ONCE)
static INIT_ONCE static_box_once = INIT_ONCE_STATIC_INIT;

static BOOL CALLBACK
InitBoxFunc (PINIT_ONCE InitOnce,
             PVOID      param,
             PVOID     *ctx)
{
  init_static_box_once ();
  return TRUE;
}

static inline void
init_static_box (void)
{
  BOOL bStatus = InitOnceExecuteOnce (&static_box_once,
                                      InitBoxFunc,
                                      NULL,
                                      NULL);

  if (!bStatus)
    fprintf (stderr, "InitOnceExecuteOnce failed\n");
}

#else /* !HAVE_PTHREAD */
static bool static_box_init = false;

static inline void
init_static_box (void)
{
  if (static_box_init)
    return;

  init_static_box_once ();
  static_box_init = true;
}

#endif /* HAVE_PTHREAD */

/**
 * graphene_box2d_zero:
 *
 * A #graphene_box2d_t with both the minimum and maximum vertices set at (0, 0).
 *
 * The returned value is owned by Graphene and should not be modified or freed.
 *
 * Returns: (transfer none): a #graphene_box2d_t
 *
 * Since: 1.12
 */
const graphene_box2d_t *
graphene_box2d_zero (void)
{
  init_static_box ();

  return &(static_box[BOX_ZERO]);
}

/**
 * graphene_box2d_one:
 *
 * A #graphene_box2d_t with the minimum vertex set at (0, 0) and the
 * maximum vertex set at (1, 1).
 *
 * The returned value is owned by Graphene and should not be modified or freed.
 *
 * Returns: (transfer none): a #graphene_box2d_t
 *
 * Since: 1.12
 */
const graphene_box2d_t *
graphene_box2d_one (void)
{
  init_static_box ();

  return &(static_box[BOX_ONE]);
}

/**
 * graphene_box2d_minus_one:
 *
 * A #graphene_box2d_t with the minimum vertex set at (-1, -1) and the
 * maximum vertex set at (0, 0).
 *
 * The returned value is owned by Graphene and should not be modified or freed.
 *
 * Returns: (transfer none): a #graphene_box2d_t
 *
 * Since: 1.12
 */
const graphene_box2d_t *
graphene_box2d_minus_one (void)
{
  init_static_box ();

  return &(static_box[BOX_MINUS_ONE]);
}

/**
 * graphene_box2d_one_minus_one:
 *
 * A #graphene_box2d_t with the minimum vertex set at (-1, -1) and the
 * maximum vertex set at (1, 1).
 *
 * The returned value is owned by Graphene and should not be modified or freed.
 *
 * Returns: (transfer none): a #graphene_box2d_t
 *
 * Since: 1.12
 */
const graphene_box2d_t *
graphene_box2d_one_minus_one (void)
{
  init_static_box ();

  return &(static_box[BOX_ONE_MINUS_ONE]);
}

/**
 * graphene_box2d_infinite:
 *
 * A degenerate #graphene_box2d_t that cannot be expanded.
 *
 * The returned value is owned by Graphene and should not be modified or freed.
 *
 * Returns: (transfer none): a #graphene_box2d_t
 *
 * Since: 1.12
 */
const graphene_box2d_t *
graphene_box2d_infinite (void)
{
  init_static_box ();

  return &(static_box[BOX_INFINITY]);
}

/**
 * graphene_box2d_empty:
 *
 * A degenerate #graphene_box2d_t that can only be expanded.
 *
 * The returned value is owned by Graphene and should not be modified or freed.
 *
 * Returns: (transfer none): a #graphene_box2d_t
 *
 * Since: 1.12
 */
const graphene_box2d_t *
graphene_box2d_empty (void)
{
  init_static_box ();

  return &(static_box[BOX_EMPTY]);
}
