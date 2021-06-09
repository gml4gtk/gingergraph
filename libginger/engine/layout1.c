/*
Checksum:  967154332      (layout1.c)
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
#include <string.h>

#include "gingerprotos.h"

/* at determine relative x positions
 * if node levels distance at edge is
 * >1 then addin extra nodes
 */

#define FIRST_LEVEL 0

int min_level = 0;		/* lowest relative x level */
int max_level = 0;		/* highest relative x level */
int delta_level = 0;		/* delta of relative level */

/* int number_of_nodes_at_level[MAX_LEVELS]; */
int *number_of_nodes_at_level = (int *) 0;

matrix **matrices = (matrix **) 0;
int x_distance = 0;
int y_distance = 0;
int x_corner = 0;
int y_corner = 0;
int vertical_tree = 0;

/* */
void
tlayout (graph * g, int cornerx, int cornery, int distx, int disty,
	 int vertical, int crossingiterations, int placementiterations)
{
  node *n = (node *) 0;
  edge *e = (edge *) 0;
  int i = 0;
  int j = 0;
  int min_position = 0;
  int level = 0;

  log_s (__FUNCTION__);

  log_m ("corner (x,y) is (%d,%d)", cornerx, cornery);
  log_m ("dist (x,y) is (%d,%d)", distx, disty);
  log_m ("vertical is %d", vertical);
  log_m ("crossingsiterations is %d", crossingiterations);
  log_m ("placementirerations is %d", placementiterations);

  /* */
  g->nodecount = nodeselectcount (g);

  /* handle self-edges a->a into a->x->x->x->a */
  /* handle doubled edges, single nodes, loops */

  log_m ("%d nodes %d edges %d selected nodes", getsize (g->nodes),
	 getsize (g->edges), g->nodecount);

  if (g->nodecount < 2)
    {
      printf ("%s(): nodecount is %d\n", __FUNCTION__, g->nodecount);
      log_e (__FUNCTION__);
      return;
    }

  min_level = 0;
  max_level = 0;

  x_distance = distx;
  y_distance = disty;

  x_corner = cornerx;
  y_corner = cornery;

  vertical_tree = vertical;

  /* Reset layouterdata. */
  for (i = 1; i <= getsize (g->nodes); i++)
    {
      n = (node *) retrieve (i, g->nodes);
      n->tree_layout.level = 0;
      n->tree_layout.position = 0;
      n->tree_layout.priority = 0;
      n->tree_layout.upper_connectivity = 0;
      n->tree_layout.lower_connectivity = 0;
    }












  /* Determine relative horizontal node positions. */
  /* privatecomputenodelevel (g); */
 /* mpi */
  for (i = 1; i <= getsize (g->nodes); i++)
    {
      n = (node *) retrieve (i, g->nodes);
      if (n->selected)
	{
	  log_m ("node[%d] `%s'", n->refnum, n->name);
	  /* onnbsdo (g, n, privateonuppernb, privateonlowernb); */
	  /* */
	  for (j = 1; j <= getsize (g->edges); j++)
	    {
	      e = (edge *) retrieve (j, g->edges);
	      /* */
	      log_m ("edge[%d] from node[%d] `%s' to edge[%d] `%s'",
		     e->refnum, e->from->refnum, e->from->name, e->to->refnum,
		     e->to->name);
	      /* */
	      if (e->to == n)
		{
		  /* nb = e->from; */
		  /* if (onuppernb != 0 && nb != 0) */
		  /* if (nb != 0) */
		  if (e->from)
		    {
		      /* (*onuppernb) (g, nb, n); */
		      /* privateonuppernb (g, nb, n); */
		      if (e->from->selected)
			{
			  privateonuppernb (g, e->from, n);
			}
		    }
		}
	      /* */
	      if (e->from == n)
		{
		  /* nb = e->to; */
		  /* if (onlowernb != 0 && nb != 0) */
		  /* if (nb != 0) */
		  if (e->to)
		    {
		      /* (*onlowernb) (g, nb, n); */
		      /* privateonlowernb (g, nb, n); */
		      if (e->to->selected)
			{
			  privateonlowernb (g, e->to, n);
			}
		    }
		}
	    }
	}
    }

  /* */
  delta_level = (max_level - min_level);

  log_m ("max_level %d min_level %d delta is %d", max_level, min_level,
	 delta_level);

  /* all nodes on same level */
  if (delta_level == 0)
    {
      log_m ("todo: delta_level %d", delta_level);
    }

  /* level tables */
  number_of_nodes_at_level =
    (int *) pool_malloc (levelpool, ((delta_level + 1) * sizeof (int)));




































  /*
   *
   */
  /* tables, iterate (mapadjustlevel, g->nodes); */
  for (i = 1; i <= getsize (g->nodes); i++)
    {
      n = (node *) retrieve (i, g->nodes);
      if (n->selected)
	{
	  log_m
	    ("node[%d] `%s' n->tree_layout.level is %d (FIRST_LEVEL %d - min_level %d)",
	     n->refnum, n->name, n->tree_layout.level, FIRST_LEVEL,
	     min_level);
	  n->tree_layout.level =
	    n->tree_layout.level + (FIRST_LEVEL - min_level);
	  level = n->tree_layout.level;
	  log_m ("to n->tree_layout.level %d", n->tree_layout.level);
	  if (level < MAX_LEVELS)
	    {
	      log_m ("changing max_level %d versus %d", max_level, level);
	      max_level = MAX (max_level, level);
	      n->tree_layout.position = number_of_nodes_at_level[level]++;
	    }
	}
    }
  /*
   *
   */





































  if (max_level == 0)
    {
      log_m ("todo: max_level %d", max_level);
      log_e (__FUNCTION__);
      return;
    }

  min_level = FIRST_LEVEL;

  log_m ("relative horizontal max_level is %d and min_level is %d",
	 max_level, min_level);


  /* */
  if ((crossingiterations == 0) && (placementiterations == 0))
    {
      /* */
    }


  /* */
  matrices =
    (matrix **) pool_malloc (matrixpool,
			     ((max_level + 1) * (sizeof (matrix *))));

  /* Relative horizontal positions of the nodes are known. */

  privatebuildandinitializematrices (g);

  if (crossingiterations > 0)
    {
      matrixarraydownupiterate (0, matrices, (max_level - min_level),
				crossingiterations);
    }

  privateupdatenodepositions ();

  for (j = 0; j < placementiterations; j++)
    {
      /* */
      log_m ("========================");
      log_m ("iteration %d of %d XXX", j, placementiterations);
      log_m ("========================");
      /* mpi */
      for (i = min_level; i < max_level; i++)
	{
	  matrixprioritylayoutdown (0, matrices[i]);
	}
      j++;
      if (j < placementiterations)
	{
	  for (i = max_level - 1; i >= min_level; i--)
	    {
	      matrixprioritylayoutup (0, matrices[i]);
	    }
	}
    }

  min_position = privatelowestnodepositions ();

  /* calculated positions */
  log_nodelist (g);
  log_edgelist (g);

  /* update node positions */
  tnewnodecoordinates (vertical, min_position);

  recalculateedgeendpointsofselectednodes (g);

  for (i = min_level; i < max_level; i++)
    {
      matrixdestroy (0, matrices[i]);
    }

  log_m ("%d malloc's %d free's %d bytes", mm_free_s, mm_malloc_s, mm_size_t);
  log_e (__FUNCTION__);

  return;
}

/************************/

/* 
 * void
 * privatecomputenodelevel (graph * g)
 * {
 *   node *n = (node *) 0;
 *   int k = 0;
 *   log_s (__FUNCTION__);
 *   for (k = 1; k <= getsize (g->nodes); k++)
 *     {
 *       n = (node *) retrieve (k, g->nodes);
 *       if (n->selected)
 * 	{
 * 	  log_m ("node[%d] `%s'", n->refnum, n->name);
 * 	  onnbsdo (g, n, privateonuppernb,
 * 			 privateonlowernb);
 * 	}
 *     }
 *   log_e (__FUNCTION__);
 *   return;
 * }
 */

/* */
void
privateonuppernb (graph * g, node * n1, node * n2)
{
  log_s (__FUNCTION__);
  if (!n1->selected)
    {
      log_e (__FUNCTION__);
      return;
    }
  if (n1->tree_layout.level)
    {
      log_m ("node[%d] n1 `%s' has level %d, skip", n1->refnum, n1->name,
	     n1->tree_layout.level);
      log_e (__FUNCTION__);
      return;
    }
  log_m ("node[%d] n1 `%s' at %d node n2 `%s' at %d", n1->refnum, n1->name,
	 n1->tree_layout.level, n2->name, n2->tree_layout.level);
  log_m ("from n1->tree_layout.level %d", n1->tree_layout.level);
  n1->tree_layout.level = n2->tree_layout.level - 1;
  log_m ("to n1->tree_layout.level %d", n1->tree_layout.level);
  min_level = MIN (min_level, n1->tree_layout.level);
  onnbsdo (g, n1, privateonuppernb, privateonlowernb);
  log_e (__FUNCTION__);
  return;
}

/* */
void
privateonlowernb (graph * g, node * n1, node * n2)
{
  log_s (__FUNCTION__);
  if (!n1->selected)
    {
      log_e (__FUNCTION__);
      return;
    }
  if (n1->tree_layout.level)
    {
      log_m ("node[%d] n1 `%s' has level %d, skip", n1->refnum, n1->name,
	     n1->tree_layout.level);
      log_e (__FUNCTION__);
      return;
    }
  log_m ("from n1->tree_layout.level %d", n1->tree_layout.level);
  n1->tree_layout.level = n2->tree_layout.level + 1;
  max_level = MAX (max_level, n1->tree_layout.level);
  onnbsdo (g, n1, privateonuppernb, privateonlowernb);
  log_e (__FUNCTION__);
  return;
}

/* */
void
onnbsdo (graph * g, node * n, node_fun onuppernb, node_fun onlowernb)
{
  int i = 0;
  /*  node *nb = (node *) 0; */
  edge *e = (edge *) 0;
  log_s (__FUNCTION__);
  log_m ("node[%d] `%s' level %d", n->refnum, n->name, n->tree_layout.level);
  for (i = 1; i <= getsize (g->edges); i++)
    {
      e = (edge *) retrieve (i, g->edges);
      log_m ("edge[%d] from node[%d] `%s' to edge[%d] `%s'", e->refnum,
	     e->from->refnum, e->from->name, e->to->refnum, e->to->name);
      if (e->to == n)
	{
	  /*  nb = e->from; */
	  /*  if (onuppernb != 0 && nb != 0) */
	  if (e->from)
	    {
	      if (e->from->selected)
		{
		  (*onuppernb) (g, e->from, n);
		}
	    }
	}
      if (e->from == n)
	{
	  /* nb = e->to; */
	  /* if (onlowernb != 0 && nb != 0) */
	  if (e->to)
	    {
	      if (e->to->selected)
		{
		  (*onlowernb) (g, e->to, n);
		}
	    }
	}
    }
  log_e (__FUNCTION__);
  return;
}

/************************/

/*
 * void
 * mapadjustlevel (void *n1)
 * {
 *   node *n = (node *) 0;
 *   int level = 0;
 *   log_s (__FUNCTION__);
 *   n = (node *) n1;
 *   if (n->selected)
 *     {
 *       log_m
 * 	("node[%d] `%s' n->tree_layout.level is %d (FIRST_LEVEL %d - min_level %d)",
 * 	 n->refnum, n->name, n->tree_layout.level, FIRST_LEVEL, min_level);
 *       n->tree_layout.level = n->tree_layout.level + (FIRST_LEVEL - min_level);
 *       level = n->tree_layout.level;
 *       log_m ("to n->tree_layout.level %d", n->tree_layout.level);
 *       if (level < MAX_LEVELS)
 * 	{
 * 	  log_m ("changing max_level %d versus %d", max_level, level);
 * 	  max_level = MAX (max_level, level);
 * 	  n->tree_layout.position = number_of_nodes_at_level[level]++;
 * 	}
 *     }
 *   log_e (__FUNCTION__);
 *   return;
 * }
 */

/************************/

/* */
void
privatebuildandinitializematrices (graph * g)
{
  int i = 0;
  int j = 0;
  int k = 0;
  node *n1 = (node *) 0;
  node *n2 = (node *) 0;
  edge *e = (edge *) 0;
  log_s (__FUNCTION__);
  for (i = min_level; i < max_level; i++)
    {
      j = 0;
      /* */
      log_m ("level %d matrix (%dx%d)", i, number_of_nodes_at_level[i],
	     number_of_nodes_at_level[i + 1]);
      /* matrices[i] = matrixcreate (g, number_of_nodes_at_level[i],number_of_nodes_at_level[i + 1]); */
      matrices[i] =
	matrixcreate (0, number_of_nodes_at_level[i],
		      number_of_nodes_at_level[i + 1]);
      /* */
      for (k = 1; k <= getsize (g->nodes); k++)
	{
	  n1 = (node *) retrieve (k, g->nodes);
	  if (n1->selected && n1->tree_layout.level == i)
	    {
	      matrixatrowputnode (0, matrices[i], n1->tree_layout.position,
				  n1);
	    }
	  if (n1->selected && n1->tree_layout.level == i + 1)
	    {
	      matrixatcolumnputnode (0, matrices[i], n1->tree_layout.position,
				     n1);
	    }
	}
      /* */
      for (k = 1; k <= getsize (g->edges); k++)
	{
	  e = (edge *) retrieve (k, g->edges);
	  n1 = e->from;
	  n2 = e->to;
	  if (n1->selected && n2->selected)
	    {
	      if ((n1->tree_layout.level == i
		   && n2->tree_layout.level == i + 1))
		{
		  matrixatput (0, matrices[i], n1->tree_layout.position,
			       n2->tree_layout.position, 1);
		  n1->tree_layout.lower_connectivity++;
		  n2->tree_layout.upper_connectivity++;
		}
	      if ((n2->tree_layout.level == i
		   && n1->tree_layout.level == i + 1))
		{
		  matrixatput (0, matrices[i], n2->tree_layout.position,
			       n1->tree_layout.position, 1);
		  n2->tree_layout.lower_connectivity++;
		  n1->tree_layout.upper_connectivity++;
		}
	    }
	}
    }
  log_e (__FUNCTION__);
  return;
}

/* */
void
privateupdatenodepositions (void)
{
  int i = 0;
  int j = 0;
  node *n1 = (node *) 0;
  log_s (__FUNCTION__);
  for (i = min_level; i < max_level; i++)
    {
      for (j = 0; j < number_of_nodes_at_level[i]; j++)
	{
	  log_m ("%d nodes at level %d", i, number_of_nodes_at_level[i]);
	  n1 = matrixnodeatrow (0, matrices[i], j);
	  if (n1)
	    {
	      n1->tree_layout.position = j;
	    }
	}
    }
  i = max_level;
  for (j = 0; j < number_of_nodes_at_level[i]; j++)
    {
      n1 = matrixnodeatcolumn (0, matrices[i - 1], j);
      if (n1)
	{
	  n1->tree_layout.position = j;
	}
    }
  log_e (__FUNCTION__);
  return;
}

/* */
int
privatelowestnodepositions (void)
{
  int i = 0;
  int j = 0;
  int res = 0;
  node *n1 = (node *) 0;
  log_s (__FUNCTION__);
  res = 1024 * 1024 * 1024;
  for (i = min_level; i < max_level; i++)
    {
      for (j = 0; j < number_of_nodes_at_level[i]; j++)
	{
	  n1 = matrixnodeatrow (0, matrices[i], j);
	  if (n1)
	    {
	      res = MIN (res, n1->tree_layout.position);
	    }
	}
    }
  i = max_level;
  for (j = 0; j < number_of_nodes_at_level[i]; j++)
    {
      n1 = matrixnodeatcolumn (0, matrices[i - 1], j);
      if (n1)
	{
	  res = MIN (res, n1->tree_layout.position);
	}
    }
  log_e (__FUNCTION__);
  return res;
}

/* */
void
tnewnodecoordinates (int vert, int min_pos)
{
  int i = 0;
  int j = 0;
  node *n1 = (node *) 0;
  log_s (__FUNCTION__);
  log_m ("vertical is %d min_pos is %d min_level is %d max_level is %d", vert,
	 min_pos, min_level, max_level);
  for (i = min_level; i < max_level; i++)
    {
      for (j = 0; j < number_of_nodes_at_level[i]; j++)
	{
	  n1 = matrixnodeatrow (0, matrices[i], j);
	  if (n1)
	    {
	      if (vert)
		{
		  n1->x =
		    x_corner +
		    ((n1->tree_layout.position - min_pos) * x_distance);
		  n1->y = y_corner + (y_distance * n1->tree_layout.level);
		}
	      else
		{
		  n1->x = x_corner + (x_distance * n1->tree_layout.level);
		  n1->y =
		    y_corner +
		    ((n1->tree_layout.position - min_pos) * y_distance);
		}
	    }
	}
    }
  i = max_level;
  for (j = 0; j < number_of_nodes_at_level[i]; j++)
    {
      n1 = matrixnodeatcolumn (0, matrices[i - 1], j);
      log_matrix (matrices[i - 1]);
      if (n1)
	{
	  if (vert)
	    {
	      n1->x =
		x_corner +
		((n1->tree_layout.position - min_pos) * x_distance);
	      n1->y = y_corner + (y_distance * n1->tree_layout.level);
	    }
	  else
	    {
	      n1->x = x_corner + (x_distance * n1->tree_layout.level);
	      n1->y =
		y_corner +
		((n1->tree_layout.position - min_pos) * y_distance);
	    }
	}
    }
  log_e (__FUNCTION__);
  return;
}

/* */
void
recalculateedgeendpointsofselectednodes (graph * g)
{
  log_s (__FUNCTION__);
  log_e (__FUNCTION__);
  return;
}

/* End. */
