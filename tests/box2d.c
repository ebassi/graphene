/* SPDX-License-Identifier: MIT */

#include <graphene.h>
#include <mutest.h>
#include <stdio.h>

static void
box2d_init_min_max (mutest_spec_t *spec)
{
  graphene_point_t points[] = {
    GRAPHENE_POINT_INIT (0.f, 0.f),
    GRAPHENE_POINT_INIT (1.f, 1.f)
  };
  graphene_point_t zero, one;
  graphene_point_t min, max;
  graphene_box2d_t *b;

  graphene_point_init (&zero, 0.f, 0.f);
  graphene_point_init (&one, 1.f, 1.f);

  b = graphene_box2d_init (graphene_box2d_alloc (), &points[0], &points[1]);
  mutest_expect ("init() does not return null",
                 mutest_pointer (b),
                 mutest_not, mutest_to_be_null,
                 NULL);

  graphene_box2d_get_min (b, &min);
  mutest_expect ("get_min() to be zero",
                 mutest_bool_value (graphene_point_equal (&min, &points[0])),
                 mutest_to_be_true,
                 NULL);
  graphene_box2d_get_max (b, &max);
  mutest_expect ("get_max() to be one",
                 mutest_bool_value (graphene_point_equal (&max, &points[1])),
                 mutest_to_be_true,
                 NULL);

  graphene_box2d_init (b, NULL, NULL);
  mutest_expect ("init(null, null) to be zero-sized",
                 mutest_bool_value (graphene_box2d_equal (b, graphene_box2d_zero ())),
                 mutest_to_be_true,
                 NULL);

  graphene_box2d_init_from_vec2 (b, graphene_vec2_zero (), graphene_vec2_one ());
  graphene_box2d_get_min (b, &min);
  graphene_box2d_get_max (b, &max);
  mutest_expect ("init_from_vec2(zero, one).min() maps to point(zero)",
                 mutest_bool_value (graphene_point_equal (&min, &zero)),
                 mutest_to_be_true,
                 NULL);
  mutest_expect ("init_from_vec2(zero, one).max() maps to point(one)",
                 mutest_bool_value (graphene_point_equal (&max, &one)),
                 mutest_to_be_true,
                 NULL);

  graphene_box2d_init_from_vec2 (b, NULL, NULL);
  mutest_expect ("init_from_vec2(null, null) is equal to box2d(zero)",
                 mutest_bool_value (graphene_box2d_equal (b, graphene_box2d_zero ())),
                 mutest_to_be_true,
                 NULL);

  graphene_box2d_free (b);
}

static void
box2d_init_from_points (mutest_spec_t *spec)
{
  graphene_point_t points[] = {
    GRAPHENE_POINT_INIT (0.f, 0.f),
    GRAPHENE_POINT_INIT (1.f, 1.f),
    GRAPHENE_POINT_INIT (2.f, 2.f),
  };
  graphene_point_t min, max;
  graphene_box2d_t *b;

  b = graphene_box2d_init_from_points (graphene_box2d_alloc (), 3, points);
  mutest_expect ("init_from_points() does not return null",
                 mutest_pointer (b),
                 mutest_not, mutest_to_be_null,
                 NULL);

  graphene_box2d_get_min (b, &min);
  graphene_box2d_get_max (b, &max);
  mutest_expect ("init_from_points().min() is (0, 0)",
                 mutest_bool_value (graphene_point_equal (&min, &points[0])),
                 mutest_to_be_true,
                 NULL);
  mutest_expect ("init_from_points().max() is (2, 2)",
                 mutest_bool_value (graphene_point_equal (&max, &points[2])),
                 mutest_to_be_true,
                 NULL);

  graphene_box2d_init_from_points (b, 1, points + 1);
  graphene_box2d_get_min (b, &min);
  graphene_box2d_get_max (b, &max);
  mutest_expect ("init_from_points() with one point sets min() to the same point",
                 mutest_bool_value (graphene_point_equal (&min, &points[1])),
                 mutest_to_be_true,
                 NULL);
  mutest_expect ("init_from_points() with one point sets max() to the same point",
                 mutest_bool_value (graphene_point_equal (&max, &points[1])),
                 mutest_to_be_true,
                 NULL);

  graphene_box2d_init_from_points (b, 0, NULL);
  mutest_expect ("init_from_points() with no points is an empty box",
                 mutest_bool_value (graphene_box2d_equal (b, graphene_box2d_empty ())),
                 mutest_to_be_true,
                 NULL);

  graphene_box2d_free (b);
}

static void
box2d_init_from_vectors (mutest_spec_t *spec)
{
  graphene_point_t points[] = {
    GRAPHENE_POINT_INIT (0.f, 0.f),
    GRAPHENE_POINT_INIT (1.f, 1.f),
    GRAPHENE_POINT_INIT (2.f, 2.f),
  };
  graphene_vec2_t *vectors;
  graphene_point_t min, max;
  graphene_box2d_t *b;

  vectors = malloc (sizeof (graphene_vec2_t) * 3);
  graphene_point_to_vec2 (&points[0], &vectors[0]);
  graphene_point_to_vec2 (&points[1], &vectors[1]);
  graphene_point_to_vec2 (&points[2], &vectors[2]);

  b = graphene_box2d_init_from_vectors (graphene_box2d_alloc (), 3, vectors);
  graphene_box2d_get_min (b, &min);
  graphene_box2d_get_max (b, &max);
  mutest_expect ("init_from_vectors().min() is (0, 0, 0)",
                 mutest_bool_value (graphene_point_equal (&min, &points[0])),
                 mutest_to_be_true,
                 NULL);
  mutest_expect ("init_from_vectors().max() is (2, 2, 2)",
                 mutest_bool_value (graphene_point_equal (&max, &points[2])),
                 mutest_to_be_true,
                 NULL);

  graphene_box2d_init_from_vectors (b, 1, vectors + 1);
  graphene_box2d_get_min (b, &min);
  graphene_box2d_get_max (b, &max);
  mutest_expect ("init_from_vectors() with one point sets min() to the same point",
                 mutest_bool_value (graphene_point_equal (&min, &points[1])),
                 mutest_to_be_true,
                 NULL);
  mutest_expect ("init_from_vectors() with one point sets max() to the same point",
                 mutest_bool_value (graphene_point_equal (&max, &points[1])),
                 mutest_to_be_true,
                 NULL);

  graphene_box2d_init_from_vectors (b, 0, NULL);
  mutest_expect ("init_from_vectors() with no points is an empty box",
                 mutest_bool_value (graphene_box2d_equal (b, graphene_box2d_empty ())),
                 mutest_to_be_true,
                 NULL);

  graphene_box2d_free (b);
  free (vectors);
}

static void
box2d_size (mutest_spec_t *spec)
{
  graphene_vec2_t size;

  graphene_box2d_get_size (graphene_box2d_zero (), &size);
  mutest_expect ("box2d(zero) to have a size of zero",
                 mutest_bool_value (graphene_vec2_equal (&size, graphene_vec2_zero ())),
                 mutest_to_be_true,
                 NULL);

  mutest_expect ("box2d(zero) to have a width of zero",
                 mutest_float_value (graphene_box2d_get_width (graphene_box2d_zero ())),
                 mutest_to_be_close_to, 0.0, 0.00001,
                 NULL);
  mutest_expect ("box2d(1) to have a width of 1",
                 mutest_float_value (graphene_box2d_get_height (graphene_box2d_one ())),
                 mutest_to_be_close_to, 1.0, 0.00001,
                 NULL);
}

static void
box2d_center (mutest_spec_t *spec)
{
  graphene_point_t zero = GRAPHENE_POINT_INIT (0.0f, 0.0f);
  graphene_point_t half = GRAPHENE_POINT_INIT (0.5f, 0.5f);
  graphene_point_t minus_half = GRAPHENE_POINT_INIT (-0.5f, -0.5f);
  graphene_point_t center;
  graphene_box2d_t b;

  graphene_box2d_init_from_box (&b, graphene_box2d_zero ());
  graphene_box2d_get_center (&b, &center);
  mutest_expect ("box2d(zero).center() to be in (0, 0)",
                 mutest_bool_value (graphene_point_equal (&center, &zero)),
                 mutest_to_be_true,
                 NULL);

  graphene_box2d_init_from_box (&b, graphene_box2d_one ());
  graphene_box2d_get_center (&b, &center);
  mutest_expect ("box2d(1).center() to be in (0.5, 0.5)",
                 mutest_bool_value (graphene_point_near (&center, &half, 0.0001f)),
                 mutest_to_be_true,
                 NULL);

  graphene_box2d_init_from_box (&b, graphene_box2d_one_minus_one ());
  graphene_box2d_get_center (&b, &center);
  mutest_expect ("box2d(1, -1).center() to be in (0, 0)",
                 mutest_bool_value (graphene_point_near (&center, &zero, 0.0001f)),
                 mutest_to_be_true,
                 NULL);

  graphene_box2d_init_from_box (&b, graphene_box2d_minus_one ());
  graphene_box2d_get_center (&b, &center);
  mutest_expect ("box2d(-1).center() to be in (-0.5, -0.5)",
                 mutest_bool_value (graphene_point_near (&center, &minus_half, 0.0001f)),
                 mutest_to_be_true,
                 NULL);

  graphene_box2d_init_from_box (&b, graphene_box2d_empty ());
  graphene_box2d_get_center (&b, &center);
  mutest_expect ("box2d(empty).center() to be in (0, 0)",
                 mutest_bool_value (graphene_point_equal (&center, &zero)),
                 mutest_to_be_true,
                 NULL);
}

static void
box2d_equal (mutest_spec_t *spec)
{
  mutest_expect ("equal(null, null) to be true",
                 mutest_bool_value (graphene_box2d_equal (NULL, NULL)),
                 mutest_to_be_true,
                 NULL);

  mutest_expect ("equal(null, box) to be false",
                 mutest_bool_value (graphene_box2d_equal (NULL, graphene_box2d_zero ())),
                 mutest_to_be_false,
                 NULL);

  mutest_expect ("equal(box, null) to be false",
                 mutest_bool_value (graphene_box2d_equal (graphene_box2d_zero (), NULL)),
                 mutest_to_be_false,
                 NULL);

  mutest_expect ("equal(box, box) to be true",
                 mutest_bool_value (graphene_box2d_equal (graphene_box2d_zero (), graphene_box2d_zero ())),
                 mutest_to_be_true,
                 NULL);
}

static void
box2d_union (mutest_spec_t *spec)
{
  graphene_box2d_t res;

  graphene_box2d_union (graphene_box2d_zero (), graphene_box2d_zero (), &res);
  mutest_expect ("union(zero, zero) to be equal to zero",
                 mutest_bool_value (graphene_box2d_equal (&res, graphene_box2d_zero ())),
                 mutest_to_be_true,
                 NULL);

  graphene_box2d_union (graphene_box2d_zero (), graphene_box2d_one (), &res);
  mutest_expect ("union(zero, one) to be equal to one",
                 mutest_bool_value (graphene_box2d_equal (&res, graphene_box2d_one ())),
                 mutest_to_be_true,
                 NULL);

  graphene_box2d_union (graphene_box2d_minus_one (), graphene_box2d_one (), &res);
  mutest_expect ("union(-1, 1) to be equal to (-1, 1)",
                 mutest_bool_value (graphene_box2d_equal (&res, graphene_box2d_one_minus_one ())),
                 mutest_to_be_true,
                 NULL);
}

static void
box2d_intersection (mutest_spec_t *spec)
{
  graphene_box2d_t res;
  graphene_box2d_t left, right;
  graphene_box2d_t top, bottom;

  graphene_box2d_intersection (graphene_box2d_empty (), graphene_box2d_zero (), &res);
  mutest_expect ("intersection(empty, zero) to be empty",
                 mutest_bool_value (graphene_box2d_equal (&res, graphene_box2d_empty ())),
                 mutest_to_be_true,
                 NULL);

  graphene_box2d_intersection (graphene_box2d_one_minus_one (), graphene_box2d_one (), &res);
  mutest_expect ("intersection((-1, 1), (1, 1)) to be (1, 1)",
                 mutest_bool_value (graphene_box2d_equal (&res, graphene_box2d_one ())),
                 mutest_to_be_true,
                 NULL);

  graphene_box2d_intersection (graphene_box2d_infinite (), graphene_box2d_one (), &res);
  mutest_expect ("intersection(inf, (1, 1)) to be (1, 1)",
                 mutest_bool_value (graphene_box2d_equal (&res, graphene_box2d_one ())),
                 mutest_to_be_true,
                 NULL);

  graphene_box2d_init (&left,
                       &GRAPHENE_POINT_INIT (5.f, 0.f),
                       &GRAPHENE_POINT_INIT (6.f, 1.f));
  graphene_box2d_init (&right,
                       &GRAPHENE_POINT_INIT (7.f, 0.f),
                       &GRAPHENE_POINT_INIT (8.f, 1.f));
  mutest_expect ("horizontally separated boxes don't intersect",
                 mutest_bool_value (graphene_box2d_intersection (&left, &right, NULL)),
                 mutest_to_be_false,
                 NULL);

  graphene_box2d_init (&top,
                       &GRAPHENE_POINT_INIT (0.f, 5.f),
                       &GRAPHENE_POINT_INIT (1.f, 6.f));
  graphene_box2d_init (&bottom,
                       &GRAPHENE_POINT_INIT (0.f, 7.f),
                       &GRAPHENE_POINT_INIT (1.f, 8.f));
  mutest_expect ("vertically separated boxes don't intersect",
                 mutest_bool_value (graphene_box2d_intersection (&top, &bottom, NULL)),
                 mutest_to_be_false,
                 NULL);
}

static void
box2d_expand_by_point (mutest_spec_t *spec)
{
  graphene_box2d_t b;
  graphene_vec2_t size, tmp;
  graphene_point_t zero = GRAPHENE_POINT_INIT_ZERO;
  graphene_point_t one = GRAPHENE_POINT_INIT (1.f, 1.f);
  graphene_point_t minus_one = GRAPHENE_POINT_INIT (-1.f, -1.f);

  graphene_box2d_init_from_box (&b, graphene_box2d_zero ());
  graphene_box2d_get_size (&b, &size);
  mutest_expect ("initial size to be zero",
                 mutest_bool_value (graphene_vec2_equal (&size, graphene_vec2_zero ())),
                 mutest_to_be_true,
                 NULL);

  graphene_box2d_expand (&b, &zero, &b);
  graphene_box2d_get_size (&b, &size);
  mutest_expect ("expand(zero) does not expand",
                 mutest_bool_value (graphene_vec2_equal (&size, graphene_vec2_zero ())),
                 mutest_to_be_true,
                 NULL);

  graphene_box2d_expand (&b, &one, &b);
  graphene_box2d_get_size (&b, &size);
  mutest_expect ("expand(1) expands to 1",
                 mutest_bool_value (graphene_vec2_equal (&size, graphene_vec2_one ())),
                 mutest_to_be_true,
                 NULL);

  graphene_box2d_expand (&b, &minus_one, &b);
  graphene_box2d_get_size (&b, &size);
  mutest_expect ("expand(-1) expands to 2",
                 mutest_bool_value (graphene_vec2_equal (&size, graphene_vec2_init (&tmp, 2.f, 2.f))),
                 mutest_to_be_true,
                 NULL);
}

static void
box2d_expand_by_vector (mutest_spec_t *spec)
{
  graphene_box2d_t b;
  graphene_vec2_t size, tmp;
  graphene_vec2_t minus_one;

  graphene_box2d_init_from_box (&b, graphene_box2d_zero ());
  graphene_box2d_get_size (&b, &size);
  mutest_expect ("initial size to be zero",
                 mutest_bool_value (graphene_vec2_equal (&size, graphene_vec2_zero ())),
                 mutest_to_be_true,
                 NULL);

  graphene_box2d_expand_vec2 (&b, graphene_vec2_zero (), &b);
  graphene_box2d_get_size (&b, &size);
  mutest_expect ("expand(zero) does not expand",
                 mutest_bool_value (graphene_vec2_equal (&size, graphene_vec2_zero ())),
                 mutest_to_be_true,
                 NULL);

  graphene_box2d_expand_vec2 (&b, graphene_vec2_one (), &b);
  graphene_box2d_get_size (&b, &size);
  mutest_expect ("expand(1) expands to 1",
                 mutest_bool_value (graphene_vec2_equal (&size, graphene_vec2_one ())),
                 mutest_to_be_true,
                 NULL);

  graphene_vec2_init (&minus_one, -1.f, -1.f);
  graphene_box2d_expand_vec2 (&b, &minus_one, &b);
  graphene_box2d_get_size (&b, &size);
  mutest_expect ("expand(-1) expands to 2",
                 mutest_bool_value (graphene_vec2_equal (&size, graphene_vec2_init (&tmp, 2.f, 2.f))),
                 mutest_to_be_true,
                 NULL);
}

static void
box2d_expand_by_scalar (mutest_spec_t *spec)
{
  graphene_box2d_t b;
  graphene_vec2_t size, tmp;

  graphene_box2d_init_from_box (&b, graphene_box2d_zero ());
  graphene_box2d_get_size (&b, &size);
  mutest_expect ("initial size to be zero",
                 mutest_bool_value (graphene_vec2_equal (&size, graphene_vec2_zero ())),
                 mutest_to_be_true,
                 NULL);

  graphene_box2d_expand_scalar (&b, 0, &b);
  graphene_box2d_get_size (&b, &size);
  mutest_expect ("expand(zero) does not expand",
                 mutest_bool_value (graphene_vec2_equal (&size, graphene_vec2_zero ())),
                 mutest_to_be_true,
                 NULL);

  graphene_box2d_expand_scalar (&b, 1, &b);
  graphene_box2d_get_size (&b, &size);
  mutest_expect ("expand(1) expands to 2",
                 mutest_bool_value (graphene_vec2_equal (&size, graphene_vec2_init (&tmp, 2.f, 2.f))),
                 mutest_to_be_true,
                 NULL);
}

static void
box2d_contains_point (mutest_spec_t *spec)
{
  graphene_box2d_t b;
  graphene_vec2_t points[8];
  graphene_point_t center;
  graphene_point_t check;
  unsigned int i;

  graphene_box2d_init_from_box (&b, graphene_box2d_one_minus_one ());
  graphene_box2d_get_center (&b, &center);
  mutest_expect ("box to contain its center",
                 mutest_bool_value (graphene_box2d_contains_point (&b, &center)),
                 mutest_to_be_true,
                 NULL);

  graphene_box2d_get_vertices (&b, points);
  for (i = 0; i < 4; i++)
    {
      char desc[128];

      graphene_point_init_from_vec2 (&check, &points[i]);
      snprintf (desc, 128, "box2d to contain its vertices [%d]", i);
      mutest_expect (desc,
                     mutest_bool_value (graphene_box2d_contains_point (&b, &check)),
                     mutest_to_be_true,
                     NULL);
    }

  graphene_point_init (&check, -2.f, 2.f);
  mutest_expect ("box to not contain a point outside its area",
                  mutest_bool_value (graphene_box2d_contains_point (&b, &check)),
                  mutest_to_be_false,
                  NULL);
}

static void
box2d_contains_box (mutest_spec_t *spec)
{
  graphene_box2d_t a, b;

  graphene_box2d_init_from_box (&a, graphene_box2d_one ());
  graphene_box2d_init_from_box (&b, graphene_box2d_minus_one ());
  mutest_expect ("box(1) to not contain box(-1)",
                 mutest_bool_value (graphene_box2d_contains_box (&a, &b)),
                 mutest_to_be_false,
                 NULL);

  graphene_box2d_init_from_box (&a, graphene_box2d_one_minus_one ());
  graphene_box2d_init_from_box (&b, graphene_box2d_one ());
  mutest_expect ("box(-1, 1) to contain box(1)",
                 mutest_bool_value (graphene_box2d_contains_box (&a, &b)),
                 mutest_to_be_true,
                 NULL);

  graphene_box2d_init_from_box (&a, graphene_box2d_infinite ());
  mutest_expect ("infinite box to contain any box",
                 mutest_bool_value (graphene_box2d_contains_box (&a, &b)),
                 mutest_to_be_true,
                 NULL);

  graphene_box2d_init_from_box (&a, graphene_box2d_empty ());
  mutest_expect ("empty box to not contain any box",
                 mutest_bool_value (graphene_box2d_contains_box (&a, &b)),
                 mutest_to_be_false,
                 NULL);
}

static void
box2d_suite (mutest_suite_t *suite)
{
  mutest_it ("initializes min/max points", box2d_init_min_max);
  mutest_it ("initializes from points", box2d_init_from_points);
  mutest_it ("initializes from vectors", box2d_init_from_vectors);
  mutest_it ("has the correct sizes", box2d_size);
  mutest_it ("has the correct center point", box2d_center);
  mutest_it ("has equality", box2d_equal);
  mutest_it ("computes unions", box2d_union);
  mutest_it ("computes intersections", box2d_intersection);
  mutest_it ("expands by point", box2d_expand_by_point);
  mutest_it ("expands by vector", box2d_expand_by_vector);
  mutest_it ("expands by scalar", box2d_expand_by_scalar);
  mutest_it ("contains point", box2d_contains_point);
  mutest_it ("contains box", box2d_contains_box);
}

MUTEST_MAIN (
  mutest_describe ("graphene_box2d_t", box2d_suite);
)
