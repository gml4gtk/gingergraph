/* $Id: graphDisplay.c,v 1.11 1996/06/14 12:19:06 lupus Exp $ */

/***************************************************************
 * This file is part of the Ginger project.                    *
 * Created 29-12-1995                                          *
 * Niels C. Larsen, Aalborg University, Denmark                *
 * (lupus@iesd.auc.dk)                                         *
 ***************************************************************/

/* ---  Functions to display a graph in the graph window  
        and to move nodes  --- */

#include <string.h>
#include "graphDisplay.h"
#include "rectangles.h"

#define EDGE_SELECTION_DISTANCE 5

/* ---  private variables  --- */

/* current_node and current_graph are used as a "second parameter" to 
   functions that are "mapped" over the graph's node or edge list */
static node *current_node;
static graph *current_graph;

/* min_p and max_p are the upper left and lower right corner
   respectively in the "bounding rectangle", i.e. the rectangle
   in which objects should be redisplayed e.g. when another object
   has moved  */
static XPoint min_p, max_p;

/* delta x and y to move selected nodes in a moveSelectedNodes() */
static int delta_x, delta_y;

/* max_x and max_y represent the "largest" coordinates in the graph */
static int max_x, max_y;

/* ---  private function headers  --- */

static void displayNode(BYTE *n);
static void displayEdge(BYTE *e);
static void moveEdgeIfAdjacent(BYTE *b);
static void displayEdgeIfInBoundingRectangle(BYTE *b);
static void displayNodeIfInBoundingRectangle(BYTE *b);
static void findNodeBoundingRectangle(graph *g, node *n);
static void boundingRectangleOfEdgeIfAdjacent(BYTE *b);
static void moveAdjacentEdges(graph *g, node *n);
static void redisplayObjectsInBoundingRectangle(graph *g);
static void updateMaxCoordinatesUsingNode(BYTE *n1);
static void GDundisplaySelectedNodes(BYTE *b);
static void GDfindBoundingRectangleOfSelectedNode(BYTE *b);
static void GDmoveAdjacentEdgesOfSelectedNodes(BYTE *b);

/* ---  public functions  --- */

/* perform a (re)display of the entire graph */
void displayGraph(graph *g) {
  iterate(displayEdge, g->edges);
  iterate(displayNode, g->nodes);
}

/* (re)display the part of the graph that is covered by the
   rectangle with corners upper_left and lower_right */
void displayGraphWithinRectangle(graph *g, XPoint upper_left, XPoint lower_right) {
  min_p=upper_left;
  max_p=lower_right;
  redisplayObjectsInBoundingRectangle(g);
}

/* move the node n to new posision (newx, newy) 
   Update all edges adjacent to n.
   The graph is redrawn as necessary to expose objects that were
   (partially) obscured by the moved node or edges */
void moveNode(graph *g, node *n, int newx, int newy) {
  min_p.x=max_p.x=n->x;
  min_p.y=max_p.y=n->y;
  
  /* delete node from old position */
  undisplayNodeImageOnScreen(n);
  
  /* initialize the bounding rectangle to the area of the node itself */
  min_p.x = n->x-n->width/2-NODE_MARGIN;
  min_p.y = n->y-n->height/2-NODE_MARGIN;
  max_p.x = n->x+n->width/2+NODE_MARGIN;
  max_p.y = n->y+n->height/2+NODE_MARGIN;

  /* determine the rectangle that might be affected by the move */
  findNodeBoundingRectangle(g, n);
  
  /* update coordinates */
  n->x = newx;
  n->y = newy;
  
  /* extend the bounding rectangle to cover the area of the node 
     after the move */
  min_p.x = MIN(min_p.x, n->x-n->width/2-NODE_MARGIN);
  min_p.y = MIN(min_p.y, n->y-n->height/2-NODE_MARGIN);
  max_p.x = MAX(max_p.x, n->x+n->width/2+NODE_MARGIN);
  max_p.y = MAX(max_p.y, n->y+n->height/2+NODE_MARGIN);
  
  /* update the position of adjacent edges */
  moveAdjacentEdges(g, n);

  /* redraw objects in bounding rectangle (including
   new image of the moved node and adjacent edges) */
  redisplayObjectsInBoundingRectangle(g);
}

/* move the selected nodes. The direction and distance to be moved 
   is taken as the vector from the center of "anchorNode" 
   to (newx, newy).
   The graph is redrawn as necessary to expose objects that were
   (partially) obscured by the moved node or edges */
void moveSelectedNodes(graph *g, node *anchorNode, int newx, int newy) {
  delta_x=newx-anchorNode->x;
  delta_y=newy-anchorNode->y;

  /* delete node from old position */
  iterate(GDundisplaySelectedNodes, g->nodes);

  /* initialize the bounding rectangle to the area of the node itself */
  min_p.x = anchorNode->x-anchorNode->width/2-NODE_MARGIN;
  min_p.y = anchorNode->y-anchorNode->height/2-NODE_MARGIN;
  max_p.x = anchorNode->x+anchorNode->width/2+NODE_MARGIN;
  max_p.y = anchorNode->y+anchorNode->height/2+NODE_MARGIN;

  /* determine the rectangle that might be affected by the move 
     and update the position of each selected node */
  current_graph=g;
  iterate(GDfindBoundingRectangleOfSelectedNode, g->nodes);

  /* update the position of adjacent edges */
  recalculateEdgeEndpointsOfSelectedNodes(g);
  
  /* redraw objects in bounding rectangle (including
   new image of the moved nodes and adjacent edges) */
  redisplayObjectsInBoundingRectangle(g);
}

/* update the position of endpoints of all edges adjacent to the 
   selected nodes */
void recalculateEdgeEndpointsOfSelectedNodes(graph *g) {
  current_graph=g;
  /* update the position of adjacent edges */
  iterate(GDmoveAdjacentEdgesOfSelectedNodes, g->nodes);
}

/* return a pointer to the node that covers the point (x,y).
   return 0 if the point isn't covered by any node */
node *nodeAtPoint(graph *g, int x, int y) {
  int i;
  node *n;
  XPoint p, rect1, rect2;
  
  p.x=x; p.y=y;
  
  /* here we perform an explicit iteration over the node list
     until we find the appropriate node (or reach end-of-list) */
  for(i=1; i<=getsize(g->nodes); i++) {
    n=(node *)retrieve(i,g->nodes);
    rect1.x = n->x-n->width/2-NODE_MARGIN;
    rect1.y = n->y-n->height/2-NODE_MARGIN;
    rect2.x = n->x+n->width/2+NODE_MARGIN;
    rect2.y = n->y+n->height/2+NODE_MARGIN;
    if( pointInRectangle(p,rect1,rect2) ) {
      return n;
    }
  }
  
  /* no appropriate node found if we reach this statement */
  return 0;
}

/* return a pointer to the edge that is "near" the point (x,y).
   return 0 if no edge is near to the point.
   "near" is defined as less than the distance 
   EDGE_SELECTION_DISTANCE */
edge *edgeAtPoint(graph *g, int x, int y) {
  int i;
  edge *e;
  int length, dist1, dist2;
  
  /* here we perform an explicit iteration over the edge list
     until we find the appropriate edge (or reach end-of-list) */
  for(i=1; i<=getsize(g->edges); i++) {
    e=(edge *)retrieve(i,g->edges);
    
    if( edgeDistanceFromPoint(e, x, y) < EDGE_SELECTION_DISTANCE ) {
      length=DIST(e->p1.x, e->p1.y, e->p2.x, e->p2.y);
      dist1 =DIST(e->p1.x, e->p1.y, x, y);
      dist2 =DIST(x, y, e->p2.x, e->p2.y);
      if( dist1+dist2 < length+EDGE_SELECTION_DISTANCE ) {
	return e;
      }
    }
  }
  
  /* no appropriate edge found if we reach this statement */
  return 0;
}

/* find the lower left corner of the graph */
void graphMaxCoordinates(graph *g, 
			 int *return_max_x, 
			 int *return_max_y ) 
{
  max_x=0;
  max_y=0;
  
  iterate(updateMaxCoordinatesUsingNode, g->nodes);
  *return_max_x=max_x;
  *return_max_y=max_y;
}

/* delete an edge. Update the graph display, including "expose" */
void graphDisplayDeleteEdge(graph *g, edge *e) {
  
  /* determine bounding rectangle of edge */
  min_p.x = MIN(e->p1.x, e->p2.x);
  min_p.y = MIN(e->p1.y, e->p2.y);
  max_p.x = MAX(e->p1.x, e->p2.x);
  max_p.y = MAX(e->p1.y, e->p2.y);
  
  /* remove the edge from the display */
  undrawEdge(e);
  
  /* destroy edge object and remove it from the graph */
  deleteEdge(g,e);
  
  /* redraw objects in bounding rectangle */ 
  redisplayObjectsInBoundingRectangle(g);
}

/* delete the selected edges. Update the graph display, incl. "expose".
   Return the number of edges deleted.  */
int graphDisplayDeleteSelectedEdges(graph *g) {
  int i,num;
  edge *e;
  int first=1;
  
  num=0;
  /* perform an explicit iteration over the edge list */
  for(i=1; i<=getsize(g->edges); i++) {
    e=(edge *)retrieve(i,g->edges);
    if( e->selected ) {
      /* determine bounding rectangle (B.R.) of edge */
      if(first) { /* initialize B.R. */
	first=0;
	min_p.x = MIN(e->p1.x, e->p2.x);
	min_p.y = MIN(e->p1.y, e->p2.y);
	max_p.x = MAX(e->p1.x, e->p2.x);
	max_p.y = MAX(e->p1.y, e->p2.y);
      }
      else {      /* expand B.R. */
	min_p.x = MIN(MIN(e->p1.x, e->p2.x), min_p.x);
	min_p.y = MIN(MIN(e->p1.y, e->p2.y), min_p.y);
	max_p.x = MAX(MAX(e->p1.x, e->p2.x), max_p.x);
	max_p.y = MAX(MAX(e->p1.y, e->p2.y), max_p.y);
      }
      
      /* remove the edge from the display */
      undrawEdge(e);
      
      /* destroy edge object and remove it from the graph */
      deleteEdge(g,e);
      
      num++;

      i--; /* very ugly hack to "fix" the problem that we're
	      iterating over the edge list while at the same time
	      removing items from it. By decreasing the loop
	      variable, we depend on the corresponding decrease 
	      of getsize(g->edges) to terminate the loop */
    }
  }
  
  /* redraw objects in bounding rectangle */ 
  redisplayObjectsInBoundingRectangle(g);

  return num;
}

/* delete the node. Update the graph display, incl. "expose" */
void graphDisplayDeleteNode(graph *g, node *n) {
  int j;
  edge *e;

  /* initialize B.R. */
  min_p.x = (n->x)-((n->width)/2)-1;
  min_p.y = (n->y)-((n->height)/2)-1;
  max_p.x = (n->x)+((n->width)/2)+1;
  max_p.y = (n->y)+((n->height)/2)+1;
  
  for(j=1; j<=getsize(g->edges); j++) {
    e=(edge *)retrieve(j,g->edges);
    if( (n==e->to) || (n==e->from) ) {
      min_p.x = MIN(MIN(e->p1.x, e->p2.x), min_p.x);
      min_p.y = MIN(MIN(e->p1.y, e->p2.y), min_p.y);
      max_p.x = MAX(MAX(e->p1.x, e->p2.x), max_p.x);
      max_p.y = MAX(MAX(e->p1.y, e->p2.y), max_p.y);
	
      /* remove the edge from the display */
      undrawEdge(e);
	  
      /* destroy edge object and remove it from the graph */
      deleteEdge(g,e);
	  
      j--; /* very ugly hack to "fix" the problem that we're
	      iterating over the edge list while at the same time
	      removing items from it. By decreasing the loop
	      variable, we depend on the corresponding decrease 
	      of getsize(g->edges) to terminate the loop */
      
	}
  }
  /* remove the node from the display */
  undisplayNodeImageOnScreen(n);
  
  /* destroy node object and remove it from the graph */
  deleteNode(g, n);
  
  /* redraw objects in bounding rectangle */ 
  redisplayObjectsInBoundingRectangle(g);
}

/* delete the selected nodes. Update the graph display, incl. "expose".
   Return the number of nodes deleted.  */
int graphDisplayDeleteSelectedNodes(graph *g) {
  node *n;
  edge *e;
  int i,j,num;
  int first=1;
  
  num=0;
  /* perform an explicit iteration over the node list */
  for(i=1; i<=getsize(g->nodes); i++) {
    n=(node *)retrieve(i,g->nodes);
    if( n->selected ) {
      if(first) { /* initialize B.R. */
	first=0;
	min_p.x = (n->x)-((n->width)/2)-1;
	min_p.y = (n->y)-((n->height)/2)-1;
	max_p.x = (n->x)+((n->width)/2)+1;
	max_p.y = (n->y)+((n->height)/2)+1;
      }
      else {      /* expand B.R. */
	min_p.x = MIN((n->x)-((n->width)/2)-1, min_p.x);
	min_p.y = MIN((n->y)-((n->height)/2)-1, min_p.y);
	max_p.x = MAX((n->x)+((n->width)/2)+1, max_p.x);
	max_p.y = MAX((n->y)+((n->height)/2)+1, max_p.y);
      }
      
      for(j=1; j<=getsize(g->edges); j++) {
	e=(edge *)retrieve(j,g->edges);
	if( (n==e->to) || (n==e->from) ) {
	  min_p.x = MIN(MIN(e->p1.x, e->p2.x), min_p.x);
	  min_p.y = MIN(MIN(e->p1.y, e->p2.y), min_p.y);
	  max_p.x = MAX(MAX(e->p1.x, e->p2.x), max_p.x);
	  max_p.y = MAX(MAX(e->p1.y, e->p2.y), max_p.y);
	
	  /* remove the edge from the display */
	  undrawEdge(e);
	  
	  /* destroy edge object and remove it from the graph */
	  deleteEdge(g,e);
	  
	  j--; /* very ugly hack to "fix" the problem that we're
		  iterating over the edge list while at the same time
		  removing items from it. By decreasing the loop
		  variable, we depend on the corresponding decrease 
		  of getsize(g->edges) to terminate the loop */
	  
	}
      }
      /* remove the node from the display */
      undisplayNodeImageOnScreen(n);
      
      /* destroy node object and remove it from the graph */
      deleteNode(g, n);
      
      num++;

      i--; /* very ugly hack to "fix" the problem that we're
	      iterating over the nodes list while at the same time
	      removing items from it. By decreasing the loop
	      variable, we depend on the corresponding decrease 
	      of getsize(g->nodes) to terminate the loop */
    }
  }
  /* redraw objects in bounding rectangle */ 
  redisplayObjectsInBoundingRectangle(g);

  return num;
}

/* ---  private functions  --- */

/* -  private functions used by displayGraph()  - */

static void displayNode(BYTE *n) {
  displayNodeImageOnScreen((node *)n);
}

static void displayEdge(BYTE *e) {
  drawEdge((edge *)e);
}

/* -  private functions used by moveNode()  - */

/* called by moveAdjacentEdges() */
static void moveEdgeIfAdjacent(BYTE *b) {
  edge *e;
  
  e=(edge *)b;
  
  if( current_node == e->from || current_node == e->to ) {
    /* undisplay old edge */
    undrawEdge(e);
    /* calculate new coordinates */
    calculateEdgeEndpoints(e);
  }
}

/* move all edges adjacent to node n.
   Used when the node n has moved 
*/
static void moveAdjacentEdges(graph *g, node *n) {
  current_node=n;
  iterate(moveEdgeIfAdjacent, g->edges);
}

/* called by findNodeBoundingRectangle().
   The function extends (if necessary) the bounding rectangle to cover this
   edge, if the edge is adjacent to "current_node"
*/
static void boundingRectangleOfEdgeIfAdjacent(BYTE *b) {
  edge *e;
  
  e=(edge *)b;
  
  if( current_node == e->from || current_node == e->to ) {
    min_p.x = MIN(MIN(e->p1.x, e->p2.x), min_p.x);
    min_p.y = MIN(MIN(e->p1.y, e->p2.y), min_p.y);
    max_p.x = MAX(MAX(e->p1.x, e->p2.x), max_p.x);
    max_p.y = MAX(MAX(e->p1.y, e->p2.y), max_p.y);
  }
}

/* find a bounding rectangle that covers the node n and all its
   adjacent edges
*/
static void findNodeBoundingRectangle(graph *g, node *n) {
  /* extend the bounding rectangle to cover the area of the node */
  min_p.x = MIN(min_p.x, n->x-n->width/2-NODE_MARGIN);
  min_p.y = MIN(min_p.y, n->y-n->height/2-NODE_MARGIN);
  max_p.x = MAX(max_p.x, n->x+n->width/2+NODE_MARGIN);
  max_p.y = MAX(max_p.y, n->y+n->height/2+NODE_MARGIN);
  
  /* iterate over edges, extend the bounding rect. to cover all 
     adjacent edges */
  current_node=n;
  iterate(boundingRectangleOfEdgeIfAdjacent, g->edges);
  
  /* printf("%d %d  -- ", min_p.x, min_p.y);
  printf("%d %d\n", max_p.x, max_p.y);
  */
}

/* called by redisplayObjectsInBoundingRectangle() */
static void displayNodeIfInBoundingRectangle(BYTE *b) {
  node *n;
  XPoint p1, p2;
  
  n=(node *)b;
  
  p1.x = n->x-n->width/2-NODE_MARGIN;
  p1.y = n->y-n->height/2-NODE_MARGIN;
  p2.x = n->x+n->width/2+NODE_MARGIN;
  p2.y = n->y+n->height/2+NODE_MARGIN;
  
  if(rectangleOverlap(p1, p2, min_p, max_p)) {
    displayNodeImageOnScreen(n);
    /* printf("n: %s\n", n->name); */
  }
}

/* called by redisplayObjectsInBoundingRectangle() */
static void displayEdgeIfInBoundingRectangle(BYTE *b) {
  edge *e;
  
  e=(edge *)b;
  
  if(rectangleOverlap(e->p1, e->p2, min_p, max_p)) {
    drawEdge(e);
    /* printf("e: %d %d\n", e->p1.x, e->p1.y); */
  }
}

/* redisplay edges and nodes that overlap with the bounding rectangle */
static void redisplayObjectsInBoundingRectangle(graph *g) {
  iterate(displayEdgeIfInBoundingRectangle, g->edges);
  iterate(displayNodeIfInBoundingRectangle, g->nodes);
  /* printf("---\n"); */
}

/* called by graphMaxCoordinates() */
static void updateMaxCoordinatesUsingNode(BYTE *n1) {
  node *n;
  n=(node *)n1;
  
  max_x=MAX(max_x, n->x+n->width/2+NODE_MARGIN);
  max_y=MAX(max_y, n->y+n->height/2+NODE_MARGIN);
}

static void GDundisplaySelectedNodes(BYTE *b) {
  node *n;
  n=(node *)b;
  
  if(n->selected) {
    undisplayNodeImageOnScreen(n);
  }
}

static void GDfindBoundingRectangleOfSelectedNode(BYTE *b) {
  node *n;
  n=(node *)b;
  
  if(n->selected) {
    findNodeBoundingRectangle(current_graph, n);
    
    /* update node position */
    n->x+=delta_x;
    n->y+=delta_y;
    
    /* extend the bounding rectangle to cover the area of the node 
       after the move */
    min_p.x = MIN(min_p.x, n->x-n->width/2-NODE_MARGIN);
    min_p.y = MIN(min_p.y, n->y-n->height/2-NODE_MARGIN);
    max_p.x = MAX(max_p.x, n->x+n->width/2+NODE_MARGIN);
    max_p.y = MAX(max_p.y, n->y+n->height/2+NODE_MARGIN);
  }
}

static void GDmoveAdjacentEdgesOfSelectedNodes(BYTE *b) {
  node *n;
  n=(node *)b;
  
  if(n->selected) {
    moveAdjacentEdges(current_graph, n);
  }
}
