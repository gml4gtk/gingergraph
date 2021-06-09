/* $Id: symtable.h,v 1.7 1996/02/26 08:51:43 lupus Exp $ */

/***************************************************************
 * This file is part of the Ginger project.                    *
 * Created 2-1-1996                                            *
 * Niels C. Larsen, Aalborg University, Denmark                *
 * (lupus@iesd.auc.dk)                                         *
 ***************************************************************/

#ifndef SYMTABLE_H
#define SYMTABLE_H

#include "clist.h"
#include "graph.h"

#define CLASH_MODE_MERGE  0
#define CLASH_MODE_WARN_AND_RENAME   1
#define CLASH_MODE_RENAME 2

#define T_NODE 0
#define T_EDGE 1
#define T_TEMPLATE 2

/* attribute object; used as a common representation of nodes,
   edges, and templates in the symbol table during parsing */
typedef struct {
  int type;          /* T_NODE, T_EDGE, or T_TEMPLATE */
  char *name;
  int ok;            /* set to 1 by the semantic check if 
			the object is well defined */
  
  node *n;           /* the "real" node object, 
			if the object represents a node */
  edge *e;           /* the "real" edge object, similar to node *n */

  char *label;       /* the following are the actual attributes. */
  int vlabel;        /* the v-prefixed members indicate whether the
			corresponding attribute is "valid", i.e. set
			to a meaningful value */
  int shape;
  int vshape;
  char *bitmap;
  int vbitmap;
  char *pixmap;
  int vpixmap;
  char *contents;
  int vcontents;
  char *editor;
  int veditor;
  char *color;
  int vcolor;
  char *bg_color;
  int vbg_color;
  char *to;
  int vto; 
  char *from;
  int vfrom;
  char *template;
  int vtemplate;
  int x;
  int vx;
  int y;
  int vy;
  int width;
  int vwidth;
  int height;
  int vheight;
  int direction;
  int vdirection;

} attribute_obj;

/* ---  public functions  --- */

void initTable();
void cleanUpTable();

void resetObj(attribute_obj *a);
void destroyObj(attribute_obj *a);

void addObj(attribute_obj *a);
void printObj(attribute_obj *a);
attribute_obj *findObjNamed(char *name, int obj_type);
void semanticCheck();
graph *buildGraphFromSymtable();
graph *mergeGraphFromSymtableWithGraph(graph *g, 
				       int node_clash_mode,
				       int edge_clash_mode);
#endif
