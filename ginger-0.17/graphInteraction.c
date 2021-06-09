/* $Id: graphInteraction.c,v 1.4 1996/06/14 12:19:19 lupus Exp $ */

/***************************************************************
 * This file is part of the Ginger project.                    *
 * Created 11-02-1996                                          *
 * Niels C. Larsen, Aalborg University, Denmark                *
 * (lupus@iesd.auc.dk)                                         *
 * Martin K. Molz, Aalborg University, Denmark                 *
 * (molz@iesd.auc.dk)                                          *
 ***************************************************************/

#include "graphInteraction.h"
#include "edgeSelection.h"

static int first;

/* ---  private function prototypes  --- */

static void connectNodes(graph *g, node *n1, node *n2);

/* ---  public functions  --- */

/* create edges between all selected nodes. Do not create
   an extra edge between nodes which are connected already.
   Do not create an edge from a node to itself. 
   Select the created edge in the current mode.
   Return the number of new edges.  */
int connectSelectedNodes(graph *g) {
  int i,j,num;
  node *n1, *n2;
  
  first=1;
  num=0;

  /* perform explicit iterations over the node list */
  for(i=1; i<=getsize(g->nodes); i++) {
    n1=(node *)retrieve(i,g->nodes);
    if( n1->selected ) {
      for(j=1; j<=getsize(g->nodes); j++) {
	n2=(node *)retrieve(j,g->nodes);
	if( n2->selected && n1!=n2 && !areNeighbors(g, n1, n2) ) {
	  num++;
	  connectNodes(g, n1, n2);
	}
      }
    }
  }
  return num;
}

/* ---  private functions  --- */

static void connectNodes(graph *g, node *n1, node *n2) {
  edge *new_edge, *e;
  char name[256];
  
  if( n2->selected > n1->selected )
    new_edge = createEdge(createNewEdgeName(g,name), n1, n2);
  else
    new_edge = createEdge(createNewEdgeName(g,name), n2, n1);
  
  e=addEdge(g, new_edge);  /* creates and stores a shallow copy of new_edge */
  freeEdge(new_edge);
  if( first ) {
    first=0;
    edgeSelectInCurrentMode(g, e);
  }
  else
    edgeSelect(g,e,ACCUMULATING_MODE);
}
