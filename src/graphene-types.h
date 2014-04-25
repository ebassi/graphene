#ifndef __GRAPHENE_TYPES_H__
#define __GRAPHENE_TYPES_H__

#if !defined(GRAPHENE_H_INSIDE) && !defined(GRAPHENE_COMPILATION)
#error "Only graphene.h can be included directly."
#endif

#include <glib.h>
#include "graphene-macros.h"

G_BEGIN_DECLS

#define GRAPHENE_VEC2_LEN       2
#define GRAPHENE_VEC3_LEN       3
#define GRAPHENE_VEC4_LEN       4

typedef struct _graphene_vec2_t         graphene_vec2_t;
typedef struct _graphene_vec3_t         graphene_vec3_t;
typedef struct _graphene_vec4_t         graphene_vec4_t;

G_END_DECLS

#endif /* __GRAPHENE_TYPES_H__ */
