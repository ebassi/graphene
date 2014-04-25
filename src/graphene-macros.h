#ifndef __GRAPHENE_MACROS_H__
#define __GRAPHENE_MACROS_H__

#if !defined(GRAPHENE_H_INSIDE) && !defined(GRAPHENE_COMPILATION)
#error "Only graphene.h can be included directly."
#endif

#if defined(GRAPHENE_HAS_SSE)
# define GRAPHENE_USE_SSE
#elif defined(__GNUC__) && !defined(__arm__)
# define GRAPHENE_USE_GCC
#else
# define GRAPHENE_USE_SCALAR
#endif

#if defined(__GNUC__) 
# define GRAPHENE_SIMD4F_ALIGNED16 __attribute__((aligned (16)))
#elif defined(_WIN32)
# define GRAPHENE_SIMD4F_ALIGNED16 __declspec(align(16))
#else
# define GRAPHENE_SIMD4F_ALIGNED16
#endif

#ifndef _GRAPHENE_PUBLIC
#define _GRAPHENE_PUBLIC        extern
#endif

#define GRAPHENE_VERSION_1_0            G_ENCODE_VERSION (1, 0)
#define GRAPHENE_AVAILABLE_IN_1_0       _GRAPHENE_PUBLIC

#endif /* __GRAPHENE_MACROS_H__ */
