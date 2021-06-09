/* $Id: edge.h,v 1.9 1996/03/14 16:02:18 lupus Exp $ */

/***************************************************************
 * This file is part of the Ginger project.                    *
 * Created 28-12-1995                                          *
 * Niels C. Larsen, Aalborg University, Denmark                *
 * (lupus@iesd.auc.dk)                                         *
 ***************************************************************/

#ifndef EDGE_H
#define EDGE_H

#include <stdio.h>
#include "gingerNew.h"

#include "mathMacros.h"
#include "color.h"
#include "node.h"

#define DIR_NONE    0
#define DIR_NORMAL  1
#define DIR_INVERSE 2
#define DIR_BIDIRECTIONAL 3

#define ARROW_LENGTH 10
#define ARROW_WIDTH 5

typedef struct {
  node *to;
  node *from;
  char *name;
  int  direction;  /* DIR_NONE, DIR_NORMAL, DIR_INVERSE, or
		      DIR_BIDIRECTIONAL */
  XPoint p1;
  XPoint p2;
  color_info foreground; /* color */

  int selected;          /* > 0 if the edge is selected */

  int highlighted;       /* private bookkeeping: is the edge 
			    currently highlighted? */
#if USE_SCHEME
  unsigned long object;/* identify the corresponding Scheme object. 
			  There will always exist a corresponding 
			  reference to the object on the Scheme side, 
			  therefore this reference does not need to be
			  protected from garbage collection.
		       */
#endif
} edge;

edge *createEdge(char *name, node *from, node *to);
void destroyEdge(edge *e);
void freeEdge(edge *e);
void calculateEdgeEndpoints(edge *e);
void calculateEdgeEndpointsElliptical(edge *e);
void drawEdge(edge *e);
void undrawEdge(edge *e);
void printEdge(edge *e, FILE *fd);
void edgeSetColor(edge *e, char *colorname);
void edgeHighlight(edge *e, int highlight_on);
int  edgeDistanceFromPoint(edge *e, int x, int y);

#endif
