/*
Checksum: 1254611892      (graph.c)
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
 *  struct circular_list *nodes;
 *  struct circular_list *edges;
 * } graph;
 */

/* */
graph *
creategraph (graph * rootgraph)
{
  graph *g = (graph *) 0;
  log_s (__FUNCTION__);
  g = (graph *) pool_malloc (graphpool, sizeof (graph));
  graphrefnum = graphrefnum + 1;
  g->refnum = graphrefnum;
  /* g->nodes = newlist (STRUCT, sizeof (node)); */
  g->nodes = createnodelist (g);
  /* g->edges = newlist (STRUCT, sizeof (edge)); */
  g->edges = createedgelist (g);
  /* number of selected nodes */
  g->nodecount = 0;
  log_m ("created graph[%d]", g->refnum);
  log_e (__FUNCTION__);
  return ((graph *) g);
}

/* */
int
sizeofgraph (void)
{
  return (sizeof (graph));
}

/* End. */
