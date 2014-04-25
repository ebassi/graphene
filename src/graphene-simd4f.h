#ifndef __GRAPHENE_SIMD4F_H__
#define __GRAPHENE_SIMD4F_H__

#include "graphene-macros.h"

#if defined(GRAPHENE_HAS_SSE)
# define GRAPHENE_USE_SSE
#elif defined(GRAPHENE_HAS_ARM_NEON)
# define GRAPHENE_USE_ARM_NEON
#elif defined(__GNUC__) && !defined(__arm__)
# define GRAPHENE_USE_GCC
#else
# define GRAPHENE_USE_SCALAR
#endif

#if defined(GRAPHENE_USE_SSE)
# include "graphene-simd4f-sse.h"
#elif defined(GRAPHENE_USE_ARM_NEON)
# warn "ARM NEON optimizations are not available yet, falling back to scalar."
# include "graphene-simd4f-scalar.h" /* include graphene-simd4f-neon.h */
#elif defined(GRAPHENE_USE_GCC)
# include "graphene-simd4f-gcc.h"
#elif defined(GRAPHENE_USE_SCALAR)
# include "graphene-simd4f-scalar.h"
#else
# error "Unsupported simd4f implementation."
#endif

#include "graphene-simd4f-common.h"

#ifdef GRAPHENE_ENABLE_DEBUG
# define GRAPHENE_SIMD4F_DEBUG(vec)             G_STMT_START { \
  g_debug ("simd4f(%.6f,%.6f,%.6f,%.6f)", \
           graphene_simd4f_get_x (vec), \
           graphene_simd4f_get_y (vec), \
           graphene_simd4f_get_z (vec), \
           graphene_simd4f_get_w (vec));        } G_STMT_END
#else
# define GRAPHENE_SIMD4F_DEBUG(vec)
#endif

#endif /* __GRAPHENE_SIMD4F_H__ */
