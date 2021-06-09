/*
Checksum: 2533956677      (mvar.c)
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

/* programname from argv[0] */
char *progname = "";

/* */
FILE *logstream = (FILE *) 0;

/* */
int loglevel = 0;

/* matrix uniq number */
int matrixrefnum = 0;

/* node uniq number */
int noderefnum = 0;

/* edge uniq number */
int edgerefnum = 0;

/* selfedge uniq number */
int selfedgerefnum = 0;

/* graph uniq number */
int graphrefnum = 0;

/* End. */
