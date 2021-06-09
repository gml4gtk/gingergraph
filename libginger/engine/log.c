/*
Checksum: 2088729254      (log.c)
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

/* */
int
log_s (char *func)
{
  if (logstream)
    {
      fprintf (logstream, "%s(){\n", func);
      fflush (logstream);
    }
  return (0);
}

/* */
int
log_e (char *func)
{
  if (logstream)
    {
      fprintf (logstream, "} /* %s() */\n", func);
      fflush (logstream);
    }
  return (0);
}

/* */
int
log_m (const char *format, ...)
{
  char buffer[1024];
  int n = 0;
  va_list args;
  if (logstream)
    {
      memset (buffer, 0, 1024);
      va_start (args, format);
      n = vsnprintf (buffer, 1024, format, args);
      if (n)
	{
	  fprintf (logstream, " /* %s */\n", buffer);
	}
      va_end (args);
      fflush (logstream);
    }
  return (0);
}

/* */
void
log_matrixarray (matrix * *mm, int n)
{
  int i = 0;
  matrix *m = (matrix *) 0;
  if (logstream)
    {
      fprintf (logstream, " /* matrix array[] with %d matrix pointers */ ",
	       n);
      for (i = 0; i < n; i++)
	{
	  m = mm[i];
	  log_matrix (m);
	}
      fflush (logstream);
    }
  return;
}

/* localhelper */
static int
log_matrixat (matrix * m, int r, int c)
{
  int i = 0;
  int ret = 0;
  i =
    (m->logical_to_physical_row_number[r].redirection * m->columns) +
    (m->logical_to_physical_col_number[c].redirection);
  ret = m->elements[i];
  return ret;
}

/* */
void
log_matrix (matrix * m)
{
  int i = 0;
  int j = 0;
  if (logstream)
    {
      fprintf (logstream, " /* \n * ");
      for (i = 0; i < m->rows; i++)
	{
	  for (j = 0; j < m->columns; j++)
	    {
	      fprintf (logstream, "%3d ", log_matrixat (m, i, j));
	    }
	  fprintf (logstream, " | %3.1f at level %d  \n * ",
		   m->logical_to_physical_row_number[i].barycenter, (i + 1));
	}
      for (j = 0; j < m->columns; j++)
	{
	  fprintf (logstream, "----");
	}
      fprintf (logstream, "-+\n * ");
      for (j = 0; j < m->columns; j++)
	{
	  fprintf (logstream, "%3.1f ",
		   m->logical_to_physical_col_number[j].barycenter);
	}
      fprintf (logstream, " [barycenter_at_col]\n");
      fprintf (logstream, " */ \n");
      fflush (logstream);
    }
  return;
}

/* */
void
log_nodelist (graph * g)
{
  int k = 0;
  node *n = (node *) 0;
  if (logstream)
    {
      fprintf (logstream,
	       " /* nodelist in graph[%d] with %d nodes and %d edges */\n",
	       g->refnum, getsize (g->nodes), getsize (g->edges));
      for (k = 1; k <= getsize (g->nodes); k++)
	{
	  n = (node *) retrieve (k, g->nodes);
	  fprintf (logstream,
		   " /* node[%d] `%s' level %d position %d (x,y) is (%d,%d) */\n",
		   n->refnum, n->name, n->tree_layout.level,
		   n->tree_layout.position, n->x, n->y);
	}
      fflush (logstream);
    }
  return;
}

/* */
void
log_edgelist (graph * g)
{
  int k = 0;
  edge *e = (edge *) 0;
  if (logstream)
    {
      fprintf (logstream,
	       " /* edgelist in graph[%d] with %d nodes and %d edges */\n",
	       g->refnum, getsize (g->nodes), getsize (g->edges));
      for (k = 1; k <= getsize (g->edges); k++)
	{
	  e = (edge *) retrieve (k, g->edges);
	  fprintf (logstream,
		   " /* edge[%d] from node[%d] `%s' to node[%d] `%s' */\n",
		   e->refnum, e->from->refnum, e->from->name, e->to->refnum,
		   e->to->name);
	}
      fflush (logstream);
    }
  return;
}

/* End. */
