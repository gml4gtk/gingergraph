/*
Checksum: 2833490571      (structs.h)
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

#define MAX_LEVELS 1000

#define CL_STRING 0

#define STRUCT 1

/* */
typedef void (*funct) (void *);
typedef int (*cmpfunct) (unsigned char *, unsigned char *);

/* */
struct list_element 
{
  unsigned char *value;
  struct list_element *next;
};

/* */
struct circular_list 
{
  struct list_element *last_element;
  struct list_element *retrieve;
  int retrievepos;
  int size;
  int cursor;
  int value_size;
  unsigned char value_type;
};

/* */
typedef struct circular_list clist;

/* nodelist and clist same for now */
typedef clist nodelist;

/* edgelist and clist same for now */
typedef clist edgelist;

/* */
typedef struct 
{
 int refnum;
 char *name;
 int selected;
 struct {
 int level;
 int position;
 int upper_connectivity;
 int lower_connectivity;
 int priority;
 } tree_layout;
 int x;
 int y;
 int width;
 int height;
 /* struct { othernodedata } */
} node;

/* */
typedef struct
{
 int x;
 int y;
} axpoint;

/* */
typedef struct 
{
 int refnum;
 node *to;
 node *from;
 char *name;
 int direction;
 axpoint p1;
 axpoint p2;
 int selected;
 int highlighted;
} edge;

/* */
typedef struct 
{
 int refnum;
 /* struct circular_list *nodes; */
 nodelist *nodes;
 /* struct circular_list *edges; */
 edgelist *edges;
 /* */
 int nodecount;
} graph;

/* */
typedef struct 
{
 int redirection;
 float barycenter;
 node *n;
} redirection_element;

/* */
typedef struct 
{
 int refnum;
 int rows;
 int columns;
 redirection_element *logical_to_physical_row_number;
 redirection_element *logical_to_physical_col_number;
 int *elements;
} matrix;

/* */
typedef void (*node_fun) (graph *, node *neighbor, node *from);
typedef void (*edge_fun) (graph *, edge *); 

/* End. */
