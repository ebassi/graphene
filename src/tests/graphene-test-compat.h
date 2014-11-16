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
    if (__n1 > __n2) { \
      if ((__n1 - __n2) < __epsilon) ; else { \
        g_assertion_message_cmpnum (G_LOG_DOMAIN, __FILE__, __LINE__, G_STRFUNC, \
                                    #n1 " == " #n2 " (+/- " #epsilon ")", \
                                    __n1, "==", __n2, 'f'); \
      } \
    } else { \
      if ((__n2 - __n1) < __epsilon) ; else { \
        g_assertion_message_cmpnum (G_LOG_DOMAIN, __FILE__, __LINE__, G_STRFUNC, \
                                    #n1 " == " #n2 " (+/- " #epsilon ")", \
                                    __n1, "==", __n2, 'f'); \
      } \
    } \
  } G_STMT_END

/* __GNUC__ */
#else
/* fallback for Visual Studio, decltype not supported */
# define graphene_assert_fuzzy_equals(n1,n2,epsilon) \
  G_STMT_START { \
    if ((n1) > (n2)) { \
      if ((n1) - (n2) < epsilon) ; else { \
        g_assertion_message_cmpnum (G_LOG_DOMAIN, __FILE__, __LINE__, G_STRFUNC, \
                                    #n1 " == " #n2 " (+/- " #epsilon ")", \
                                    n1, "==", n2, 'f'); \
      } \
    } else { \
      if ((n2) - (n1) < epsilon) ; else { \
        g_assertion_message_cmpnum (G_LOG_DOMAIN, __FILE__, __LINE__, G_STRFUNC, \
                                    #n1 " == " #n2 " (+/- " #epsilon ")", \
                                    n1, "==", n2, 'f'); \
      } \
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
  graphene_point3d_t zero = GRAPHENE_POINT3D_INIT_ZERO; \
  graphene_point3d_t one = GRAPHENE_POINT3D_INIT (1.f, 1.f, 1.f); \
  graphene_point3d_t two = GRAPHENE_POINT3D_INIT (2.f, 2.f, 2.f); \
  graphene_point3d_t half = GRAPHENE_POINT3D_INIT (.5f, .5f, .5f);

#define GRAPHENE_TEST_UNIT_END \
  GRAPHENE_TEST_UNUSED (x); \
  GRAPHENE_TEST_UNUSED (y); \
  GRAPHENE_TEST_UNUSED (z); \
  GRAPHENE_TEST_UNUSED (w); \
  GRAPHENE_TEST_UNUSED (zero); \
  GRAPHENE_TEST_UNUSED (one); \
  GRAPHENE_TEST_UNUSED (two); \
  GRAPHENE_TEST_UNUSED (half); \
}

#define GRAPHENE_TEST_UNIT(path,name) \
  g_test_add_func (path, name);

#define GRAPHENE_TEST_SUITE(stanzas) \
int \
main (int argc, char *argv[]) \
{ \
  g_test_init (&argc, &argv, NULL); \
\
  { \
    stanzas \
  } \
\
  return g_test_run (); \
}
