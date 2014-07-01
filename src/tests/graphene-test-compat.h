#include <glib.h>

#if !GLIB_CHECK_VERSION (2, 40, 0)
# define g_assert_true(expr)            g_assert ((expr))
# define g_assert_false(expr)           g_assert (!(expr))
# define g_assert_null(expr)            g_assert ((expr) == NULL)
# define g_assert_nonnull(expr)         g_assert ((expr) != NULL)
#endif
