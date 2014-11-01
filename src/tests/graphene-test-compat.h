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
