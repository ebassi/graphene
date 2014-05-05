#include <glib.h>
#include <graphene.h>

static void
rect_init (void)
{
  graphene_rect_t *r;
  graphene_rect_t s;

  r = graphene_rect_init (graphene_rect_alloc (), 0.f, 0.f, 10.f, 10.f);
  g_assert_cmpfloat (r->origin.x, ==, 0.f);
  g_assert_cmpfloat (r->origin.y, ==, 0.f);
  g_assert_cmpfloat (r->size.width, ==, 10.f);
  g_assert_cmpfloat (r->size.height, ==, 10.f);

  graphene_rect_init_from_rect (&s, r);
  g_assert_true (graphene_rect_equal (&s, r));

  graphene_rect_free (r);
}

int
main (int argc, char *argv[])
{
  g_test_init (&argc, &argv, NULL);

  g_test_add_func ("/rect/init", rect_init);

  return g_test_run ();
}
