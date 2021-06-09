/*
Checksum: 4002253921      (svg.c)
*/

/*
 * Copyright Martin K. Molz and Niels C. Larsen, Denmark.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "gingerprotos.h"

/*
 * REC-SVG11-20030114.pdf www.w3.org 
 *
 * too very minimalistic. svg generated 
 * can be imported into the program dia
 * http://www.lysator.liu.se/~alla/dia/
 */

/* */
static int minx = 0;
static int miny = 0;
static int maxx = 0;
static int maxy = 0;

void
svggraph (graph * g, char *filename)
{
  FILE *stream = (FILE *) 0;
  int k = 0;
  node *n = (node *) 0;
  edge *e = (edge *) 0;
  log_s (__FUNCTION__);

  stream = fopen (filename, "w");

  if (!stream)
    {
      return;
    }

  minx = 1024 * 1024;
  miny = 1024 * 1024;
  maxx = 0;
  maxy = 0;

  for (k = 1; k <= getsize (g->nodes); k++)
    {
      n = (node *) retrieve (k, g->nodes);
      if (n->x > maxx)
	{
	  maxx = n->x;
	}
      if (n->x < minx)
	{
	  minx = n->x;
	}
      if (n->y > maxy)
	{
	  maxy = n->y;
	}
      if (n->y < miny)
	{
	  miny = n->y;
	}
    }

  log_m ("min (x,y) is (%d,%d) max (x,y) is (%d,%d)", minx, miny, maxx, maxy);

  fprintf (stream,
	   "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\"?>\n");
  fprintf (stream,
	   "<svg width=\"3cm\" height=\"6cm\" viewBox=\"0 0 3 6\">\n");

  /* nodes */
  for (k = 1; k <= getsize (g->nodes); k++)
    {
      n = (node *) retrieve (k, g->nodes);
      log_m ("node[%d] `%s' at (%d,%d)", n->refnum, n->name, n->x, n->y);
      fprintf (stream,
	       "  <rect style=\"fill: #ffffff\" x=\"%d\" y=\"%d\" width=\"1.0\" height=\"1.0\"/>\n",
	       n->x, n->y);
    }

  /* edges */
  for (k = 1; k <= getsize (g->edges); k++)
    {
      e = (edge *) retrieve (k, g->edges);
      log_m ("edge[%d]", e->refnum);
      fprintf (stream,
	       "  <line style=\"fill: none; fill-opacity:0; stroke-width: 0.1; stroke: #000000\" x1=\"%d\" y1=\"%d\" x2=\"%d\" y2=\"%d\"/>\n",
	       e->from->x, e->from->y, e->to->x, e->to->y);

    }

  fprintf (stream, "</svg>\n");

  fclose (stream);

  log_e (__FUNCTION__);

  return;
}

/* End. */
