/* graphene-alloc.c: aligned allocator
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

#include "config.h"

#include "graphene-alloc-private.h"

#if defined(HAVE_POSIX_MEMALIGN) && !defined(_XOPEN_SOURCE)
# define _XOPEN_SOURCE 600
#endif

#include <stdlib.h>

/*< private >
 * graphene_alloc:
 * @size: the size of the memory to allocate
 * @number: the multiples of @size to allocate
 * @alignment: the alignment to be enforced, as a power of 2
 *
 * Allocates @number times @size memory, with the given @alignment.
 *
 * If the total requested memory overflows %G_MAXSIZE, this function
 * will abort.
 *
 * If allocation fails, this function will abort, in line with
 * the behaviour of GLib.
 *
 * Returns: (transfer full): the allocated memory
 */
gpointer
graphene_alloc (gsize size,
                gsize number,
                gsize alignment)
{
  gpointer res;
  gsize real_size;
  int err;

#ifndef G_DISABLE_ASSERT
  if (size > G_MAXSIZE / number || number > G_MAXSIZE / size)
    g_error ("Overflow in the allocation of (%" G_GSIZE_FORMAT ", %" G_GSIZE_FORMAT ") bytes\n",
             size,
             number);
#endif

  real_size = size * number;

#if defined(HAVE_POSIX_MEMALIGN)
  err = posix_memalign (&res, alignment, real_size);
#elif defined(HAVE_ALIGNED_ALLOC)
  /* real_size must be a multiple of alignment */
  if (real_size % alignment != 0)
    {
      gsize offset = real_size % alignment;
      real_size += (alignment - offset);
    }

  errno = 0;
  res = aligned_alloc (alignment, real_size);
  err = errno;
#elif defined(HAVE_MEMALIGN)
  errno = 0;
  res = memalign (alignment, real_size);
  err = errno;
#else
  res = malloc (real_size);
#endif

  if (G_UNLIKELY (err != 0 || res == NULL))
    g_error ("%s", g_strerror (err));

  return res;
}

/*< private >
 * graphene_free:
 * @mem: the memory to deallocate
 *
 * Frees the memory allocated by graphene_alloc().
 */
void
graphene_free (gpointer mem)
{
  free (mem);
}
