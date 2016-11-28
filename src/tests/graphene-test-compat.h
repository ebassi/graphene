#include <glib.h>

#if !GLIB_CHECK_VERSION (2, 40, 0)
# define g_assert_true(expr)            g_assert ((expr))
# define g_assert_false(expr)           g_assert (!(expr))
# define g_assert_null(expr)            g_assert ((expr) == NULL)
# define g_assert_nonnull(expr)         g_assert ((expr) != NULL)
#endif

#if defined(__GNUC__) && (__GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ >= 9))
/* Use typeof on GCC */
# define graphene_fuzzy_equals(n1,n2,epsilon) \
  G_GNUC_EXTENSION({ \
    __auto_type _n1 = (n1); \
    __auto_type _n2 = (n2); \
    __auto_type _epsilon = (epsilon); \
    (bool) ((_n1 > _n2 ? (_n1 - _n2) : (_n2 - _n1)) < _epsilon); \
  })

#else
/* fallback for Visual Studio, typeof not supported */
# define graphene_fuzzy_equals(n1,n2,epsilon) \
  (((n1) > (n2) ? ((n1) - (n2)) : ((n2) - (n1))) < (epsilon))

#endif /* __GNUC__ */

#define graphene_assert_fuzzy_equals(n1,n2,epsilon) \
  G_STMT_START { \
    if (graphene_fuzzy_equals (n1, n2, epsilon)) ; else { \
      g_assertion_message_cmpnum (G_LOG_DOMAIN, __FILE__, __LINE__, G_STRFUNC, \
                                  #n1 " == " #n2 " (+/- " #epsilon ")", \
                                  n1, "==", n2, 'f'); \
    } \
  } G_STMT_END

#define graphene_assert_fuzzy_matrix_cell_equal(row,col,n1,n2,epsilon) \
  G_STMT_START { \
    if (graphene_fuzzy_equals (n1, n2, epsilon)) ; else { \
      char *s = g_strdup_printf ("[%d][%d]: " #n1 " == " #n2 " (+/- " #epsilon "): (%.7g == %.7g)", \
                                 row, col, n1, n2); \
      g_assertion_message (G_LOG_DOMAIN, __FILE__, __LINE__, G_STRFUNC, s); \
      g_free (s); \
    } \
  } G_STMT_END

#define graphene_assert_fuzzy_matrix_equal(m1,m2,epsilon) \
  G_STMT_START { \
    unsigned int __i, __j; \
    float __m1[16], __m2[16]; \
    graphene_matrix_to_float ((m1), __m1); \
    graphene_matrix_to_float ((m2), __m2); \
    for (__i = 0; __i < 4; __i++) { \
      for (__j = 0; __j < 4; __j++) { \
        unsigned int __idx = __i * 4 + __j; \
        graphene_assert_fuzzy_matrix_cell_equal (__i, __j, __m1[__idx], __m2[__idx], epsilon); \
      } \
    } \
  } G_STMT_END

#define graphene_assert_fuzzy_vec2_equal(v1,v2,epsilon) \
  G_STMT_START { \
    if (graphene_vec2_near (v1, v2, epsilon)) ; \
    else { \
      char *s = g_strdup_printf (#v1 " == " #v2 " (+/- " #epsilon "): " \
                                 "{ x:%.7g, y:%.7g } == { x:%.7g, y:%.7g }", \
                                 graphene_vec2_get_x (v1), graphene_vec2_get_y (v1), \
                                 graphene_vec2_get_x (v2), graphene_vec2_get_y (v2)); \
      g_assertion_message (G_LOG_DOMAIN, __FILE__, __LINE__, G_STRFUNC, s); \
      g_free (s); \
    } \
  } G_STMT_END

#define graphene_assert_fuzzy_vec3_equal(v1,v2,epsilon) \
  G_STMT_START { \
    if (graphene_vec3_near (v1, v2, epsilon)) ; \
    else { \
      char *s = g_strdup_printf (#v1 " == " #v2 " (+/- " #epsilon "): " \
                                 "{ x:%.7g, y:%.7g, z:%.7g } == { x:%.7g, y:%.7g, z:%.7g }", \
                                 graphene_vec3_get_x (v1), graphene_vec3_get_y (v1), graphene_vec3_get_z (v1), \
                                 graphene_vec3_get_x (v2), graphene_vec3_get_y (v2), graphene_vec3_get_z (v2)); \
      g_assertion_message (G_LOG_DOMAIN, __FILE__, __LINE__, G_STRFUNC, s); \
      g_free (s); \
    } \
  } G_STMT_END

#define graphene_assert_fuzzy_vec4_equal(v1,v2,epsilon) \
  G_STMT_START { \
    if (graphene_vec4_near (v1, v2, epsilon)) ; \
    else { \
      char *s = g_strdup_printf (#v1 " == " #v2 " (+/- " #epsilon "): " \
                                 "{ x:%.7g, y:%.7g, z:%.7g, w:%.7g } == { x:%.7g, y:%.7g, z:%.7g, w:%.7g }", \
                                 graphene_vec4_get_x (v1), graphene_vec4_get_y (v1), graphene_vec4_get_z (v1), graphene_vec4_get_w (v1), \
                                 graphene_vec4_get_x (v2), graphene_vec4_get_y (v2), graphene_vec4_get_z (v2), graphene_vec4_get_w (v2)); \
      g_assertion_message (G_LOG_DOMAIN, __FILE__, __LINE__, G_STRFUNC, s); \
      g_free (s); \
    } \
  } G_STMT_END

#define graphene_assert_fuzzy_point_equal(p1,p2,epsilon) \
  G_STMT_START { \
    if (graphene_point_near (p1, p2, epsilon)) ; \
    else { \
      char *s = g_strdup_printf (#p1 " == " #p2 " (+/- " #epsilon "): " \
                                 "{ x:%.7g, y:%.7g } == { x:%.7g, y:%.7g }", \
                                 (p1)->x, (p1)->y, (p2)->x, (p2)->y); \
      g_assertion_message (G_LOG_DOMAIN, __FILE__, __LINE__, G_STRFUNC, s); \
      g_free (s); \
    } \
  } G_STMT_END

#define graphene_assert_fuzzy_size_equal(s1,s2,epsilon) \
  G_STMT_START { \
    if (graphene_fuzzy_equals ((s1)->width, (s2)->width, epsilon) && \
        graphene_fuzzy_equals ((s1)->height, (s2)->height, epsilon)) ; else { \
      char *s = g_strdup_printf ("{ width:%.7g, height:%.7g } ==  { width:%.7g, height:%.7g }", \
                                 (s1)->width, (s1)->height, (s2)->width, (s2)->height); \
      g_assertion_message (G_LOG_DOMAIN, __FILE__, __LINE__, G_STRFUNC, s); \
      g_free (s); \
    } \
  } G_STMT_END

#define graphene_assert_fuzzy_rect_equal(r1,r2,epsilon) \
  G_STMT_START { \
    graphene_assert_fuzzy_point_equal (&((r1)->origin), &((r2)->origin), epsilon); \
    graphene_assert_fuzzy_size_equal (&((r1)->size), &((r2)->size), epsilon); \
  } G_STMT_END

#define GRAPHENE_TEST_UNUSED(type,var) \
  if (0) { \
    type unused G_GNUC_UNUSED; \
    unused = var; \
  }

#define GRAPHENE_TEST_UNIT_BEGIN(name) \
static void \
name (void) \
{ \
  const float x = 2.f, y = 3.f, z = 4.f, w = 5.f; \
  const graphene_point_t zero2 = GRAPHENE_POINT_INIT_ZERO; \
  const graphene_point_t one2 = GRAPHENE_POINT_INIT (1.f, 1.f); \
  const graphene_point_t half2 = GRAPHENE_POINT_INIT (.5f, .5f); \
  const graphene_point3d_t zero3 = GRAPHENE_POINT3D_INIT_ZERO; \
  const graphene_point3d_t one3 = GRAPHENE_POINT3D_INIT (1.f, 1.f, 1.f); \
  const graphene_point3d_t half3 = GRAPHENE_POINT3D_INIT (.5f, .5f, .5f);

#define GRAPHENE_TEST_UNIT_END \
  GRAPHENE_TEST_UNUSED (float, x) \
  GRAPHENE_TEST_UNUSED (float, y) \
  GRAPHENE_TEST_UNUSED (float, z) \
  GRAPHENE_TEST_UNUSED (float, w) \
  GRAPHENE_TEST_UNUSED (graphene_point_t, zero2) \
  GRAPHENE_TEST_UNUSED (graphene_point_t, one2) \
  GRAPHENE_TEST_UNUSED (graphene_point_t, half2) \
  GRAPHENE_TEST_UNUSED (graphene_point3d_t, zero3) \
  GRAPHENE_TEST_UNUSED (graphene_point3d_t, one3) \
  GRAPHENE_TEST_UNUSED (graphene_point3d_t, half3) \
}

#define GRAPHENE_TEST_UNIT(path,name) \
  g_test_add_func (path, name);

#define GRAPHENE_TEST_SUITE(stanzas) \
int \
main (int argc, char *argv[]) \
{ \
  g_test_init (&argc, &argv, NULL); \
  g_test_bug_base ("https://github.com/ebassi/graphene/issues/%s"); \
\
  { \
    stanzas \
  } \
\
  return g_test_run (); \
}
