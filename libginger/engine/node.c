/*
Checksum: 2565977210      (node.c)
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
static int the_count = 0;

/*
 * typedef struct 
 * {
 *  int refnum;
 *  char *name;
 *  int selected;
 *  struct {
 *  int level;
 *  int position;
 *  int upper_connectivity;
 *  int lower_connectivity;
 *  int priority;
 *  } tree_layout;
 *  int x;
 *  int y;
 *  int width;
 *  int height;
 * } node;
 * 
 */
node *
createnode (graph * g, char *name)
{
  node *v = (node *) 0;
  log_s (__FUNCTION__);
  /* v = (node *) mmalloc (sizeof (node)); */
  v = (node *) pool_malloc (nodepool, sizeof (node));
  noderefnum = noderefnum + 1;
  v->refnum = noderefnum;
  v->name = uniqstr (name);
  v->selected = 1;		/* on */
  v->tree_layout.level = 0;
  v->tree_layout.position = 0;
  v->tree_layout.upper_connectivity = 0;
  v->tree_layout.lower_connectivity = 0;
  v->tree_layout.priority = 0;
  v->x = 0;
  v->y = 0;
  v->width = 0;
  v->height = 0;
  log_m ("node[%d] `%s'", v->refnum, name);
  log_e (__FUNCTION__);
  return ((node *) v);
}

/* */
node *
addnode (graph * g, node * n)
{
  node *m = (node *) 0;
  log_s (__FUNCTION__);
  insert_last ((unsigned char *) n, g->nodes);
  m = (node *) get_last (g->nodes);
  log_m ("added node[%d] `%s' to graph[%d]", n->refnum, n->name, g->refnum);
  log_e (__FUNCTION__);
  return ((node *) m);
}

/* */
node *
findnode (graph * g, char *name)
{
  node *n = (node *) 0;
  node *ret = (node *) 0;
  int k = 0;
  for (k = 1; k <= getsize (g->nodes); k++)
    {
      n = (node *) retrieve (k, g->nodes);
      if (!strcmp (name, n->name))
	{
	  ret = n;
	  break;
	}
    }
  return ((node *) ret);
}

/* */
void
cnodeselectioncount (void *b)
{
  node *n = (node *) 0;
  n = (node *) b;
  if (!n)
    {
      return;
    }
  if (n->selected)
    {
      the_count = the_count + 1;
    }
  return;
}

/* */
int
nodeselectcount (graph * g)
{
  the_count = 0;
  if (!g)
    {
      return 0;
    }
  iterate (cnodeselectioncount, g->nodes);
  return the_count;
}

/* */
node *
startnode (graph * g)
{
  node *n = (node *) 0;
  n = (node *) retrieve (1, g->nodes);
  return ((node *) n);
}








/* */
nodelist *
createnodelist (graph * g)
{
  nodelist *vv = (nodelist *) 0;
  vv = (nodelist *) newlist (STRUCT, sizeof (node));
  return ((nodelist *) vv);
}

/* */
node *
startnodelist (graph * g)
{
  node *n = (node *) 0;
  n = (node *) retrieve (1, g->nodes);
  return ((node *) n);
}

/* */
node *
nextnodelist (graph * g, nodelist * n)
{
  return (0);
}

/* */
int
sizeofnode (void)
{
  return (sizeof (node));
}

/* End. */
