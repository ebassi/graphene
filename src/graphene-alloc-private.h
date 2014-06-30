/* graphene-alloc-private.h: aligned allocator
 *
 * copyright © 2014  emmanuele bassi
 *
 * permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "software"), to deal
 * in the software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the software, and to permit persons to whom the software is
 * furnished to do so, subject to the following conditions:
 *
 * the above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the software.
 *
 * the software is provided "as is", without warranty of any kind, express or
 * implied, including but not limited to the warranties of merchantability,
 * fitness for a particular purpose and noninfringement. in no event shall the
 * authors or copyright holders be liable for any claim, damages or other
 * liability, whether in an action of contract, tort or otherwise, arising from,
 * out of or in connection with the software or the use or other dealings in
 * the software.
 */

#ifndef __GRAPHENE_ALLOC_PRIVATE_H__
#define __GRAPHENE_ALLOC_PRIVATE_H__

#include "graphene-types.h"

#include <stddef.h>

GRAPHENE_BEGIN_DECLS

void *          graphene_alloc          (size_t size,
                                         size_t number,
                                         size_t alignment);

void            graphene_free           (void  *mem);

GRAPHENE_END_DECLS

#endif /* __GRAPHENE_ALLOC_PRIVATE_H__ */
