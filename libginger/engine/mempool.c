/*
Checksum: 4288353414      (mempool.c)
*/

/*
 *    This file is part of:
 *
 *     Ginger -- an INteractive Graph EditoR for the X Window System.
 *                          COPYRIGHT (C) 1996 
 *
 *    Martin K. Molz and Niels C. Larse, Aalborg University, Denmark.
 *
 *    This program is free software; you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation; either version 2 of the License, or
 *    (at your option) any later version.
 *
 *    This program is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with Bison; see the file COPYING.  If not, write to
 *    the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 *    Boston, MA 02110-1301, USA.
 */

/*
 * region-allocator.c -- region based memory allocator.
 *
 * Copyright (c) 2001-2004, NLnet Labs. All rights reserved.
 *
 * See LICENSE for the license:
 *
 * Copyright (c) 2001-2004, NLnet Labs. All rights reserved.
 * 
 * This software is open source.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 
 * Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 * 
 * Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 * 
 * Neither the name of the NLNET LABS nor the names of its contributors may
 * be used to endorse or promote products derived from this software without
 * specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
 * TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 * 
 * The NSD was primarily developed by NLnet Labs on request from and close
 * cooperation with RIPE NCC, as an alternative name server software to be
 * run on the root name server RIPE NCC operates. http://www.nlnetlabs.nl
 *
 * [refurbish'ed from k&r book on `a storage allocator' page 185]
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "gingerprotos.h"

/*
 * Memory chunk size.
 * Depends on machine, cpu cache size etc.
 */
#define CHUNK_SIZE 1*1024
/* #define CHUNK_SIZE 4*1024 */
/* #define CHUNK_SIZE 8*1024 */
/* #define CHUNK_SIZE 16*1024 */
/* #define CHUNK_SIZE 32*1024 */
/* #define CHUNK_SIZE 64*1024 */

/*
 * Adjust malloc amount depending on machine.
 */
#define ALIGN_UP(x, s)     (((x) + s - 1) & (~(s - 1)))

/*
 * Larger memory chunk size.
 */
#define LARGE_OBJECT_SIZE  (CHUNK_SIZE / 8)

/*
 * Data type for alignment.
 */
#define ALIGNMENT          (sizeof(void *))

/*
 * 
 */
#define CLEANUP_COUNT 16

/*
 * create pool with `new_pool()', `pmalloc()' instead of malloc
 * and `delete_pool()' to free allocated memory in pool at once. 
 *
 * pool p = (pool)0;
 * void *mem1;
 * void *mem2;
 * p = new_pool();         // new pool area
 * mem1 = pmalloc (p,1234); // alloc memory
 * mem2 = pmalloc (p,2345); // alloc memory
 * p = delete_pool(p);     // delete all alloced memory
 *
 * the lower level malloc() is supposed to
 * return zeroed memory, like calloc() .
 */

typedef struct region region_type;

/* */
static region_type *region_create (void *(*allocator) (size_t size),
				   void (*deallocator) (void *));
static void region_destroy (region_type * region);
static void region_set_current (region_type * region);
static region_type *region_get_current (void);
static size_t region_add_cleanup (region_type * region,
				  void (*action) (void *), void *data);
static void *region_alloc (region_type * region, size_t size);
static void *region_alloc_init (region_type * region, const void *init,
				size_t size);
static void *region_alloc_zero (region_type * region, size_t size);
static void *region_alloc_current (size_t size);
static void region_free_all (region_type * region);
static char *region_strdup (region_type * region, const char *string);
static void region_dump_stats (region_type * region, FILE * stream);

/* Machine dependacy. */
static size_t chunk_size = CHUNK_SIZE;

/* */
static int justzero = 0;
static int justone = 1;

/* */
int
pool_init (void)
{
  region_type *x;
  char *s;
  /* Pacify */
  if (justzero)
    {
      justone = 1;
      region_set_current (x);
      x = region_get_current ();
      region_alloc_zero (x, 1);
      region_alloc_current (1);
      s = region_strdup (x, "");
      region_dump_stats (x, (FILE *) 0);
    }
  /* Machine dep. */
  chunk_size = CHUNK_SIZE;
  /* Do not malloc()/free() at this stage */
  return (0);
}

/* */
int
pool_deinit (void)
{
  /* */
  return (0);
}

/* New memory pool. */
pool
pool_create (void)
{
  region_type *p = (region_type *) 0;
  p = region_create (mmalloc, mmfree);
  return ((pool) p);
}

/* Malloc in a memory pool. */
void *
pool_malloc (pool p, size_t n)
{
  void *pp = (void *) 0;
  pp = (void *) region_alloc ((region_type *) p, n);
  return ((void *) pp);
}

/* Free in a memory pool. */
pool
pool_delete (pool pp)
{
  if (pp)
    {
      region_destroy ((region_type *) pp);
    }
  return ((pool) 0);
}

/* */
int
pool_check (FILE * stream)
{
  pool p;
  pool q;
  void *m;
  p = pool_create ();
  m = pool_malloc (p, 100);
  m = pool_malloc (p, 1024 + 10);
  m = pool_malloc (p, 1024 * 1024);
  q = pool_create ();
  m = pool_malloc (q, 100);
  p = pool_delete (p);
  q = pool_delete (q);
  fflush (stream);
  return (0);
}

/*
 * Create a new region.
 *
 * region_type *region_create (void *(*allocator) (size_t),
 * 			    void (*deallocator) (void *));
 *
 * Destroy REGION.  All memory associated with REGION is freed as if
 * region_free_all was called.
 *
 * void region_destroy (region_type * region);
 *
 * Add a cleanup to REGION.  ACTION will be called with DATA as
 * parameter when the region is freed or destroyed.
 *
 * Returns 0 on failure.
 *
 * size_t region_add_cleanup (region_type * region,
 * 			   void (*action) (void *), void *data);
 *
 * Allocate SIZE bytes of memory inside REGION.  The memory is
 * deallocated when region_free_all is called for this region.
 *
 * void *region_alloc (region_type * region, size_t size);
 *
 * Allocate SIZE bytes of memory inside REGION and copy INIT into it.
 * The memory is deallocated when region_free_all is called for this
 * region.
 *
 * void *region_alloc_init (region_type * region, const void *init, size_t size); 
 *
 * Allocate SIZE bytes of memory inside REGION that are initialized to
 * 0.  The memory is deallocated when region_free_all is called for
 * this region.
 *
 * void *region_alloc_zero (region_type * region, size_t size);
 *
 * Run the cleanup actions and free all memory associated with REGION.
 *
 * void region_free_all (region_type * region);
 * 
 * Duplicate STRING and allocate the result in REGION.
 *
 * char *region_strdup (region_type * region, const char *string);
 *
 * Set the current active region to REGION.
 *
 * void region_set_current (region_type * region);
 *
 * Return the current active region.
 *
 * region_type *region_get_current (void);
 *
 * Allocate SIZE bytes of memory inside the currently active region.
 * The memory is deallocated when region_free_all is called for the
 * active region.  This is provided as an easy replacement of malloc.
 *
 * void *region_alloc_current (size_t size);
 *
 * Print some REGION statistics to OUT.
 *
 * void region_dump_stats (region_type * region, FILE * out);
 */

/* */
typedef struct cleanup cleanup_type;

/* */
struct cleanup
{
  void (*action) (void *);
  void *data;
};

/* */
struct region
{
  size_t total_allocated;
  size_t small_objects;
  size_t large_objects;
  size_t chunk_count;
  size_t unused_space;
  size_t allocated;
  void *initial_data;
  void *data;
  void *(*allocator) (size_t);
  void (*deallocator) (void *);
  size_t maximum_cleanup_count;
  size_t cleanup_count;
  cleanup_type *cleanups;
};

/* */
static region_type *current_region = (region_type *) 0;

/* */
static region_type *
region_create (void *(*allocator) (size_t size), void (*deallocator) (void *))
{
  region_type *result = (region_type *) 0;
  void *x = (void *) 0;

  result = (region_type *) allocator (sizeof (region_type));

  if (result == (region_type *) 0)
    {
      return ((region_type *) 0);
    }

  x = (void *) memset (result, 0, (sizeof (region_type)));

  result->total_allocated = 0;
  result->small_objects = 0;
  result->large_objects = 0;
  result->chunk_count = 1;
  result->unused_space = 0;

  result->allocated = 0;

  result->data = (void *) allocator (chunk_size);

  if (result->data == (void *) 0)
    {
      deallocator (result);
      result = (region_type *) 0;
      return ((region_type *) 0);
    }

  x = (void *) memset (result->data, 0, (chunk_size));

  result->initial_data = result->data;

  result->allocator = allocator;
  result->deallocator = deallocator;

  result->maximum_cleanup_count = CLEANUP_COUNT;

  result->cleanup_count = 0;

  result->cleanups =
    (cleanup_type *) allocator (result->maximum_cleanup_count *
				sizeof (cleanup_type));

  if (result->cleanups == (cleanup_type *) 0)
    {
      deallocator (result->data);
      result->data = (void *) 0;
      deallocator (result);
      result = (region_type *) 0;
      return ((region_type *) 0);
    }

  x = (void *) memset (result->cleanups, 0,
		       (result->maximum_cleanup_count *
			sizeof (cleanup_type)));

  return ((region_type *) result);
}

/* */
static void
region_destroy (region_type * region)
{
  void (*deallocator) (void *);

  if (region == (region_type *) 0)
    {
      return;
    }

  deallocator = region->deallocator;

  region_free_all (region);

  deallocator (region->cleanups);
  region->cleanups = (cleanup_type *) 0;

  deallocator (region->initial_data);
  region->initial_data = (void *) 0;

  deallocator (region);
  region = (region_type *) 0;

  return;
}

/* */
static void
region_set_current (region_type * region)
{

  if (region)
    {
      current_region = region;
    }
  else
    {
      /* Empty region. */
      current_region = region;
    }

  return;
}

/* */
static region_type *
region_get_current (void)
{
  return ((region_type *) current_region);
}

/* */
static size_t
region_add_cleanup (region_type * region, void (*action) (void *), void *data)
{
  cleanup_type *cleanups = (cleanup_type *) 0;
  void *x = (void *) 0;

  if (region->cleanup_count >= region->maximum_cleanup_count)
    {

      cleanups =
	(cleanup_type *) region->allocator (2 *
					    region->maximum_cleanup_count *
					    sizeof (cleanup_type));

      if (cleanups == (cleanup_type *) 0)
	{
	  return ((size_t) 0);
	}

      x = (void *) memset (cleanups, 0,
			   (2 * region->maximum_cleanup_count *
			    sizeof (cleanup_type)));

      x = (void *) memmove (cleanups, region->cleanups,
			    region->cleanup_count * sizeof (cleanup_type));

      region->deallocator (region->cleanups);

      region->cleanups = (cleanup_type *) 0;

      region->cleanups = cleanups;
      region->maximum_cleanup_count = (region->maximum_cleanup_count * 2);
    }

  region->cleanups[region->cleanup_count].action = action;
  region->cleanups[region->cleanup_count].data = data;

  region->cleanup_count = region->cleanup_count + 1;

  return ((size_t) region->cleanup_count);
}

/* */
static void *
region_alloc (region_type * region, size_t size)
{
  size_t tmpx = 0;
  size_t aligned_size = 0;
  void *result = (void *) 0;
  void *chunk = (void *) 0;
  void *x = (void *) 0;
  size_t xsize = 0;

  if (size == 0)
    {
      /* malloc(0) into malloc(1). */
      xsize = 1;
    }
  else
    {
      xsize = size;
    }

  aligned_size = ALIGN_UP (xsize, ALIGNMENT);

  if (aligned_size >= LARGE_OBJECT_SIZE)
    {

      result = (void *) region->allocator (xsize);

      if (result == (void *) 0)
	{
	  return ((void *) 0);
	}

      x = (void *) memset (result, 0, xsize);

      tmpx = region_add_cleanup (region, region->deallocator, result);

      if (tmpx == (size_t) 0)
	{
	  region->deallocator (result);
	  result = (void *) 0;
	  return ((void *) 0);
	}

      region->total_allocated = region->total_allocated + xsize;

      region->large_objects = region->large_objects + 1;

      return ((void *) result);
    }

  if ((region->allocated + aligned_size) > chunk_size)
    {

      chunk = (void *) region->allocator (chunk_size);

      if (chunk == (void *) 0)
	{
	  return ((void *) 0);
	}

      x = (void *) memset (chunk, 0, chunk_size);

      region->chunk_count = region->chunk_count + 1;
      region->unused_space =
	region->unused_space + (chunk_size - region->allocated);

      region_add_cleanup (region, region->deallocator, chunk);

      region->allocated = 0;
      region->data = (void *) chunk;
    }

  result = region->data + region->allocated;
  region->allocated = region->allocated + aligned_size;

  region->total_allocated = region->total_allocated + aligned_size;
  region->unused_space = region->unused_space + aligned_size - xsize;

  region->small_objects = region->small_objects + 1;

  return ((void *) result);
}

/* */
static void *
region_alloc_init (region_type * region, const void *init, size_t size)
{
  void *result = (void *) 0;
  void *x = (void *) 0;

  result = region_alloc (region, size);

  if (result == (void *) 0)
    {
      return ((void *) 0);
    }

  x = (void *) memmove (result, init, size);

  return ((void *) result);
}

/* */
static void *
region_alloc_zero (region_type * region, size_t size)
{
  void *result = (void *) 0;
  void *x = (void *) 0;

  result = (void *) region_alloc (region, size);

  if (result == (void *) 0)
    {
      return ((void *) 0);
    }

  x = (void *) memset (result, 0, size);

  return ((void *) result);
}

/* */
static void *
region_alloc_current (size_t size)
{
  return ((void *) region_alloc (current_region, size));
}

/* */
static void
region_free_all (region_type * region)
{
  size_t i = 0;

  i = region->cleanup_count;

  while (i > 0)
    {
      i = i - 1;
      region->cleanups[i].action (region->cleanups[i].data);
    }

  region->data = region->initial_data;
  region->cleanup_count = 0;
  region->allocated = 0;

  region->total_allocated = 0;
  region->small_objects = 0;
  region->large_objects = 0;
  region->chunk_count = 1;
  region->unused_space = 0;

  return;
}

/* */
static char *
region_strdup (region_type * region, const char *string)
{
  return (char *) region_alloc_init (region, string, strlen (string) + 1);
}

/* */
static void
region_dump_stats (region_type * region, FILE * stream)
{

  fprintf (stream, "%lu objects \n",
	   (unsigned long) (region->small_objects + region->large_objects));
  fprintf (stream, "%lu small \n", (unsigned long) region->small_objects);
  fprintf (stream, "%lu large \n", (unsigned long) region->large_objects);
  fprintf (stream, "%lu bytes allocated \n",
	   (unsigned long) region->total_allocated);
  fprintf (stream, "%lu wasted \n", (unsigned long) region->unused_space);
  fprintf (stream, "%lu chunks \n", (unsigned long) region->chunk_count);
  fprintf (stream, "%lu cleanups \n", (unsigned long) region->cleanup_count);

  return;
}

/* www.python.org memory subsystem can also be used for this */

/* End. */
