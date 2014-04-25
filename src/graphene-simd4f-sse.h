#ifndef __GRAPHENE_SIMD4F_SSE_H__
#define __GRAPHENE_SIMD4F_SSE_H__

#include <xmmintrin.h>
#include <string.h>  // memcpy

#ifdef __cplusplus
extern "C" {
#endif

typedef __m128 graphene_simd4f_t;

typedef union {
  graphene_simd4f_t s;
  float f[4];
  unsigned int ui[4];
} graphene_simd4f_union_t;

#define GRAPHENE_SIMD4F_GET(field,idx) \
static inline float \
graphene_simd4f_get_##field (graphene_simd4f_t s) \
{ \
  graphene_simd4f_union_t u = { s }; \
  return u.f[idx]; \
}

GRAPHENE_SIMD4F_GET (x, 0)
GRAPHENE_SIMD4F_GET (y, 1)
GRAPHENE_SIMD4F_GET (z, 2)
GRAPHENE_SIMD4F_GET (w, 3)

#undef GRAPHENE_SIMD4F_GET

static inline graphene_simd4f_t
graphene_simd4f_init (float x,
                      float y,
                      float z,
                      float w)
{
  graphene_simd4f_t s = { x, y, z, w };
  return s;
}

static inline graphene_simd4f_t
graphene_simd4f_init_zero (void)
{
  return _mm_setzero_ps();
}

static inline graphene_simd4f_t
graphene_simd4f_init_4f (const float *v)
{
  graphene_simd4f_t s = _mm_loadu_ps (v);
  return s;
}

static inline graphene_simd4f_t
graphene_simd4f_init_3f (const float *v)
{
  graphene_simd4f_t s = graphene_simd4f_init (v[0], v[1], v[2], 0.f);
  return s;
}

static inline graphene_simd4f_t
graphene_simd4f_init_2f (const float *v)
{
  graphene_simd4f_t s = graphene_simd4f_init (v[0], v[1], 0.f, 0.f);
  return s;
}

static inline void
graphene_simd4f_dup_4f (const graphene_simd4f_t  s,
                        float                   *v)
{
  _mm_storeu_ps (v, s);
}

static inline void
graphene_simd4f_dup_3f (const graphene_simd4f_t  s,
                        float                   *v)
{
  memcpy (v, &s, sizeof (float) * 3);
}

static inline void
graphene_simd4f_dup_2f (const graphene_simd4f_t  s,
                        float                   *v)
{
  memcpy (v, &s, sizeof (float) * 2);
}

static inline graphene_simd4f_t
graphene_simd4f_splat (float v)
{
  graphene_simd4f_t s = _mm_set1_ps (v);
  return s;
}

static inline graphene_simd4f_t
graphene_simd4f_splat_x (graphene_simd4f_t v)
{
  graphene_simd4f_t s = _mm_shuffle_ps (v, v, _MM_SHUFFLE (0, 0, 0, 0));
  return s;
}

static inline graphene_simd4f_t
graphene_simd4f_splat_y (graphene_simd4f_t v)
{
  graphene_simd4f_t s = _mm_shuffle_ps (v, v, _MM_SHUFFLE (1, 1, 1, 1));
  return s;
}

static inline graphene_simd4f_t
graphene_simd4f_splat_z (graphene_simd4f_t v)
{
  graphene_simd4f_t s = _mm_shuffle_ps (v, v, _MM_SHUFFLE (2, 2, 2, 2));
  return s;
}

static inline graphene_simd4f_t
graphene_simd4f_splat_w (graphene_simd4f_t v)
{
  graphene_simd4f_t s = _mm_shuffle_ps (v, v, _MM_SHUFFLE (3, 3, 3, 3));
  return s;
}

#define GRAPHENE_SIMD4F_OP(op,func) \
static inline graphene_simd4f_t \
graphene_simd4f_##op (graphene_simd4f_t a, graphene_simd4f_t b) \
{ \
  graphene_simd4f_t res = func (a, b); \
  return res; \
}

GRAPHENE_SIMD4F_OP (add, _mm_add_ps)
GRAPHENE_SIMD4F_OP (sub, _mm_sub_ps)
GRAPHENE_SIMD4F_OP (mul, _mm_mul_ps)
GRAPHENE_SIMD4F_OP (div, _mm_div_ps)

#undef GRAPHENE_SIMD4F_OP

static inline graphene_simd4f_t
graphene_simd4f_reciprocal (graphene_simd4f_t v)
{
  const graphene_simd4f_t two = graphene_simd4f_init (2.0f, 2.0f, 2.0f, 2.0f);
  graphene_simd4f_t s = _mm_rcp_ps (v);
  s = graphene_simd4f_mul (s, graphene_simd4f_sub (two, graphene_simd4f_mul (v, s)));
  return s;
}

static inline graphene_simd4f_t
graphene_simd4f_sqrt (graphene_simd4f_t v) {
  graphene_simd4f_t s = _mm_sqrt_ps (v);
  return s;
}

static inline graphene_simd4f_t
graphene_simd4f_rsqrt (graphene_simd4f_t v) {
  const graphene_simd4f_t half = graphene_simd4f_init (0.5f, 0.5f, 0.5f, 0.5f);
  const graphene_simd4f_t three = graphene_simd4f_init (3.0f, 3.0f, 3.0f, 3.0f);
  graphene_simd4f_t s = _mm_rsqrt_ps (v);
  s = graphene_simd4f_mul (graphene_simd4f_mul (s, half),
                           graphene_simd4f_sub (three,
                                                graphene_simd4f_mul (s, graphene_simd4f_mul (v, s))));
  return s;
}

static inline graphene_simd4f_t
graphene_simd4f_cross3 (graphene_simd4f_t a,
                        graphene_simd4f_t b)
{
  const graphene_simd4f_t a_yzx = _mm_shuffle_ps (a, a, _MM_SHUFFLE (3, 0, 2, 1));
  const graphene_simd4f_t a_zxy = _mm_shuffle_ps (a, a, _MM_SHUFFLE (3, 1, 0, 2));
  const graphene_simd4f_t b_yzx = _mm_shuffle_ps (b, b, _MM_SHUFFLE (3, 0, 2, 1));
  const graphene_simd4f_t b_zxy = _mm_shuffle_ps (b, b, _MM_SHUFFLE (3, 1, 0, 2));

  return _mm_sub_ps (_mm_mul_ps (a_yzx, b_zxy), _mm_mul_ps (a_zxy, b_yzx));
}

static inline graphene_simd4f_t
graphene_simd4f_min (graphene_simd4f_t a,
                     graphene_simd4f_t b)
{
  return _mm_min_ps (a, b);
}

static inline graphene_simd4f_t
graphene_simd4f_max (graphene_simd4f_t a,
                     graphene_simd4f_t b)
{
  return _mm_max_ps (a, b);
}

#ifdef __cplusplus
}
#endif

#endif
