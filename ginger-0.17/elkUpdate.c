/* $Id: elkUpdate.c,v 1.2 1996/03/28 13:55:49 lupus Exp $ */
  
/***************************************************************
 * This file is part of the Ginger project.                    *
 * Created 27-03-1996                                          *
 * Niels C. Larsen, Aalborg University, Denmark                *
 * (lupus@iesd.auc.dk)                                         *
 * Martin K. Molz, Aalborg University, Denmark                 *
 * (molz@iesd.auc.dk)                                          *
 ***************************************************************/

/* Interface for reflecting interactive actions to "the Scheme side" */

#include <stdio.h>
#include <string.h>

#include "clist.h"
#include "config.h"
#include "graph.h"
#include "elkInterface.h"
#include "elkUpdate.h"
#include "nodeSelection.h"

/* ---  private declarations  --- */

static void updateNodeIfSelected(BYTE *b);
static void unselectNode(BYTE *b);
static void unselectEdge(BYTE *b);

/* ---  public functions  --- */

void elkNodeClick(node *n) {
#if USE_SCHEME  
  char buf[256];
  
  if(n->object) {
    sprintf(buf, "(gingerNodeClick %d)", n->object);
    
    elkInterfaceEvalString(buf);
  }
#endif /* USE_SCHEME */
}

void elkNodeMove(node *n, int newx, int newy) {
#if USE_SCHEME  
  char buf[256];
  
  if(n->object) {
    sprintf(buf, "(gingerNodeMove %d %d %d)", n->object, newx, newy);
    
    elkInterfaceEvalString(buf);
  }
#endif /* USE_SCHEME */
}

void elkNodeSelect(node *n, int selected) {
#if USE_SCHEME  
  char buf[256];
  
  if(n->object) {
    sprintf(buf, "(gingerNodeSelected %d %d)", n->object, selected);
    
    elkInterfaceEvalString(buf);
  }
#endif /* USE_SCHEME */
}

void elkEdgeSelect(edge *e, int selected) {
#if USE_SCHEME  
  char buf[256];
  
  if(e->object) {
    sprintf(buf, "(gingerEdgeSelected %d %d)", e->object, selected);
    
    elkInterfaceEvalString(buf);
  }
#endif /* USE_SCHEME */
}

void elkNodePrepareSelect(graph *g, int selection_state) {
#if USE_SCHEME 
  if(selection_state!=ACCUMULATING_MODE)
    elkUnselectAllNodes(g);
#endif /* USE_SCHEME */
}

void elkEdgePrepareSelect(graph *g, int selection_state) {
#if USE_SCHEME 
  if(selection_state!=ACCUMULATING_MODE)
    elkUnselectAllEdges(g);
#endif /* USE_SCHEME */
}

void elkUnselectAllNodes(graph *g) {
#if USE_SCHEME 
  iterate(unselectNode, g->nodes);
#endif /* USE_SCHEME */
}

void elkUnselectAllEdges(graph *g) {
#if USE_SCHEME 
  iterate(unselectEdge, g->edges);
#endif /* USE_SCHEME */
}

void elkUnselectAllObjects(graph *g) {
#if USE_SCHEME 
  elkUnselectAllNodes(g);
  elkUnselectAllEdges(g);
#endif /* USE_SCHEME */
}

void elkEdgeClick(edge *e) {
#if USE_SCHEME  
  char buf[256];
  
  if(e->object) {
    sprintf(buf, "(gingerEdgeClick %d)", e->object);
    
    elkInterfaceEvalString(buf);
  }
#endif /* USE_SCHEME */
}

void elkSelectedNodesMoved(graph *g) {
#if USE_SCHEME 
  iterate(updateNodeIfSelected, g->nodes);
#endif /* USE_SCHEME */
}

/* ---  private functions  --- */

#if USE_SCHEME
static void updateNodeIfSelected(BYTE *b) {
  node *n;
  char buf[256];
  
  n=(node *)b;
  
  if(n->selected && n->object) {
    sprintf(buf, "(gingerNodeMove %d %d %d)", n->object, n->x, n->y);
    elkInterfaceEvalString(buf);
  }
}

static void unselectNode(BYTE *b) {
  node *n;
  char buf[256];
  
  n=(node *)b;
  
  if(n->object) {
    sprintf(buf, "(gingerNodeSelected %d 0)", n->object);
    elkInterfaceEvalString(buf);
  }
}

static void unselectEdge(BYTE *b) {
  edge *e;
  char buf[256];
  
  e=(edge *)b;
  
  if(e->object) {
    sprintf(buf, "(gingerEdgeSelected %d 0)", e->object);
    elkInterfaceEvalString(buf);
  }
}

#endif /* USE_SCHEME */
