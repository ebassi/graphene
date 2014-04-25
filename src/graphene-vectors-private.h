#ifndef __GRAPHENE_VECTORS_PRIVATE_H__
#define __GRAPHENE_VECTORS_PRIVATE_H__

#include "graphene-types.h"

#include "graphene-vec2.h"
#include "graphene-vec3.h"
#include "graphene-vec4.h"

#include "graphene-simd4f.h"

struct _graphene_vec2_t
{
  graphene_simd4f_t value;
};

struct _graphene_vec3_t
{
  graphene_simd4f_t value;
};

struct _graphene_vec4_t
{
  graphene_simd4f_t value;
};


#endif /* __GRAPHENE_VECTORS_PRIVATE_H__ */
