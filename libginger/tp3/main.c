/*
Checksum: 3840499432      (main.c)
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
static int main2 (int argc, char *argv[]);

/* */
static int justzero = 0;
static int justone = 1;

/* */
int
main (int argc, char *argv[])
{
  int status = 0;
  if (justzero)
    {
      justone = 1;
      status = ginger_init (0, 0, argc, argv);
      status = machine_check (stdout);
      status = pool_check (stdout);
      status = mm_check (stdout);
      status = uniqstr_check (stdout);
      status = ginger_check (stdout);
      status = ginger_deinit ();
      return (0);
    }
  /* optional malloc()/free() wrapper */
  status = ginger_init (0, 0, argc, argv);
  status = main2 (argc, argv);
  status = ginger_machine (stdout);
  status = ginger_deinit ();
  return (0);
}

/* */
static int
main2 (int argc, char *argv[])
{
  int status = 0;
  FILE *stream = (FILE *) 0;
  FILE *logfile = (FILE *) 0;
  graph *g = (graph *) 0;
  if (justone)
    {
      /* Pacify. */
      justzero = 0;
      justone = 1;
      logfile = fopen ("./logfile.txt", "w");
      if (logfile)
	{
	  status = ginger_log (logfile, 1);
	}
      else
	{
	  status = ginger_log (0, 0);
	}
    }
  else
    {
      status = ginger_log (0, 0);
    }

  g = creategraph (0);

  status = eparse ("input.txt", g);

  log_m ("eparse status: %d (zero is oke)\n", status);

  if (status)
    {
      log_m ("some parser error\n");
      return (1);
    }

  /* graph, corner (x,y), dist (x,y), vertical, crossingiter, placementiter */
  tlayout (g, 10, 20, 50, 50, 0, 1, 3);
  /* tlayout (g, 1, 1, 25, 25, 0, 1, 3); */
  /* tlayout (g, 10, 20, 50, 50, 0, 1, 30); */

  stream = fopen ("./test.ps", "w");
  postscriptgraph (g, stream);
  fclose (stream);

  gd12graph (g, "test.gif");

  svggraph (g, "test.svg");

  if (logfile)
    {
      fclose (logfile);
    }

  return (0);
}

/* End. */
