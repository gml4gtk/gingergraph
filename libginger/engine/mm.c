/*
Checksum: 1789807977      (mm.c)
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "gingerprotos.h"

/* */
pool world = (pool) 0;
pool uniqstrings = (pool) 0;
pool topgraphpool = (pool) 0;
pool graphpool = (pool) 0;
pool nodepool = (pool) 0;
pool edgepool = (pool) 0;
pool drawpool = (pool) 0;
pool clistpool = (pool) 0;
pool levelpool = (pool) 0;
pool matrixpool = (pool) 0;

/* number of malloc/free */
int mm_free_s = 0;
int mm_malloc_s = 0;

/* amount of memory */
size_t mm_size_t = 0;

/* wrapped malloc() is assumed to return zero'ed memory like calloc() */
void *(*engine_malloc) (size_t n) = 0;

/* wrapped free() is assumed to pass only valid pointers */
void (*engine_free) (void *p) = 0;

/* memory subsystem init */
int
mm_init (void)
{

  world = pool_create ();
  uniqstrings = pool_create ();
  topgraphpool = pool_create ();
  graphpool = pool_create ();
  nodepool = pool_create ();
  edgepool = pool_create ();
  drawpool = pool_create ();
  clistpool = pool_create ();
  levelpool = pool_create ();
  matrixpool = pool_create ();

  return (0);
}

/* memory subsystem de-init */
int
mm_deinit (void)
{

  matrixpool = pool_delete (matrixpool);
  levelpool = pool_delete (levelpool);
  clistpool = pool_delete (clistpool);
  drawpool = pool_delete (drawpool);
  edgepool = pool_delete (edgepool);
  nodepool = pool_delete (nodepool);
  graphpool = pool_delete (graphpool);
  topgraphpool = pool_delete (topgraphpool);
  uniqstrings = pool_delete (uniqstrings);
  world = pool_delete (world);

  return (0);
}

/* free allocated memory */
void
mmfree (void *p)
{
  if (p)
    {
      engine_free (p);
      mm_free_s = mm_free_s + 1;
      /* */
      p = (void *) 0;
    }
  return;
}

/* malloc wrapper assuming zeroed memory is returned like calloc() */
void *
mmalloc (size_t n)
{
  void *p = (void *) 0;
  void *x = (void *) 0;
  if (n)
    {
      p = engine_malloc (n);
      if (p)
	{
	  x = (void *) memset (p, 0, n);
	  mm_malloc_s = mm_malloc_s + 1;
	  mm_size_t = mm_size_t + n;
	}
    }
  return ((void *) p);
}

/* check */
int
mm_check (FILE * stream)
{

  fprintf (stream,
	   "%d malloc's and %d free's %d bytes %d megabyte %d gigabyte\n",
	   mm_malloc_s, mm_free_s, mm_size_t, (mm_size_t + 512) / 1024,
	   (mm_size_t + 512 * 1024) / (1024 * 1024));
  fflush (stream);

  return (0);
}

/* End. */
