/* $Id: graphDisplay.h,v 1.10 1996/06/14 12:18:58 lupus Exp $ */

/***************************************************************
 * This file is part of the Ginger project.                    *
 * Created 29-12-1995                                          *
 * Niels C. Larsen, Aalborg University, Denmark                *
 * (lupus@iesd.auc.dk)                                         *
 ***************************************************************/

#ifndef GRAPH_DISPLAY_H
#define GRAPH_DISPLAY_H

#include "graph.h"

/* size added to the actual size of the graph when determining
   the desired size of the graph drawing area */
#define GRAPH_MARGIN 15

/* perform a (re)display of the entire graph */
void displayGraph(graph *g);

/* perform a (re)display of a part of the graph */
void displayGraphWithinRectangle(graph *g, XPoint upper_left, XPoint lower_right);

/* move the node n to new posision (newx, newy) 
   Update all adjacent edges to n  */
void moveNode(graph *g, node *n, int newx, int newy);

/* move the selected nodes */
void moveSelectedNodes(graph *g, node *anchorNode, int newx, int newy);

/* return a pointer to the node that covers the point (x,y).
   return 0 if the point isn't covered by any node */
node *nodeAtPoint(graph *g, int x, int y);

/* return a pointer to the edge that is "near" the point (x,y).
   return 0 if no edge is near to the point.
   "near" is defined as less than the distance 
   EDGE_SELECTION_DISTANCE */
edge *edgeAtPoint(graph *g, int x, int y);

/* find the lower left corner of the graph */
void graphMaxCoordinates(graph *g, int *return_max_x, int *return_max_y );

/* delete an edge. Update the graph display, including "expose" */
void graphDisplayDeleteEdge(graph *g, edge *e);

/* delete the selected edges. Update the graph display, incl. "expose" */
int graphDisplayDeleteSelectedEdges(graph *g);

/* delete the selected nodes and their adjacent edges.
   Update the graph display, incl. "expose" */
int graphDisplayDeleteSelectedNodes(graph *g);

/* delete the node and its adjacent edges. 
   Update the graph display, incl. "expose" */
void graphDisplayDeleteNode(graph *g, node *n);

/* update the position of endpoints of all edges adjacent to the 
   selected nodes */
void recalculateEdgeEndpointsOfSelectedNodes(graph *g);

#endif
