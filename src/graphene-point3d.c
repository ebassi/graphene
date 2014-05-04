/* graphene-point3d.hc: Point in 3D space
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

#include <math.h>

#include "graphene-point3d.h"
#include "graphene-vec3.h"
#include "graphene-simd4f.h"

graphene_point3d_t *
graphene_point3d_alloc (void)
{
  return g_slice_new (graphene_point3d_t);
}

void
graphene_point3d_free (graphene_point3d_t *p)
{
  if (G_LIKELY (p != NULL))
    g_slice_free (graphene_point3d_t, p);
}

graphene_point3d_t *
graphene_point3d_init (graphene_point3d_t *p,
                       float               x,
                       float               y,
                       float               z)
{
  g_return_val_if_fail (p != NULL, NULL);

  p->x = x;
  p->y = y;
  p->z = z;

  return p;
}

graphene_point3d_t *
graphene_point3d_init_from_vec3 (graphene_point3d_t    *p,
                                 const graphene_vec3_t *v)
{
  g_return_val_if_fail (p != NULL, NULL);
  g_return_val_if_fail (v != NULL, p);

  p->x = graphene_simd4f_get_x (v->value);
  p->y = graphene_simd4f_get_y (v->value);
  p->z = graphene_simd4f_get_z (v->value);

  return p;
}

void
graphene_point3d_to_vec3 (const graphene_point3d_t *p,
                          graphene_vec3_t          *v)
{
  g_return_if_fail (p != NULL);
  g_return_if_fail (v != NULL);

  v->value = graphene_simd4f_init (p->x, p->y, p->z, 0.f);
}

void
graphene_point3d_scale (const graphene_point3d_t *p,
                        float                     factor,
                        graphene_point3d_t       *res)
{
  graphene_simd4f_t v;

  g_return_if_fail (p != NULL);
  g_return_if_fail (res != NULL);

  v = graphene_simd4f_init (p->x, p->y, p->z, 0.f);
  v = graphene_simd4f_mul (v, graphene_simd4f_splat (factor));

  graphene_point3d_init (res,
                         graphene_simd4f_get_x (v),
                         graphene_simd4f_get_y (v),
                         graphene_simd4f_get_z (v));
}

void
graphene_point3d_cross (const graphene_point3d_t *a,
                        const graphene_point3d_t *b,
                        graphene_point3d_t       *res)
{
  graphene_simd4f_t av, bv, resv;

  g_return_if_fail (a != NULL && b != NULL);
  g_return_if_fail (res != NULL);

  av = graphene_simd4f_init (a->x, a->y, a->z, 0.f);
  bv = graphene_simd4f_init (b->x, b->y, b->z, 0.f);
  resv = graphene_simd4f_cross3 (av, bv);

  res->x = graphene_simd4f_get_x (resv);
  res->y = graphene_simd4f_get_y (resv);
  res->z = graphene_simd4f_get_z (resv);
}

float
graphene_point3d_dot (const graphene_point3d_t *a,
                      const graphene_point3d_t *b)
{
  graphene_simd4f_t av, bv, resv;

  g_return_val_if_fail (a != NULL && b != NULL, 0.f);

  av = graphene_simd4f_init (a->x, a->y, a->z, 0.f);
  bv = graphene_simd4f_init (b->x, b->y, b->z, 0.f);
  resv = graphene_simd4f_dot3 (av, bv);

  return graphene_simd4f_get_x (resv);
}

float
graphene_point3d_length (const graphene_point3d_t *p)
{
  graphene_simd4f_t res;

  g_return_val_if_fail (p != NULL, 0.f);

  res = graphene_simd4f_init (p->x, p->y, p->z, 0.f);

  return graphene_simd4f_get_x (graphene_simd4f_length3 (res));
}

void
graphene_point3d_normalize (const graphene_point3d_t *p,
                            graphene_point3d_t       *res)
{
  graphene_simd4f_t v;

  g_return_if_fail (p != NULL);
  g_return_if_fail (res != NULL);

  v = graphene_simd4f_init (p->x, p->y, p->z, 0.f);
  v = graphene_simd4f_normalize3 (v);

  res->x = graphene_simd4f_get_x (v);
  res->y = graphene_simd4f_get_x (v);
  res->z = graphene_simd4f_get_x (v);
}

void
graphene_point3d_interpolate (const graphene_point3d_t *a,
                              const graphene_point3d_t *b,
                              double                    factor,
                              graphene_point3d_t       *res)
{
  g_return_if_fail (a != NULL && b != NULL);
  g_return_if_fail (res != NULL);

  res->x = a->x + (b->x - a->x) * factor;
  res->y = a->y + (b->y - a->y) * factor;
  res->z = a->z + (b->z - a->z) * factor;
}
