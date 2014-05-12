/* graphene-gobject.c: Shared GObject types
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

#include "graphene-gobject.h"

#define GRAPHENE_DEFINE_BOXED_TYPE(TypeName,type_name) \
static type_name ## _t * \
type_name ## _copy_internal (type_name ## _t *v) \
{ \
  type_name ## _t *res = NULL; \
  if (G_LIKELY (v != NULL)) \
    { \
      res = type_name ## _alloc (); \
      *res = *v; \
    } \
  return res; \
} \
\
GType \
type_name ## _get_type (void) \
{ \
  static volatile gsize graphene_define_id__volatile = 0; \
  if (g_once_init_enter (&graphene_define_id__volatile)) \
    { \
      GType graphene_define_id = \
        g_boxed_type_register_static (g_intern_static_string (#TypeName), \
                                      (GBoxedCopyFunc) type_name ## _copy_internal, \
                                      (GBoxedFreeFunc) type_name ## _free); \
      g_once_init_leave (&graphene_define_id__volatile, graphene_define_id); \
    } \
  return graphene_define_id__volatile; \
}

GRAPHENE_DEFINE_BOXED_TYPE (GraphenePoint, graphene_point)

GRAPHENE_DEFINE_BOXED_TYPE (GraphenePoint3D, graphene_point3d)

GRAPHENE_DEFINE_BOXED_TYPE (GrapheneSize, graphene_size)

GRAPHENE_DEFINE_BOXED_TYPE (GrapheneRect, graphene_rect)

GRAPHENE_DEFINE_BOXED_TYPE (GrapheneVec2, graphene_vec2)

GRAPHENE_DEFINE_BOXED_TYPE (GrapheneVec3, graphene_vec3)

GRAPHENE_DEFINE_BOXED_TYPE (GrapheneVec4, graphene_vec4)

GRAPHENE_DEFINE_BOXED_TYPE (GrapheneQuad, graphene_quad)

GRAPHENE_DEFINE_BOXED_TYPE (GrapheneQuaternion, graphene_quaternion)

GRAPHENE_DEFINE_BOXED_TYPE (GrapheneMatrix, graphene_matrix)
