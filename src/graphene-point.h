#ifndef __GRAPHENE_POINT_H__
#define __GRAPHENE_POINT_H__

#if !defined(GRAPHENE_H_INSIDE) && !defined(GRAPHENE_COMPILATION)
#error "Only graphene.h can be included directly."
#endif

#include "graphene-types.h"

G_BEGIN_DECLS

#define GRAPHENE_POINT_INIT(x,y)        { x, y }
#define GRAPHENE_POINT_INIT_ZERO        GRAPHENE_POINT_INIT (0.f, 0.f)

struct _graphene_point_t
{
  float x;
  float y;
};

GRAPHENE_AVAILABLE_IN_1_0
graphene_point_t *              graphene_point_alloc            (void);
GRAPHENE_AVAILABLE_IN_1_0
void                            graphene_point_free             (graphene_point_t       *p);

GRAPHENE_AVAILABLE_IN_1_0
graphene_point_t *              graphene_point_init             (graphene_point_t       *p,
                                                                 float                   x,
                                                                 float                   y);
GRAPHENE_AVAILABLE_IN_1_0
graphene_point_t *              graphene_point_init_from_point  (graphene_point_t *p,
                                                                 const graphene_point_t *src);

GRAPHENE_AVAILABLE_IN_1_0
float                           graphene_point_distance         (const graphene_point_t *a,
                                                                 const graphene_point_t *b,
                                                                 float                  *d_x,
                                                                 float                  *d_y);

GRAPHENE_AVAILABLE_IN_1_0
gboolean                        graphene_point_near             (const graphene_point_t *a,
                                                                 const graphene_point_t *b,
                                                                 float                   epsilon);

GRAPHENE_AVAILABLE_IN_1_0
void                            graphene_point_interpolate      (const graphene_point_t *a,
                                                                 const graphene_point_t *b,
                                                                 double                  factor,
                                                                 graphene_point_t       *res);

GRAPHENE_AVAILABLE_IN_1_0
const graphene_point_t *        graphene_point_zero             (void);

#define GRAPHENE_SIZE_INIT(w,h)         { w, h }
#define GRAPHENE_SIZE_INIT_ZERO         GRAPHENE_SIZE_INIT (0.f, 0.f)

struct _graphene_size_t
{
  float width;
  float height;
};

GRAPHENE_AVAILABLE_IN_1_0
graphene_size_t *               graphene_size_alloc             (void);
GRAPHENE_AVAILABLE_IN_1_0
void                            graphene_size_free              (graphene_size_t        *s);
GRAPHENE_AVAILABLE_IN_1_0
graphene_size_t *               graphene_size_init              (graphene_size_t        *s,
                                                                 float                   width,
                                                                 float                   height);
GRAPHENE_AVAILABLE_IN_1_0
graphene_size_t *               graphene_size_init_from_size    (graphene_size_t        *s,
                                                                 const graphene_size_t  *src);

GRAPHENE_AVAILABLE_IN_1_0
void                            graphene_size_scale             (const graphene_size_t  *s,
                                                                 float                   factor,
                                                                 graphene_size_t        *res);
GRAPHENE_AVAILABLE_IN_1_0
void                            graphene_size_interpolate       (const graphene_size_t  *a,
                                                                 const graphene_size_t  *b,
                                                                 double                  factor,
                                                                 graphene_size_t        *res);

G_END_DECLS

#endif /* __GRAPHENE_POINT_H__ */
