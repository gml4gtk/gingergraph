/* $Id: graph.h,v 1.10 1996/03/14 16:02:18 lupus Exp $ */

/***************************************************************
 * This file is part of the Ginger project.                    *
 * Created 28-12-1995                                          *
 * Niels C. Larsen, Aalborg University, Denmark                *
 * (lupus@iesd.auc.dk)                                         *
 ***************************************************************/

#ifndef GRAPH_H
#define GRAPH_H

#include "node.h"
#include "edge.h"

#include "clist.h"

typedef struct {
  CLIST *nodes;
  CLIST *edges;
} graph;

typedef void (*node_fun)(graph *, node *neighbor, node *from);
typedef void (*edge_fun)(graph *, edge *); 

graph *createGraph();
void destroyGraph(graph *g);
node *addNode(graph *g, node *n);
void deleteNode(graph *g, node *n);
edge *addEdge(graph *g, edge *e);
void deleteEdge(graph *g, edge *e);

int  areNeighbors(graph *g, node *n1, node *n2);
char *createNewEdgeName(graph *g, char *return_buffer);
char *createNewNodeName(graph *g, char *return_buffer);
node *graphFindNodeNamed(graph *g, char *name);
edge *graphFindEdgeNamed(graph *g, char *name);

void onNeighborsDo(graph *g, node *n, node_fun onUpperNeighbor,
		   node_fun onLowerNeighbor);

#if USE_SCHEME
/* find objects correspoing to the given Scheme objects */
node *graphFindNodeObject(graph *g, unsigned long obj);
edge *graphFindEdgeObject(graph *g, unsigned long obj);
#endif

#endif
