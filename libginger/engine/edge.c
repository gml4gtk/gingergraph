/*
Checksum: 1866385828      (edge.c)
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

#include "gingerprotos.h"

/* 
 * typedef struct 
 * {
 *  int refnum;
 *  node *to;
 *  node *from;
 *  int direction;
 *  axpoint p1;
 *  axpoint p2;
 *  int selected;
 * } edge;
 */

/* */
edge *
createedge (graph * g, node * from, node * to)
{
  edge *e = (edge *) 0;
  log_s (__FUNCTION__);
  /* e = (edge *) mmalloc (sizeof (edge)); */
  e = (edge *) pool_malloc (edgepool, sizeof (edge));
  edgerefnum = edgerefnum + 1;
  e->refnum = edgerefnum;
  e->to = (node *) to;
  e->from = (node *) from;
  e->direction = 0;
  e->p1.x = 0;
  e->p1.y = 0;
  e->p2.x = 0;
  e->p2.y = 0;
  e->selected = 1;
  log_m ("created edge[%d] from node[%d] `%s' to node[%d] `%s'", e->refnum,
	 e->from->refnum, e->from->name, e->to->refnum, e->to->name);
  log_e (__FUNCTION__);
  return ((edge *) e);
}

/* */
edge *
addedge (graph * g, edge * e)
{
  edge *e1 = (edge *) 0;
  log_s (__FUNCTION__);
  /* split selfedge into multiple ones: a--x--x--x--b */
  if (e->from->refnum == e->to->refnum)
    {
      /* */
    }
  insert_last ((unsigned char *) e, g->edges);
  e1 = (edge *) get_last (g->edges);
  log_m ("added edge[%d]", e1->refnum);
  log_e (__FUNCTION__);
  return ((edge *) e1);
}

/* */
edgelist *
createedgelist (graph * g)
{
  edgelist *ee = (edgelist *) 0;
  ee = (edgelist *) newlist (STRUCT, sizeof (edge));
  return ((edgelist *) ee);
}

/* */
int
sizeofedge (void)
{
  return (sizeof (edge));
}

/* End. */
