/*
Checksum: 3315905027      (eparse.c)
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
#include <errno.h>

#include "gingerprotos.h"

/*
 * easy file format first nodes then edges. example:
 * 1
 * 2
 * 3
 * 1,2
 * 1,3
 * 
 * this defines node 1, 2, 3 
 * edge from node 1 to node 2
 * edge from node 1 to node 3
 */

/* */
static int nlines = 0;

/* lexparse given file */
int
eparse (char *filename, graph * g)
{
  char buffer[256];
  char str1[16];
  char str2[16];
  int num1 = 0;
  int num2 = 0;
  int status = 0;
  FILE *stream = (FILE *) 0;
  node *n1 = (node *) 0;
  edge *e1 = (edge *) 0;
  log_s (__FUNCTION__);
  nlines = 1;
  errno = 0;
  stream = fopen (filename, "r");
  log_m ("filename `%s' has stream %p", filename, stream);
  if (!stream)
    {
      log_m ("could not open file and errno %d `%s'\n", errno,
	     strerror (errno));
      log_e (__FUNCTION__);
      return (1);
    }
  /* */
  memset (buffer, 0, 256);
  memset (str1, 0, 16);
  memset (str2, 0, 16);
  /* */
  num1 = 0;
  num2 = 0;
  /* */
  while (fgets (buffer, 256, stream))
    {
      /* raw full line */
      log_m ("input line %d `%s'", nlines, buffer);
      nlines = nlines + 1;
      /* */
      status = sscanf (buffer, "%d,%d", &num1, &num2);
      log_m ("status %d from %d and %d", status, num1, num2);
      /* */
      if (status == 1)
	{
	  /* node */
	  sprintf (str1, "%d", num1);
	  n1 = ginger_node (g, str1);
	  log_m ("added node `%s' `%s'", n1->name, str1);
	}
      else if (status == 2)
	{
	  /* edge from->to */
	  sprintf (str1, "%d", num1);
	  sprintf (str2, "%d", num2);
	  log_m ("edge from `%s' to `%s'", str1, str2);
	  e1 = ginger_edge (g, str1, str2);
	}
      else
	{
	  /* skip */
	}
      /* */
      memset (buffer, 0, 256);
      memset (str1, 0, 16);
      memset (str2, 0, 16);
      /* */
      num1 = 0;
      num2 = 0;
    }

  fclose (stream);

  /* what has been parsed. */
  log_nodelist (g);
  log_edgelist (g);

  log_e (__FUNCTION__);

  return (0);
}

/* End. */
