/* graphene-box2d.h: An 2D, axis aligned bounding box
 *
 * SPDX-License-Identifier: MIT
 * SPDX-FileCopyrightText: 2023  Emmanuele Bassi
 */

#pragma once

#if !defined(GRAPHENE_H_INSIDE) && !defined(GRAPHENE_COMPILATION)
#error "Only graphene.h can be included directly."
#endif

#include "graphene-types.h"
#include "graphene-point.h"
#include "graphene-simd4f.h"
#include "graphene-vec2.h"
#include "graphene-vec4.h"

GRAPHENE_BEGIN_DECLS

/**
 * graphene_box2d_t:
 *
 * A 2D box, described as the axis-aligned area between a minimum and
 * a maximum vertices lying on the same plane.
 *
 * Since: 1.12
 */
struct _graphene_box2d_t
{
  /*< private >*/
  GRAPHENE_PRIVATE_FIELD (graphene_vec4_t, minmax);
};

GRAPHENE_AVAILABLE_IN_1_12
graphene_box2d_t *        graphene_box2d_alloc                      (void);
GRAPHENE_AVAILABLE_IN_1_12
void                      graphene_box2d_free                       (graphene_box2d_t       *box);

GRAPHENE_AVAILABLE_IN_1_12
graphene_box2d_t *        graphene_box2d_init                       (graphene_box2d_t       *box,
                                                                     const graphene_point_t *min,
                                                                     const graphene_point_t *max);
GRAPHENE_AVAILABLE_IN_1_12
graphene_box2d_t *        graphene_box2d_init_from_points           (graphene_box2d_t       *box,
                                                                     unsigned int            n_points,
                                                                     const graphene_point_t *points);
GRAPHENE_AVAILABLE_IN_1_12
graphene_box2d_t *        graphene_box2d_init_from_vectors          (graphene_box2d_t       *box,
                                                                     unsigned int            n_vectors,
                                                                     const graphene_vec2_t  *vectors);
GRAPHENE_AVAILABLE_IN_1_12
graphene_box2d_t *        graphene_box2d_init_from_box              (graphene_box2d_t       *box,
                                                                     const graphene_box2d_t *src);
GRAPHENE_AVAILABLE_IN_1_12
graphene_box2d_t *        graphene_box2d_init_from_vec2             (graphene_box2d_t       *box,
                                                                     const graphene_vec2_t  *min,
                                                                     const graphene_vec2_t  *max);
GRAPHENE_AVAILABLE_IN_1_12
graphene_box2d_t *        graphene_box2d_init_from_rect             (graphene_box2d_t       *box,
                                                                     const graphene_rect_t  *src);

GRAPHENE_AVAILABLE_IN_1_12
void                      graphene_box2d_expand                     (const graphene_box2d_t *box,
                                                                     const graphene_point_t *point,
                                                                     graphene_box2d_t       *res);
GRAPHENE_AVAILABLE_IN_1_12
void                      graphene_box2d_expand_vec2                (const graphene_box2d_t *box,
                                                                     const graphene_vec2_t  *vec,
                                                                     graphene_box2d_t       *res);
GRAPHENE_AVAILABLE_IN_1_12
void                      graphene_box2d_expand_scalar              (const graphene_box2d_t *box,
                                                                     float                   scalar,
                                                                     graphene_box2d_t       *res);
GRAPHENE_AVAILABLE_IN_1_12
void                      graphene_box2d_scale_offset               (const graphene_box2d_t *box,
                                                                     const graphene_vec2_t  *scale,
                                                                     const graphene_point_t *offset,
                                                                     graphene_box2d_t       *res);

GRAPHENE_AVAILABLE_IN_1_12
void                      graphene_box2d_union                      (const graphene_box2d_t *a,
                                                                     const graphene_box2d_t *b,
                                                                     graphene_box2d_t       *res);
GRAPHENE_AVAILABLE_IN_1_12
bool                      graphene_box2d_intersection               (const graphene_box2d_t *a,
                                                                     const graphene_box2d_t *b,
                                                                     graphene_box2d_t       *res);

GRAPHENE_AVAILABLE_IN_1_12
float                     graphene_box2d_get_width                  (const graphene_box2d_t *box);
GRAPHENE_AVAILABLE_IN_1_12
float                     graphene_box2d_get_height                 (const graphene_box2d_t *box);
GRAPHENE_AVAILABLE_IN_1_12
void                      graphene_box2d_get_size                   (const graphene_box2d_t *box,
                                                                     graphene_vec2_t        *size);
GRAPHENE_AVAILABLE_IN_1_12
void                      graphene_box2d_get_center                 (const graphene_box2d_t *box,
                                                                     graphene_point_t       *center);
GRAPHENE_AVAILABLE_IN_1_12
void                      graphene_box2d_get_minmax                 (const graphene_box2d_t *box,
                                                                     graphene_point_t       *min,
                                                                     graphene_point_t       *max);
GRAPHENE_AVAILABLE_IN_1_12
void                      graphene_box2d_get_min                    (const graphene_box2d_t *box,
                                                                     graphene_point_t       *min);
GRAPHENE_AVAILABLE_IN_1_12
void                      graphene_box2d_get_max                    (const graphene_box2d_t *box,
                                                                     graphene_point_t       *max);
GRAPHENE_AVAILABLE_IN_1_12
void                      graphene_box2d_get_vertices               (const graphene_box2d_t *box,
                                                                     graphene_vec2_t         vertices[]);

GRAPHENE_AVAILABLE_IN_1_12
void                      graphene_box2d_to_float                   (const graphene_box2d_t *box,
                                                                     float                   v[4]);
GRAPHENE_AVAILABLE_IN_1_12
void                      graphene_box2d_to_rect                    (const graphene_box2d_t *box,
                                                                     graphene_rect_t        *rect);

GRAPHENE_AVAILABLE_IN_1_12
bool                      graphene_box2d_contains_point             (const graphene_box2d_t *box,
                                                                     const graphene_point_t *point);
GRAPHENE_AVAILABLE_IN_1_12
bool                      graphene_box2d_contains_box               (const graphene_box2d_t *a,
                                                                     const graphene_box2d_t *b);
GRAPHENE_AVAILABLE_IN_1_12
bool                      graphene_box2d_contains_rect              (const graphene_box2d_t *box,
                                                                     const graphene_rect_t  *rect);

GRAPHENE_AVAILABLE_IN_1_12
bool                      graphene_box2d_equal                      (const graphene_box2d_t *a,
                                                                     const graphene_box2d_t *b);

GRAPHENE_AVAILABLE_IN_1_12
const graphene_box2d_t *  graphene_box2d_zero                       (void);
GRAPHENE_AVAILABLE_IN_1_12
const graphene_box2d_t *  graphene_box2d_one                        (void);
GRAPHENE_AVAILABLE_IN_1_12
const graphene_box2d_t *  graphene_box2d_minus_one                  (void);
GRAPHENE_AVAILABLE_IN_1_12
const graphene_box2d_t *  graphene_box2d_one_minus_one              (void);
GRAPHENE_AVAILABLE_IN_1_12
const graphene_box2d_t *  graphene_box2d_infinite                   (void);
GRAPHENE_AVAILABLE_IN_1_12
const graphene_box2d_t *  graphene_box2d_empty                      (void);

GRAPHENE_AVAILABLE_IN_1_12
bool graphene_box2d_intersects (const graphene_box2d_t *a,
                                const graphene_box2d_t *b);

#ifndef __GTK_DOC_IGNORE__

#define graphene_box2d_intersects(a,b) \
  graphene_box2d_intersects_inline ((a), (b))

static inline bool
graphene_box2d_intersects_inline (const graphene_box2d_t *a,
                                  const graphene_box2d_t *b)
{
  graphene_point_t min_a, max_a;
  graphene_box2d_get_minmax (a, &min_a, &max_a);

  graphene_point_t min_b, max_b;
  graphene_box2d_get_minmax (b, &min_b, &max_b);

  graphene_simd4f_t min_v =
    graphene_simd4f_max (graphene_simd4f_init (min_a.x, min_a.y, 0.f, 0.f),
                         graphene_simd4f_init (min_b.x, min_b.y, 0.f, 0.f));
  graphene_simd4f_t max_v =
    graphene_simd4f_min (graphene_simd4f_init (max_a.x, max_a.y, 0.f, 0.f),
                         graphene_simd4f_init (max_b.x, max_b.y, 0.f, 0.f));

  if (!graphene_simd4f_cmp_le (min_v, max_v))
    return false;

  return true;
}
#endif /* __GTK_DOC_IGNORE__ */

GRAPHENE_END_DECLS
