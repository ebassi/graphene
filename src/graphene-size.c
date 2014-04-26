#include "config.h"

#include "graphene-point.h"

graphene_size_t *
graphene_size_alloc (void)
{
  return g_slice_new (graphene_size_t);
}

void
graphene_size_free (graphene_size_t *s)
{
  if (G_LIKELY (s != NULL))
    g_slice_free (graphene_size_t, s);
}

graphene_size_t *
graphene_size_init (graphene_size_t *s,
                    float            width,
                    float            height)
{
  g_return_val_if_fail (s != NULL, NULL);

  s->width = width;
  s->height = height;

  return s;
}

graphene_size_t *
graphene_size_init_from_size (graphene_size_t       *s,
                              const graphene_size_t *src)
{
  g_return_val_if_fail (s != NULL, NULL);
  g_return_val_if_fail (src != NULL, s);

  *s = *src;

  return s;
}

void
graphene_size_scale (const graphene_size_t *s,
                     float                  factor,
                     graphene_size_t       *res)
{
  g_return_if_fail (s != NULL);
  g_return_if_fail (res != NULL);

  *res = *s;

  res->width *= factor;
  res->height *= factor;
}

void
graphene_size_interpolate (const graphene_size_t *a,
                           const graphene_size_t *b,
                           double                 factor,
                           graphene_size_t       *res)
{
  g_return_if_fail (a != NULL && b != NULL);
  g_return_if_fail (res != NULL);

  res->width = a->width + (b->width - a->width) * factor;
  res->height = a->height + (b->height - a->height) * factor;
}
