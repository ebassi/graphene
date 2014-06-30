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

#include "graphene-private.h"
#include "graphene-alloc-private.h"

#if defined(HAVE_POSIX_MEMALIGN) && !defined(_XOPEN_SOURCE)
# define _XOPEN_SOURCE 600
#endif

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

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
void *
graphene_alloc (size_t size,
                size_t number,
                size_t alignment)
{
  void *res;
  size_t max_size = (size_t) -1;
  size_t real_size;
  int err;

  if (size == 0 || number == 0)
    return NULL;

#ifndef G_DISABLE_ASSERT
  if (size > 0 && number > max_size / size)
    {
      fprintf (stderr,
               "Overflow in the allocation of (%lu x %lu) bytes\n",
               (unsigned long) size,
               (unsigned long) number);
      abort ();
    }
#endif

  real_size = size * number;

#if defined(HAVE_POSIX_MEMALIGN)
  err = posix_memalign (&res, alignment, real_size);
#elif defined(HAVE_ALIGNED_ALLOC)
  /* real_size must be a multiple of alignment */
  if (real_size % alignment != 0)
    {
      size_t offset = real_size % alignment;
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

#ifndef G_DISABLE_ASSERT
  if (err != 0 || res == NULL)
    {
      fprintf (stderr, "Allocation error: %s\n", strerror (err));
      abort ();
    }
#endif

  return res;
}

/*< private >
 * graphene_free:
 * @mem: the memory to deallocate
 *
 * Frees the memory allocated by graphene_alloc().
 */
void
graphene_free (void *mem)
{
  free (mem);
}
