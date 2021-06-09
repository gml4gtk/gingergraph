/*
Checksum: 3210548744      (main.c)
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
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#include "gingerprotos.h"

/* */
static int main2 (int argc, char *argv[], int round);

/* */
static graph *g = (graph *) 0;

/* nodes */
static int maxn = 0;

/* */
int
main (int argc, char *argv[])
{
  int status = 0;
  int round = 0;
  int i = 0;
  ginger_randinit ();
  for (i = 0; i < 1024 * 1024 * 1024; i++)
    {
      round++;
      status = ginger_init (0, 0, argc, argv);
      status = main2 (argc, argv, round);
      status = ginger_deinit ();
      sleep (1);
    }
  return (0);
}

/* */
static int
main2 (int argc, char *argv[], int round)
{
  int i = 0;
  int from = 0;
  int to = 0;
  int crossiters = 0;
  int placeiters = 0;
  char str1[256];
  char fnam[256];
  node *n1 = (node *) 0;
  node *n2 = (node *) 0;
  edge *e1 = (edge *) 0;
  edge *e2 = (edge *) 0;
  memset (str1, 0, 256);
  memset (fnam, 0, 256);
  maxn = ginger_rand () % 128;
  /* */
  if (maxn < 5)
    {
      return (0);
    }
  g = ginger_graph (0);
  for (i = 0; i < maxn; i++)
    {
      sprintf (str1, "%d", i);
      n1 = createnode (g, str1);
      n2 = addnode (g, n1);
    }
  /* for (i = 0; i < maxn/2; i++) */
  /* for (i = 0; i < maxn; i++) */
  for (i = 0; i < maxn * 2; i++)
    {
      from = ginger_rand () % maxn;
      sprintf (str1, "%d", from);
      n1 = findnode (g, str1);
      to = ginger_rand () % maxn;
      sprintf (str1, "%d", to);
      n2 = findnode (g, str1);
      e1 = createedge (0, n1, n2);
      e2 = addedge (g, e1);
    }
  crossiters = ginger_rand () % 64 + 1;
  placeiters = ginger_rand () % 64 + 3;
  tlayout (g, 10, 20, 50, 50, 0, crossiters, placeiters);
  sprintf (fnam, "test%d.gif", round);
  gd12graph (g, fnam);
  return (0);
}

/* End. */
