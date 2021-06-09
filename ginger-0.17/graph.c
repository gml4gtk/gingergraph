/* $Id: graph.c,v 1.13 1996/03/14 16:02:18 lupus Exp $ */

/***************************************************************
 * This file is part of the Ginger project.                    *
 * Created 28-12-1995                                          *
 * Niels C. Larsen, Aalborg University, Denmark                *
 * (lupus@iesd.auc.dk)                                         *
 ***************************************************************/

#include "graph.h"

/* ---  Functions to create graphs, and to add/delete nodes
        and egdes  --- */

/* ---  private variables  --- */  

/* currentId is used for constructing unique names for created
   nodes and edges */
static int currentId=0;
     
/* ---  private function prototypes  --- */

static void mapDestroyNode(BYTE *n);
static void mapDestroyEdge(BYTE *e);

/* ---  public functions  --- */

/* create and initialize a graph object */
graph *createGraph() {
  graph *result;
  
  result = GingerNew(graph);
  result->nodes = newlist(STRUCT, sizeof(node));
  result->edges = newlist(STRUCT, sizeof(edge));
  
  return result;
}

/* destroy the graph object. The function also destroys all
   node and edge objects in the graph and frees the graph object */
void destroyGraph(graph *g) {
  /* destroy the contents of node and edge objects */
  iterate(mapDestroyNode, g->nodes);
  iterate(mapDestroyEdge, g->edges);

  /* delete node and edge objects */
  delete_all(g->nodes);
  delete_all(g->edges);

  /* destroy list objects */
  free(g->nodes);
  free(g->edges);

  /* deallocate graph object */
  GingerFree((char *)g);
}

/* add an existing node object to graph.
   The node object is shallow-copied by the graph, so it's safe
   to free the node parameter after the call.
   A pointer to the new copy is returned.
*/
node *addNode(graph *g, node *n) {
  insert_last((BYTE *)n, g->nodes);
  return (node *)get_last(g->nodes);
}

/* delete a node object from the graph.
   The node object itself is destroyed by the operation.
   There should be NO edges to/from the node when this operation
   is called.
*/
void deleteNode(graph *g, node *n) {
  destroyNode(n);
  delete_item((BYTE *)n, g->nodes);
}

/* add an existing edge object to graph.
   The edge object is shallow-copied by the graph, so it's safe
   to free the edge parameter after the call.
   A pointer to the new copy is returned.
*/
edge *addEdge(graph *g, edge *e) {
  insert_last((BYTE *)e, g->edges);
  return (edge *)get_last(g->edges);
}

/* delete an edge object from the graph.
   The edge object itself is destroyed by the operation.
*/
void deleteEdge(graph *g, edge *e) {
  delete_item((BYTE *)e, g->edges);
}

/* are nodes n1 and n2 neighbors? */
int areNeighbors(graph *g, node *n1, node *n2) {
  int i;
  edge *e;
  
  /* perform an explicit iteration over the edge list */
  for(i=1; i<=getsize(g->edges); i++) {
    e=(edge *)retrieve(i,g->edges);
    if( (e->to==n1 && e->from==n2) || (e->to==n2 && e->from==n1) )
      return 1;
  }
  
  return 0;
}

char *createNewEdgeName(graph *g, char *return_buffer) {
  int i;
  i=0;
  do {
    sprintf(return_buffer, "_edge%d", currentId++);
    i++;
  } while( graphFindEdgeNamed(g,return_buffer) && i<10000 );
  return return_buffer;
}

char *createNewNodeName(graph *g, char *return_buffer) {
  int i;
  i=0;
  do {
    sprintf(return_buffer, "_node%d", currentId++);
    i++;
  } while( graphFindNodeNamed(g,return_buffer) && i<10000 );
  return return_buffer;
}

/* return 0 if the node doesn't exist in the graph */
node *graphFindNodeNamed(graph *g, char *name) { 
  int i;
  node *n;
  
  for (i=1; i<=getsize(g->nodes); i++) {
    n=(node *)retrieve(i,g->nodes);
    if( !strcmp(n->name,name) )
      return n;
  }
  return 0;
}

/* return 0 if the edge doesn't exist in the graph */
edge *graphFindEdgeNamed(graph *g, char *name) { 
  int i;
  edge *e;
  
  for (i=1; i<=getsize(g->edges); i++) {
    e=(edge *)retrieve(i,g->edges);
    if( !strcmp(e->name,name) )
      return e;
  }
  return 0;
}

void onNeighborsDo(graph *g, node *n, node_fun onUpperNeighbor,
		   node_fun onLowerNeighbor) {
  int i;
  node *neighbor;
  edge *e;

  /* perform an explicit iteration over the edge list */
  for(i=1; i<=getsize(g->edges); i++) {
    e=(edge *)retrieve(i,g->edges);
    if( e->to==n ) {
      neighbor=e->from;
      if( onUpperNeighbor!=0 && neighbor!=0 )
	(*onUpperNeighbor)(g,neighbor,n);
    }
    if( e->from==n ) {
      neighbor=e->to;
      if( onLowerNeighbor!=0 && neighbor!=0 )
	(*onLowerNeighbor)(g,neighbor,n);
    }
  }
}

node *graphFindNodeObject(graph *g, unsigned long obj) {
#if USE_SCHEME
  int i;
  node *n;
  
  for (i=1; i<=getsize(g->nodes); i++) {
    n=(node *)retrieve(i,g->nodes);
    if( n->object==obj )
      return n;
  }
#endif
  return 0;
}

edge *graphFindEdgeObject(graph *g, unsigned long obj) {
#if USE_SCHEME
  int i;
  edge *e;
  
  for (i=1; i<=getsize(g->edges); i++) {
    e=(edge *)retrieve(i,g->edges);
    if( e->object==obj )
      return e;
  }
#endif
  return 0;
}

/* ---  private functions  --- */

static void mapDestroyNode(BYTE *n) {
  destroyNode((node *)n);
}

static void mapDestroyEdge(BYTE *e) {
  destroyEdge((edge *)e);
}
