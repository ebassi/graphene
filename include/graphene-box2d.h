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
#include "graphene-vec2.h"

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
  GRAPHENE_PRIVATE_FIELD (graphene_vec2_t, min);
  GRAPHENE_PRIVATE_FIELD (graphene_vec2_t, max);
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
void                      graphene_box2d_get_min                    (const graphene_box2d_t *box,
                                                                     graphene_point_t       *min);
GRAPHENE_AVAILABLE_IN_1_12
void                      graphene_box2d_get_max                    (const graphene_box2d_t *box,
                                                                     graphene_point_t       *max);
GRAPHENE_AVAILABLE_IN_1_12
void                      graphene_box2d_get_vertices               (const graphene_box2d_t *box,
                                                                     graphene_vec2_t         vertices[]);
GRAPHENE_AVAILABLE_IN_1_12
bool                      graphene_box2d_contains_point             (const graphene_box2d_t *box,
                                                                     const graphene_point_t *point);
GRAPHENE_AVAILABLE_IN_1_12
bool                      graphene_box2d_contains_box               (const graphene_box2d_t *a,
                                                                     const graphene_box2d_t *b);

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

GRAPHENE_END_DECLS
