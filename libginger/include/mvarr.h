/*
Checksum: 2037824648      (mvarr.h)
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

extern char *progname;

extern pool world;
extern pool uniqstrings;
extern pool topgraphpool;
extern pool graphpool;
extern pool nodepool;
extern pool edgepool;
extern pool drawpool;
extern pool clistpool;
extern pool levelpool;
extern pool matrixpool;

extern int mm_free_s;
extern int mm_malloc_s;
extern size_t mm_size_t;

extern void *(*engine_malloc) (size_t n);
extern void (*engine_free) (void *p);

extern FILE *logstream;
extern int loglevel;

extern int matrixrefnum;
extern int noderefnum;
extern int edgerefnum;
extern int selfedgerefnum;
extern int graphrefnum;

/* End. */
