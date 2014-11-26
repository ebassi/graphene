#include <glib.h>

#if !GLIB_CHECK_VERSION (2, 40, 0)
# define g_assert_true(expr)            g_assert ((expr))
# define g_assert_false(expr)           g_assert (!(expr))
# define g_assert_null(expr)            g_assert ((expr) == NULL)
# define g_assert_nonnull(expr)         g_assert ((expr) != NULL)
#endif

#if defined(__GNUC__)
/* Use typeof on GCC */
# define graphene_assert_fuzzy_equals(n1,n2,epsilon) \
  G_STMT_START { \
    typeof ((n1)) __n1 = (n1); \
    typeof ((n2)) __n2 = (n2); \
    typeof ((epsilon)) __epsilon = (epsilon); \
    if ((__n1 > __n2 ? (__n1 - __n2) : (__n2 - __n1)) < __epsilon) ; else { \
      g_assertion_message_cmpnum (G_LOG_DOMAIN, __FILE__, __LINE__, G_STRFUNC, \
                                  #n1 " == " #n2 " (+/- " #epsilon ")", \
                                  __n1, "==", __n2, 'f'); \
    } \
  } G_STMT_END

# define graphene_assert_fuzzy_matrix_equal(row,col,n1,n2,epsilon) \
  G_STMT_START { \
    typeof ((n1)) __n1 = (n1); \
    typeof ((n2)) __n2 = (n2); \
    typeof ((epsilon)) __epsilon = (epsilon); \
    if ((__n1 > __n2 ? (__n1 - __n2) : (__n2 - __n1)) < __epsilon) ; else { \
      char *s = g_strdup_printf ("[%d][%d]: " #n1 " == " #n2 " (+/- " #epsilon "): (%.7g == %.7g)", \
                                 row, col, __n1, __n2); \
      g_assertion_message (G_LOG_DOMAIN, __FILE__, __LINE__, G_STRFUNC, s); \
      g_free (s); \
    } \
  } G_STMT_END

/* __GNUC__ */
#else
/* fallback for Visual Studio, decltype not supported */
# define graphene_assert_fuzzy_equals(n1,n2,epsilon) \
  G_STMT_START { \
    if (((n1) > (n2) ? ((n1) - (n2)) : ((n2) - (n1))) < (epsilon)) ; else { \
      g_assertion_message_cmpnum (G_LOG_DOMAIN, __FILE__, __LINE__, G_STRFUNC, \
                                  #n1 " == " #n2 " (+/- " #epsilon ")", \
                                  n1, "==", n2, 'f'); \
    } \
  } G_STMT_END

# define graphene_assert_fuzzy_matrix_equal(row,col,n1,n2,epsilon) \
  G_STMT_START { \
    if (((n1) > (n2) ? ((n1) - (n2)) : ((n2) - (n1))) < (epsilon)) ; else { \
      char *s = g_strdup_printf ("[%d][%d]: " #n1 " == " #n2 " (+/- " #epsilon "): (%.7g == %.7g)", \
                                 row, col, (n1), (n2)); \
      g_assertion_message (G_LOG_DOMAIN, __FILE__, __LINE__, G_STRFUNC, s); \
      g_free (s); \
    } \
  } G_STMT_END

#endif /* __GNUC__ */

#define GRAPHENE_TEST_UNUSED(var) \
  if (0) var = var

#define GRAPHENE_TEST_UNIT_BEGIN(name) \
static void \
name (void) \
{ \
  float x = 2.f, y = 3.f, z = 4.f, w = 5.f; \
  graphene_point_t zero2 = GRAPHENE_POINT_INIT_ZERO; \
  graphene_point_t one2 = GRAPHENE_POINT_INIT (1.f, 1.f); \
  graphene_point_t half2 = GRAPHENE_POINT_INIT (.5f, .5f); \
  graphene_point3d_t zero3 = GRAPHENE_POINT3D_INIT_ZERO; \
  graphene_point3d_t one3 = GRAPHENE_POINT3D_INIT (1.f, 1.f, 1.f); \
  graphene_point3d_t half3 = GRAPHENE_POINT3D_INIT (.5f, .5f, .5f);

#define GRAPHENE_TEST_UNIT_END \
  GRAPHENE_TEST_UNUSED (x); \
  GRAPHENE_TEST_UNUSED (y); \
  GRAPHENE_TEST_UNUSED (z); \
  GRAPHENE_TEST_UNUSED (w); \
  GRAPHENE_TEST_UNUSED (zero2); \
  GRAPHENE_TEST_UNUSED (one2); \
  GRAPHENE_TEST_UNUSED (half2); \
  GRAPHENE_TEST_UNUSED (zero3); \
  GRAPHENE_TEST_UNUSED (one3); \
  GRAPHENE_TEST_UNUSED (half3); \
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
