/* graphene-quad.c: Quad
 *
 * Copyright © 2014  Emmanuele Bassi
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include "config.h"

#include "graphene-quad.h"
#include "graphene-rect.h"
#include "graphene-line-segment.h"

graphene_quad_t *
graphene_quad_alloc (void)
{
  return calloc (1, sizeof (graphene_quad_t));
}

void
graphene_quad_free (graphene_quad_t *q)
{
  free (q);
}

graphene_quad_t *
graphene_quad_init (graphene_quad_t        *q,
                    const graphene_point_t *p1,
                    const graphene_point_t *p2,
                    const graphene_point_t *p3,
                    const graphene_point_t *p4)
{
  g_return_val_if_fail (q != NULL, NULL);
  g_return_val_if_fail (p1 != NULL && p2 != NULL && p3 != NULL && p4 != NULL, q);

  q->points[0] = *p1;
  q->points[1] = *p2;
  q->points[2] = *p3;
  q->points[3] = *p4;

  return q;
}

graphene_quad_t *
graphene_quad_init_from_rect (graphene_quad_t       *q,
                              const graphene_rect_t *r)
{
  g_return_val_if_fail (q != NULL, NULL);
  g_return_val_if_fail (r != NULL, q);

  graphene_rect_get_top_left (r, &(q->points[0]));
  graphene_rect_get_top_right (r, &(q->points[1]));
  graphene_rect_get_bottom_right (r, &(q->points[2]));
  graphene_rect_get_bottom_left (r, &(q->points[3]));

  return q;
}

gboolean
graphene_quad_contains (const graphene_quad_t  *q,
                        const graphene_point_t *p)
{
  graphene_line_segment_t l1, l2, l3, l4;

  g_return_val_if_fail (q != NULL, FALSE);
  g_return_val_if_fail (p != NULL, FALSE);

  l1 = graphene_line_segment_init (&q->points[0], &q->points[1]);
  l2 = graphene_line_segment_init (&q->points[1], &q->points[2]);
  l3 = graphene_line_segment_init (&q->points[2], &q->points[3]);
  l4 = graphene_line_segment_init (&q->points[3], &q->points[0]);

  return graphene_line_segment_points_on_same_side (l1, p, &(q->points[2])) &&
         graphene_line_segment_points_on_same_side (l2, p, &(q->points[3])) &&
         graphene_line_segment_points_on_same_side (l3, p, &(q->points[0])) &&
         graphene_line_segment_points_on_same_side (l4, p, &(q->points[1]));
}

void
graphene_quad_bounds (const graphene_quad_t *q,
                      graphene_rect_t       *r)
{
  float min_x, max_x;
  float min_y, max_y;
  int i;

  g_return_if_fail (q != NULL);
  g_return_if_fail (r != NULL);

  min_x = max_x = q->points[0].x;
  min_y = max_y = q->points[0].y;

  for (i = 1; i < 4; i += 1)
    {
      min_x = MIN (q->points[i].x, min_x);
      min_y = MIN (q->points[i].y, min_y);

      max_x = MAX (q->points[i].x, max_x);
      max_y = MAX (q->points[i].y, max_y);
    }

  graphene_rect_init (r, min_x, min_y, max_x - min_x, max_y - min_y);
}
