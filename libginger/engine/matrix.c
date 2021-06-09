/*
Checksum: 3095261694      (matrix.c)
*/

/*
 *    Tys Lefering
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

/* XXX if trymovenodeinrow delta is zero, skip */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "gingerprotos.h"

/* */
int current_edge_crossings = 0;

/* */
matrix *
matrixcreate (graph * g, int r, int c)
{
  matrix *m = (matrix *) 0;
  int i = 0;
  /* */
  m = mmalloc (sizeof (matrix));
  m->rows = r;
  m->columns = c;
  m->logical_to_physical_row_number =
    mmalloc ((r * sizeof (redirection_element)));
  m->logical_to_physical_col_number =
    mmalloc ((c * sizeof (redirection_element)));
  m->elements = mmalloc ((r * c * sizeof (int)));
  /* */
  for (i = 0; i < r; i++)
    {
      m->logical_to_physical_row_number[i].redirection = i;
      m->logical_to_physical_row_number[i].n = 0;
      m->logical_to_physical_row_number[i].barycenter = 0.0;
    }
  /* */
  for (i = 0; i < c; i++)
    {
      m->logical_to_physical_col_number[i].redirection = i;
      m->logical_to_physical_col_number[i].n = 0;
      m->logical_to_physical_col_number[i].barycenter = 0.0;
    }
  /* */
  for (i = 0; i < (r * c); i++)
    {
      m->elements[i] = 0;
    }
  matrixrefnum = matrixrefnum + 1;
  m->refnum = matrixrefnum;
  return ((matrix *) m);
}

/* */
int
matrixsizeof (void)
{
  int n = 0;
  n = sizeof (matrix);
  n = n + sizeof (redirection_element);
  n = n + sizeof (redirection_element);
  /* n->elements depends on matrixsize */
  return (n);
}

/* */
void
matrixdestroy (graph * g, matrix * m)
{
  mmfree (m->logical_to_physical_row_number);
  mmfree (m->logical_to_physical_col_number);
  mmfree (m->elements);
  mmfree (m);
  return;
}

/* */
matrix *
matrixcopy (graph * g, matrix * m)
{
  matrix *res = (matrix *) 0;
  int i = 0;
  int r = 0;
  int c = 0;
  r = m->rows;
  c = m->columns;
  res = matrixcreate (0, r, c);
  for (i = 0; i < r; i++)
    {
      res->logical_to_physical_row_number[i] =
	m->logical_to_physical_row_number[i];
    }
  for (i = 0; i < c; i++)
    {
      res->logical_to_physical_col_number[i] =
	m->logical_to_physical_col_number[i];
    }
  for (i = 0; i < (r * c); i++)
    {
      res->elements[i] = m->elements[i];
    }
  return res;
}

/* */
int
matrixat (graph * g, matrix * m, int r, int c)
{
  int i = 0;
  int ret = 0;
  i =
    (m->logical_to_physical_row_number[r].redirection * m->columns) +
    m->logical_to_physical_col_number[c].redirection;
  ret = m->elements[i];
  return (ret);
}

/* */
void
matrixatput (graph * g, matrix * m, int r, int c, int element)
{
  int i = 0;
  i =
    (m->logical_to_physical_row_number[r].redirection * m->columns) +
    (m->logical_to_physical_col_number[c].redirection);
  m->elements[i] = element;
  return;
}

/* */
void
matrixatrowputnode (graph * g, matrix * m, int r, node * n1)
{
  m->logical_to_physical_row_number[r].n = n1;
  return;
}

/* */
void
matrixatcolumnputnode (graph * g, matrix * m, int c, node * n1)
{
  m->logical_to_physical_col_number[c].n = n1;
  return;
}

/* */
node *
matrixnodeatrow (graph * g, matrix * m, int r)
{
  node *n = (node *) 0;
  n = ((node *) m->logical_to_physical_row_number[r].n);
  return ((node *) n);
}

/* */
node *
matrixnodeatcolumn (graph * g, matrix * m, int c)
{
  node *n = (node *) 0;
  n = ((node *) m->logical_to_physical_col_number[c].n);
  return ((node *) n);
}

/* */
void
matrixprioritylayoutdown (graph * g, matrix * m)
{
  int i = 0;
  int desired_position = 0;
  node *n = (node *) 0;
  /* */
  for (i = 0; i < m->columns; i++)
    {
      n = matrixnodeatcolumn (0, m, i);
      if (n)
	{
	  n->tree_layout.priority = n->tree_layout.upper_connectivity;
	}
    }

  /* */
  for (i = 0; i < m->columns; i++)
    {
      n = matrixnodeatcolumn (0, m, i);
      if (n)
	{
	  desired_position = matrixnodeupperbarycenter (0, m, n, i);
	  /* XXX if delta position is 0 skip, its oke. 
	     if (desired_position - n->tree_layout.position) */
	  trymovenodeincolumn (0, m, i, n->tree_layout.position,
			       (desired_position - n->tree_layout.position),
			       n->tree_layout.priority);
	}
    }
  return;
}

/* */
void
matrixprioritylayoutup (graph * g, matrix * m)
{
  int i = 0;
  int desired_position = 0;
  node *n = (node *) 0;

  for (i = 0; i < m->rows; i++)
    {
      n = matrixnodeatrow (0, m, i);
      if (n)
	{
	  n->tree_layout.priority = n->tree_layout.lower_connectivity;
	}
    }

  for (i = 0; i < m->rows; i++)
    {
      n = matrixnodeatrow (0, m, i);
      if (n)
	{
	  desired_position = matrixnodelowerbarycenter (0, m, n, i);
	  trymovenodeinrow (0, m, i, n->tree_layout.position,
			    (desired_position - n->tree_layout.position),
			    n->tree_layout.priority);
	}
    }
  return;
}

/* */
void
matrixarraydownupiterate (graph * g, matrix * m[], int size,
			  int max_iterations)
{
  int i = 0;
  current_edge_crossings = matrixarraycrossings (0, m, size);
  for (i = 0; i < max_iterations; i++)
    {
      if (current_edge_crossings == 0)
	{
	  return;
	}
      matrixarraydownup (0, m, size);
    }
  return;
}

/* */
int
matrixarraycrossings (graph * g, matrix * array[], int size)
{
  int i = 0;
  int sum = 0;
  sum = 0;
  for (i = 0; i < size; i++)
    {
      sum = sum + matrixcrossings (0, array[i]);
    }
  return sum;
}

/* */
int
matrixcrossings (graph * g, matrix * m)
{
  int j = 0;
  int k = 0;
  int p = 0;
  int sum = 0;
  sum = 0;
  p = m->rows;
  for (j = 0; j < (p - 1); j++)
    {
      for (k = (j + 1); k < p; k++)
	{
	  sum = sum + matrixcrossingsinrows (0, m, j, k);
	}
    }
  return sum;
}

/* */
int
matrixcrossingsinrows (graph * g, matrix * m, int j, int k)
{
  int a = 0;
  int b = 0;
  int q = 0;
  int sum = 0;
  sum = 0;
  q = m->columns;
  for (a = 0; a < (q - 1); a++)
    {
      for (b = (a + 1); b < q; b++)
	{
	  sum = sum + (matrixat (0, m, j, b) * matrixat (0, m, k, a));
	}
    }
  return sum;
}

/* */
int
matrixnodeupperbarycenter (graph * g, matrix * m, node * n, int c)
{
  int ret = 0;
  int i = 0;
  int sum = 0;
  node *n1 = (node *) 0;

  if (n->tree_layout.upper_connectivity == 0)
    {
      return ((int) -1);
    }
  sum = 0;
  for (i = 0; i < m->rows; i++)
    {
      n1 = matrixnodeatrow (0, m, i);
      if (n1)
	{
	  sum = sum + (n1->tree_layout.position * matrixat (0, m, i, c));
	}
    }
  ret = (sum / n->tree_layout.upper_connectivity);
  return (ret);
}

/* */
int
matrixnodelowerbarycenter (graph * g, matrix * m, node * n, int r)
{
  int i = 0;
  int sum = 0;
  int ret = 0;
  node *n1 = (node *) 0;
  if (n->tree_layout.lower_connectivity == 0)
    {
      return ((int) -1);
    }
  sum = 0;
  for (i = 0; i < m->columns; i++)
    {
      n1 = matrixnodeatcolumn (0, m, i);
      if (n1)
	{
	  sum = sum + (n1->tree_layout.position * matrixat (0, m, r, i));
	}
    }
  ret = (sum / n->tree_layout.lower_connectivity);
  return (ret);
}

/* */
int
trymovenodeincolumn (graph * g, matrix * m, int column, int old_position,
		     int delta_position, int priority)
{
  int i = 0;
  int dist = 0;
  node *n = (node *) 0;
  /* */
  if (delta_position == 0)
    {
      return 0;
    }
  /* to left */
  if (delta_position < 0)
    {
      if (column == 0)
	{
	  movenodeincolumn (0, m, column, delta_position);
	  return delta_position;
	}
      i = (column - 1);
      n = matrixnodeatcolumn (0, m, i);
      if (n)
	{
	  if (n->tree_layout.position < (old_position + delta_position))
	    {
	      movenodeincolumn (0, m, column, delta_position);
	      return delta_position;
	    }
	  else if (n->tree_layout.priority < priority)
	    {
	      dist = n->tree_layout.position - old_position + 1;
	      /* if zero dont
	         if (delta_position - dist) */
	      dist =
		dist + trymovenodeincolumn (0, m, i, n->tree_layout.position,
					    (delta_position - dist),
					    priority);
	      if (dist != 0)
		{
		  movenodeincolumn (0, m, column, dist);
		}
	      return dist;
	    }
	  else
	    {
	      dist = n->tree_layout.position - old_position + 1;
	      if (dist != 0)
		{
		  movenodeincolumn (0, m, column, dist);
		}
	      return dist;
	    }
	}
      else
	{
	  return 0;
	}
    }
  /* to right */
  if (delta_position > 0)
    {
      if (column == (m->columns - 1))
	{
	  movenodeincolumn (0, m, column, delta_position);
	  return delta_position;
	}
      i = (column + 1);
      n = matrixnodeatcolumn (0, m, i);
      if (n)
	{
	  if (n->tree_layout.position > (old_position + delta_position))
	    {
	      movenodeincolumn (0, m, column, delta_position);
	      return delta_position;
	    }
	  else
	    {
	      dist = n->tree_layout.position - old_position - 1;
	      /* if zero its oke
	         if (delta_position - dist) */
	      dist =
		dist + trymovenodeincolumn (0, m, i, n->tree_layout.position,
					    (delta_position - dist),
					    priority);
	      if (dist != 0)
		{
		  movenodeincolumn (0, m, column, dist);
		}
	      return dist;
	    }
	}
      else
	{
	  return 0;
	}
    }
  return 0;
}

/* */
int
trymovenodeinrow (graph * g, matrix * m, int row, int old_position,
		  int delta_position, int priority)
{
  int i = 0;
  int dist = 0;
  node *n = (node *) 0;
  if (delta_position == 0)
    {
      return 0;
    }
  /* to left */
  if (delta_position < 0)
    {
      if (row == 0)
	{
	  movenodeinrow (0, m, row, delta_position);
	  return delta_position;
	}
      i = (row - 1);
      n = matrixnodeatrow (0, m, i);
      if (n)
	{
	  if (n->tree_layout.position < (old_position + delta_position))
	    {
	      movenodeinrow (0, m, row, delta_position);
	      return delta_position;
	    }
	  else if (n->tree_layout.priority < priority)
	    {
	      dist = n->tree_layout.position - old_position + 1;
	      dist =
		dist + trymovenodeinrow (0, m, i, n->tree_layout.position,
					 (delta_position - dist), priority);
	      if (dist != 0)
		{
		  movenodeinrow (0, m, row, dist);
		}
	      return dist;
	    }
	  else
	    {
	      dist = n->tree_layout.position - old_position + 1;
	      if (dist != 0)
		{
		  movenodeinrow (0, m, row, dist);
		}
	      return dist;
	    }
	}
      else
	{
	  return 0;
	}
    }
  /* to right */
  if (delta_position > 0)
    {
      if (row == (m->rows - 1))
	{
	  movenodeinrow (0, m, row, delta_position);
	  return delta_position;
	}
      i = (row + 1);
      n = matrixnodeatrow (0, m, i);
      if (n)
	{
	  if (n->tree_layout.position > (old_position + delta_position))
	    {
	      movenodeinrow (0, m, row, delta_position);
	      return delta_position;
	    }
	  else
	    {
	      dist = n->tree_layout.position - old_position - 1;
	      dist =
		dist + trymovenodeinrow (0, m, i, n->tree_layout.position,
					 (delta_position - dist), priority);
	      if (dist != 0)
		{
		  movenodeinrow (0, m, row, dist);
		}
	      return dist;
	    }
	}
      else
	{
	  return 0;
	}
    }
  return 0;
}

/* */
void
movenodeincolumn (graph * g, matrix * m, int c, int delta_position)
{
  node *n = (node *) 0;
  n = matrixnodeatcolumn (0, m, c);
  if (n)
    {
      n->tree_layout.position = n->tree_layout.position + delta_position;
    }
  return;
}

/* */
void
movenodeinrow (graph * g, matrix * m, int r, int delta_position)
{
  node *n = (node *) 0;
  n = matrixnodeatrow (0, m, r);
  if (n)
    {
      n->tree_layout.position = n->tree_layout.position + delta_position;
    }
  return;
}

/* */
void
matrixarraydownup (graph * g, matrix * m[], int size)
{
  matrixarraydown (0, m, size);
  if (current_edge_crossings == 0)
    {
      return;
    }
  matrixarrayup (0, m, size);
  return;
}

/* */
void
matrixarraydown (graph * g, matrix * m[], int size)
{
  int i = 0;
  int c1 = 0;
  int c2 = 0;
  matrix *m1[MAX_LEVELS];
//  matrix **m1down;
  if (size >= MAX_LEVELS)
    {
      return;
    }
  /* */
  /* */
  for (i = 0; i < size; i++)
    {
      m1[i] = matrixcopy (0, m[i]);
    }
  /* */
  c1 = current_edge_crossings;
  /* */
  for (i = 0; i < (size - 1); i++)
    {
      matrixcomputecolumnbarycenters (0, m[i]);
      matrixsortcolumnsonbarycenterfollow (0, m[i], m[i + 1]);
    }
  /* */
  matrixcomputecolumnbarycenters (0, m[i]);
  matrixsortcolumnsonbarycenter (0, m[i]);
  /* */
  c2 = matrixarraycrossings (0, m, size);

  /* */
  if (c2 <= c1)
    {
      current_edge_crossings = c2;
      for (i = 0; i < size; i++)
	{
	  matrixdestroy (0, m1[i]);
	}
    }
  else
    {
      for (i = 0; i < size; i++)
	{
	  matrixdestroy (0, m[i]);
	  m[i] = m1[i];
	}
    }
  return;
}

/* todo */
static matrix **Xm1up;

void
matrixarrayup_alloc (int nlevel)
{
  Xm1up =
    (matrix **) mmalloc ((nlevel + 1) * (sizeof (matrix *)));
  return;
}

/* */
void
matrixarrayup (graph * g, matrix * m[], int size)
{
  int i = 0;
  int c1 = 0;
  int c2 = 0;
  /* matrix *m1[MAX_LEVELS]; */
  /* matrix **m1up; */
  matrix *m1up[MAX_LEVELS];
  /* */
  if (size >= MAX_LEVELS)
    {
      return;
    }

  for (i = 0; i < size; i++)
    {
      m1up[i] = matrixcopy (0, m[i]);
    }

  c1 = current_edge_crossings;

  for (i = (size - 1); i > 0; i--)
    {
      matrixcomputerowbarycenters (0, m[i]);
      matrixsortrowsonbarycenterfollow (0, m[i], m[i - 1]);
    }

  matrixcomputerowbarycenters (0, m[0]);
  matrixsortrowsonbarycenter (0, m[0]);

  c2 = matrixarraycrossings (0, m, size);

  if (c2 <= c1)
    {
      current_edge_crossings = c2;
      for (i = 0; i < size; i++)
	{
	  matrixdestroy (0, m1up[i]);
	}
    }
  else
    {
      for (i = 0; i < size; i++)
	{
	  matrixdestroy (0, m[i]);
	  m[i] = m1up[i];
	}
    }
  return;
}

/* */
void
matrixcomputecolumnbarycenters (graph * g, matrix * m)
{
  int c = 0;
  for (c = 0; c < m->columns; c++)
    {
      matrixatcolumnputbarycenter (0, m, c, matrixcolumnbarycenter (0, m, c));
    }
  return;
}

/* */
void
matrixcomputerowbarycenters (graph * g, matrix * m)
{
  int r = 0;
  for (r = 0; r < m->rows; r++)
    {
      matrixatrowputbarycenter (0, m, r, matrixrowbarycenter (0, m, r));
    }
  return;
}

/* */
float
matrixrowbarycenter (graph * g, matrix * m, int r)
{
  float ret = 0.0;
  int l = 0;
  int q = 0;
  int sum1 = 0;
  int sum2 = 0;
  q = m->columns;
  sum1 = 0;
  sum2 = 0;
  for (l = 0; l < q; l++)
    {
      sum1 = sum1 + ((l + 1) * matrixat (0, m, r, l));
      sum2 = sum2 + (matrixat (0, m, r, l));
    }
  if (sum1 == 0)
    {
      ret = ((float) 0.0);
    }
  else if (sum2 == 0)
    {
      ret = ((float) 0.0);
    }
  else
    {
      ret = ((float) ((float) sum1 / (float) sum2));
    }
  return ((float) ret);
}

/* */
void
matrixatrowputbarycenter (graph * g, matrix * m, int r, float b)
{
  int i = 0;
  i = m->logical_to_physical_row_number[r].redirection;
  m->logical_to_physical_row_number[i].barycenter = b;
  return;
}

/* */
void
matrixatcolumnputbarycenter (graph * g, matrix * m, int c, float b)
{
  int i = 0;
  i = m->logical_to_physical_col_number[c].redirection;
  m->logical_to_physical_col_number[i].barycenter = b;
  return;
}

/* */
float
matrixcolumnbarycenter (graph * g, matrix * m, int c)
{
  float ret = 0.0;
  int k = 0;
  int p = 0;
  int sum1 = 0;
  int sum2 = 0;
  p = m->rows;
  sum1 = 0;
  sum2 = 0;
  for (k = 0; k < p; k++)
    {
      sum1 = sum1 + ((k + 1) * matrixat (0, m, k, c));
      sum2 = sum2 + matrixat (0, m, k, c);
    }
  if (sum1 == 0)
    {
      ret = ((float) 0.0);
    }
  else if (sum2 == 0)
    {
      ret = ((float) 0.0);
    }
  else
    {
      ret = ((float) ((float) sum1 / (float) sum2));
    }
  return ((float) ((float) sum1 / (float) sum2));
}

/* */
void
swapelements (graph * g, redirection_element array[], int i, int j)
{
  redirection_element tmp;
  tmp = array[i];
  array[i] = array[j];
  array[j] = tmp;
  return;
}

/* */
void
sortredirectionvectoronbarycenter (graph * g, redirection_element array[],
				   int array_size)
{
  int i = 0;
  int j = 0;
  /* */
  for (i = 1; i < array_size; i++)
    {
      for (j = i; j > 0 && array[j - 1].barycenter > array[j].barycenter; j--)
	{
	  swapelements (0, array, (j - 1), j);
	}
    }
  return;
}

/* */
void
sortredirectionvectoronbarycenterfollow (graph * g,
					 redirection_element array[],
					 redirection_element follow_array[],
					 int array_size)
{
  int i = 0;
  int j = 0;
 /* */
  for (i = 1; i < array_size; i++)
    {
      for (j = i; j > 0 && array[j - 1].barycenter > array[j].barycenter; j--)
	{
	  swapelements (0, array, (j - 1), j);
	  swapelements (0, follow_array, (j - 1), j);
	}
    }
  return;
}

/* */
void
matrixsortrowsonbarycenter (graph * g, matrix * m)
{
  sortredirectionvectoronbarycenter (0, m->logical_to_physical_row_number,
				     m->rows);
  return;
}

/* */
void
matrixsortcolumnsonbarycenter (graph * g, matrix * m)
{
  sortredirectionvectoronbarycenter (0, m->logical_to_physical_col_number,
				     m->columns);
  return;
}

/* */
void
matrixsortrowsonbarycenterfollow (graph * g, matrix * m, matrix * follow)
{
  if (m->rows != follow->columns)
    {
      return;
    }
  sortredirectionvectoronbarycenterfollow (0,
					   m->logical_to_physical_row_number,
					   follow->
					   logical_to_physical_col_number,
					   m->rows);
  return;
}

/* */
void
matrixsortcolumnsonbarycenterfollow (graph * g, matrix * m, matrix * follow)
{
  if (m->columns != follow->rows)
    {
      return;
    }
  sortredirectionvectoronbarycenterfollow (0,
					   m->logical_to_physical_col_number,
					   follow->
					   logical_to_physical_row_number,
					   m->columns);
  return;
}

/* */
void
matrixprintlog (FILE * logstream, matrix * m)
{
  int i = 0;
  int j = 0;
  if (!logstream)
    {
      return;
    }
  for (i = 0; i < m->rows; i++)
    {
      for (j = 0; j < m->columns; j++)
	{
	  fprintf (logstream, "%3d ", matrixat (0, m, i, j));
	}
      fprintf (logstream, " | %3.1f at level %d\n",
	       m->logical_to_physical_row_number[i].barycenter, (i + 1));
    }
  for (j = 0; j < m->columns; j++)
    {
      fprintf (logstream, "----");
    }
  fprintf (logstream, "-+\n");
  for (j = 0; j < m->columns; j++)
    {
      fprintf (logstream, "%3.1f ",
	       m->logical_to_physical_col_number[j].barycenter);
    }
  fprintf (logstream, " [barycenter_at_col]\n");
  return;
}

/* End. */
