#ifndef __GRAPHENE_VECTORS_H__
#define __GRAPHENE_VECTORS_H__

#if !defined(GRAPHENE_H_INSIDE) && !defined(GRAPHENE_COMPILATION)
#error "Only graphene.h can be included directly."
#endif

#include "graphene-types.h"

G_BEGIN_DECLS

GRAPHENE_AVAILABLE_IN_1_0
graphene_vec4_t *       graphene_vec4_alloc             (void);
GRAPHENE_AVAILABLE_IN_1_0
void                    graphene_vec4_free              (graphene_vec4_t       *v);
GRAPHENE_AVAILABLE_IN_1_0
graphene_vec4_t *       graphene_vec4_init              (graphene_vec4_t       *v,
                                                         float                  x,
                                                         float                  y,
                                                         float                  z,
                                                         float                  w);
GRAPHENE_AVAILABLE_IN_1_0
graphene_vec4_t *       graphene_vec4_init_from_vec4    (graphene_vec4_t       *v,
                                                         const graphene_vec4_t *src);
GRAPHENE_AVAILABLE_IN_1_0
graphene_vec4_t *       graphene_vec4_init_from_float   (graphene_vec4_t       *v,
                                                         const float           *src);
GRAPHENE_AVAILABLE_IN_1_0
void                    graphene_vec4_to_float          (const graphene_vec4_t *v,
                                                         float                 *dest);
GRAPHENE_AVAILABLE_IN_1_0
void                    graphene_vec4_add               (const graphene_vec4_t *a,
                                                         const graphene_vec4_t *b,
                                                         graphene_vec4_t       *res);
GRAPHENE_AVAILABLE_IN_1_0
void                    graphene_vec4_subtract          (const graphene_vec4_t *a,
                                                         const graphene_vec4_t *b,
                                                         graphene_vec4_t       *res);
GRAPHENE_AVAILABLE_IN_1_0
void                    graphene_vec4_multiply          (const graphene_vec4_t *a,
                                                         const graphene_vec4_t *b,
                                                         graphene_vec4_t       *res);
GRAPHENE_AVAILABLE_IN_1_0
void                    graphene_vec4_divide            (const graphene_vec4_t *a,
                                                         const graphene_vec4_t *b,
                                                         graphene_vec4_t       *res);
GRAPHENE_AVAILABLE_IN_1_0
float                   graphene_vec4_dot               (const graphene_vec4_t *a,
                                                         const graphene_vec4_t *b);
GRAPHENE_AVAILABLE_IN_1_0
float                   graphene_vec4_length            (const graphene_vec4_t *v);
GRAPHENE_AVAILABLE_IN_1_0
void                    graphene_vec4_normalize         (const graphene_vec4_t *v,
                                                         graphene_vec4_t       *res);
GRAPHENE_AVAILABLE_IN_1_0
void                    graphene_vec4_min               (const graphene_vec4_t *a,
                                                         const graphene_vec4_t *b,
                                                         graphene_vec4_t       *res);
GRAPHENE_AVAILABLE_IN_1_0
void                    graphene_vec4_max               (const graphene_vec4_t *a,
                                                         const graphene_vec4_t *b,
                                                         graphene_vec4_t       *res);

GRAPHENE_AVAILABLE_IN_1_0
float                   graphene_vec4_get_x             (const graphene_vec4_t *v);
GRAPHENE_AVAILABLE_IN_1_0
float                   graphene_vec4_get_y             (const graphene_vec4_t *v);
GRAPHENE_AVAILABLE_IN_1_0
float                   graphene_vec4_get_z             (const graphene_vec4_t *v);
GRAPHENE_AVAILABLE_IN_1_0
float                   graphene_vec4_get_w             (const graphene_vec4_t *v);

GRAPHENE_AVAILABLE_IN_1_0
void                    graphene_vec4_get_xy            (const graphene_vec4_t *v,
                                                         graphene_vec2_t       *res);
GRAPHENE_AVAILABLE_IN_1_0
void                    graphene_vec4_get_xyz           (const graphene_vec4_t *v,
                                                         graphene_vec3_t       *res);

GRAPHENE_AVAILABLE_IN_1_0
const graphene_vec4_t * graphene_vec4_zero              (void);
GRAPHENE_AVAILABLE_IN_1_0
const graphene_vec4_t * graphene_vec4_one               (void);
GRAPHENE_AVAILABLE_IN_1_0
const graphene_vec4_t * graphene_vec4_x_axis            (void);
GRAPHENE_AVAILABLE_IN_1_0
const graphene_vec4_t * graphene_vec4_y_axis            (void);
GRAPHENE_AVAILABLE_IN_1_0
const graphene_vec4_t * graphene_vec4_z_axis            (void);
GRAPHENE_AVAILABLE_IN_1_0
const graphene_vec4_t * graphene_vec4_w_axis            (void);

G_END_DECLS

#endif /* __GRAPHENE_VECTORS_H__ */
