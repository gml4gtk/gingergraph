/*
Checksum: 3915728921      (lginger.c)
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
#include <stdarg.h>
#include <time.h>

#include "gingerprotos.h"

/**
 * int ginger_init (void *(*mallocroutine) (size_t n), void (*freeroutine) (void *p))
 * @param mallocroutine - optional malloc reroute
 * @param freeroutine - optional free reroute
 * @return status - init status
 *
 * If arguments are zero the c-lib malloc/free is used,
 * otherwise the supplied routines will be used to wrap
 * all malloc and free routine calls.
 *
 * The wrapped malloc() is supposed to allocate zero'ed memory like calloc().
 *
 * Return status is set on some error, otherwise zero and that is oke.
 *
 * GC_MALLOC(n) macro to use to wrap malloc()'s via libgc.
 **/
int
ginger_init (void *(*mallocroutine) (size_t n), void (*freeroutine) (void *p),
	     int argc, char **argv)
{
  /* */
  if (mallocroutine)
    {
      engine_malloc = mallocroutine;
    }
  else
    {
      engine_malloc = malloc;
    }
  /* */
  if (freeroutine)
    {
      engine_free = freeroutine;
    }
  else
    {
      engine_free = free;
    }
  /* */
  if (machine_init (argc, argv))
    {
      return (1);
    }
  /* */
  if (pool_init ())
    {
      return (2);
    }
  /* */
  if (mm_init ())
    {
      return (3);
    }
  /* */
  if (uniqstr_init ())
    {
      return (4);
    }
  /* */
  if (ginger_randinit ())
    {
      return (5);
    }
  /* Oke. */
  return (0);
}

/**
 * int ginger_deinit (void)
 *
 * De-init lib, return status 0 if oke,
 * and all memory has been de-allocated.
 * Return status other then zero indicates
 * something went wrong and memory leaks
 * may have happened.
 **/
int
ginger_deinit (void)
{
  /* */
  if (uniqstr_deinit ())
    {
      return (1);
    }
  if (mm_deinit ())
    {
      return (2);
    }
  if (pool_deinit ())
    {
      return (3);
    }
  if (machine_deinit ())
    {
      return (4);
    }
  /* Oke. */
  return (0);
}

/* */
int
ginger_check (FILE * stream)
{
  /* */
  return (0);
}

/* if rootedgraph is 0 then it's the rootgraph */
graph *
ginger_graph (graph * rootedgraph)
{
  return (creategraph (rootedgraph));
}

/* */
node *
ginger_node (graph * g, char *name)
{
  node *n1 = (node *) 0;
  node *n2 = (node *) 0;
  n1 = createnode (g, name);
  n2 = addnode (g, n1);
  return ((node *) n2);
}

/* */
edge *
ginger_edge (graph * g, char *src, char *dst)
{
  node *n1 = (node *) 0;
  node *n2 = (node *) 0;
  edge *e1 = (edge *) 0;
  edge *e2 = (edge *) 0;
  n1 = findnode (g, src);
  n2 = findnode (g, dst);
  if (n1 && n2)
    {
      /* if (n1 == n2) it is a selfedge from node x->x */
      e1 = createedge (g, n1, n2);
      e2 = addedge (g, e1);
    }
  return ((edge *) e2);
}

/* */
int
ginger_machine (FILE * stream)
{

  fprintf (stream, " /* matrix uniq number %d */ \n", matrixrefnum);
  fprintf (stream, " /* node uniq number %d */ \n", noderefnum);
  fprintf (stream, " /* edge uniq number %d */ \n", edgerefnum);
  fprintf (stream, " /* selfedge uniq number %d */ \n", selfedgerefnum);
  fprintf (stream, " /* graph uniq number %d */ \n", graphrefnum);
  fprintf (stream, " /* sizeof(graph) %d */ \n", sizeofgraph ());
  fprintf (stream, " /* sizeof(edge) %d */ \n", sizeofedge ());
  fprintf (stream, " /* sizeof(node) %d */ \n", sizeofnode ());
  fprintf (stream, " /* sizeof(matrix) %d */ \n", matrixsizeof ());
  fflush (stream);

  return (0);
}

/* Make a logfile */
int
ginger_log (FILE * stream, int level)
{
  logstream = (FILE *) 0;
  loglevel = 0;
  if (stream)
    {
      logstream = stream;
      loglevel = level;
      fprintf (stream, "/* loglevel %d */\n", level);
      fflush (stream);
    }
  return (0);
}

/* */
int
ginger_randinit (void)
{
  srand48 ((unsigned int) time (NULL));
  return (0);
}

/* */
int
ginger_rand (void)
{
  int ret = 0;
  ret = (int) lrand48 ();
  return (ret);
}

/* End. */
