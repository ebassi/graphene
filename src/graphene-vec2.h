#ifndef __GRAPHENE_VECTORS_H__
#define __GRAPHENE_VECTORS_H__

#if !defined(GRAPHENE_H_INSIDE) && !defined(GRAPHENE_COMPILATION)
#error "Only graphene.h can be included directly."
#endif

#include "graphene-types.h"

G_BEGIN_DECLS

GRAPHENE_AVAILABLE_IN_1_0
graphene_vec2_t *       graphene_vec2_alloc             (void);
GRAPHENE_AVAILABLE_IN_1_0
void                    graphene_vec2_free              (graphene_vec2_t       *v);
GRAPHENE_AVAILABLE_IN_1_0
graphene_vec2_t *       graphene_vec2_init              (graphene_vec2_t       *v,
                                                         float                  x,
                                                         float                  y);
GRAPHENE_AVAILABLE_IN_1_0
graphene_vec2_t *       graphene_vec2_init_from_vec2    (graphene_vec2_t       *v,
                                                         const graphene_vec2_t *src);
GRAPHENE_AVAILABLE_IN_1_0
graphene_vec2_t *       graphene_vec2_init_from_float   (graphene_vec2_t       *v,
                                                         const float           *src);
GRAPHENE_AVAILABLE_IN_1_0
void                    graphene_vec2_to_float          (const graphene_vec2_t *v,
                                                         float                 *dest);
GRAPHENE_AVAILABLE_IN_1_0
void                    graphene_vec2_add               (const graphene_vec2_t *a,
                                                         const graphene_vec2_t *b,
                                                         graphene_vec2_t       *res);
GRAPHENE_AVAILABLE_IN_1_0
void                    graphene_vec2_subtract          (const graphene_vec2_t *a,
                                                         const graphene_vec2_t *b,
                                                         graphene_vec2_t       *res);
GRAPHENE_AVAILABLE_IN_1_0
void                    graphene_vec2_multiply          (const graphene_vec2_t *a,
                                                         const graphene_vec2_t *b,
                                                         graphene_vec2_t       *res);
GRAPHENE_AVAILABLE_IN_1_0
void                    graphene_vec2_divide            (const graphene_vec2_t *a,
                                                         const graphene_vec2_t *b,
                                                         graphene_vec2_t       *res);
GRAPHENE_AVAILABLE_IN_1_0
float                   graphene_vec2_dot               (const graphene_vec2_t *a,
                                                         const graphene_vec2_t *b);
GRAPHENE_AVAILABLE_IN_1_0
float                   graphene_vec2_length            (const graphene_vec2_t *v);
GRAPHENE_AVAILABLE_IN_1_0
void                    graphene_vec2_normalize         (const graphene_vec2_t *v,
                                                         graphene_vec2_t       *res);
GRAPHENE_AVAILABLE_IN_1_0
void                    graphene_vec2_min               (const graphene_vec2_t *a,
                                                         const graphene_vec2_t *b,
                                                         graphene_vec2_t       *res);
GRAPHENE_AVAILABLE_IN_1_0
void                    graphene_vec2_max               (const graphene_vec2_t *a,
                                                         const graphene_vec2_t *b,
                                                         graphene_vec2_t       *res);

GRAPHENE_AVAILABLE_IN_1_0
float                   graphene_vec2_get_x             (const graphene_vec2_t *v);
GRAPHENE_AVAILABLE_IN_1_0
float                   graphene_vec2_get_y             (const graphene_vec2_t *v);

GRAPHENE_AVAILABLE_IN_1_0
const graphene_vec2_t * graphene_vec2_zero              (void);
GRAPHENE_AVAILABLE_IN_1_0
const graphene_vec2_t * graphene_vec2_one               (void);
GRAPHENE_AVAILABLE_IN_1_0
const graphene_vec2_t * graphene_vec2_x_axis            (void);
GRAPHENE_AVAILABLE_IN_1_0
const graphene_vec2_t * graphene_vec2_y_axis            (void);

G_END_DECLS

#endif /* __GRAPHENE_VECTORS_H__ */
