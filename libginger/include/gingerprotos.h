/*
Checksum: 1317940467      (gingerprotos.h)
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

/* memory pool subsystem */
struct region;
typedef struct region *pool;

/* */
#undef MIN
#define MIN(x,y)    ( ((x)<(y)) ? (x):(y) )

/* */
#undef MAX
#define MAX(x,y)    ( ((x)>(y)) ? (x):(y) )

/* gcc oke status */
#ifdef SUCCESS_EXIT_CODE
#undef SUCCESS_EXIT_CODE
#endif
#define SUCCESS_EXIT_CODE 0

/* gcc err status */
#ifdef FAILURE_EXIT_CODE
#undef FAILURE_EXIT_CODE
#endif
#define FAILURE_EXIT_CODE 33

#include "structs.h"
#include "clist.h"
#include "edge.h"
#include "eparse.h"
#include "vparse.h"
#include "graph.h"
#include "layout1.h"
#include "layout2.h"
#include "lginger.h"
#include "machine.h"
#include "mempool.h"
#include "matrix.h"
#include "mm.h"
#include "mvarr.h"
#include "node.h"
#include "uniqstr.h"
#include "ps.h"
#include "log.h"
#include "gd12.h"
#include "svg.h"
#include "dotl.h"

/* End. */
