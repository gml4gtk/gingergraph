/* $Id: treeLayout.c,v 1.4 1996/03/20 15:13:37 lupus Exp $ */
 
/***************************************************************
 * This file is part of the Ginger project.                    *
 * Created 15-02-1996                                          *
 * Niels C. Larsen, Aalborg University, Denmark                *
 * (lupus@iesd.auc.dk)                                         *
 * Martin K. Molz, Aalborg University, Denmark                 *
 * (molz@iesd.auc.dk)                                          *
 ***************************************************************/

/* implementation of a hierarchy/tree layout algorithm.
   The implementation is based on the algorithm presented in the article
   "Methods for Visual Understanding of Hierarchical System Structures"
   by Kozo Sugiyama, Shojiro Tagawa, and Mitsuhiko Toda,
   in "IEEE Transactions on Systems, Man, and Cybernetics" February 1981,
   volume 11 number 2.
*/

#include "treeLayout.h"
#include "graphDisplay.h"
#include "graphBuffer.h"
#include "nodeSelection.h"
#include "mathMacros.h"
#include "matrix.h"
#include "clist.h"

/* ---  private declarations  --- */

#define FIRST_LEVEL 0   /* must be zero */
#define MAX_LEVELS 100

static int min_level, max_level;
static int number_of_nodes_at_level[MAX_LEVELS];
static matrix *matrices[MAX_LEVELS];
static int y_distance, y_corner, x_distance, x_corner;
static int vertical_tree;

/* private function prototypes */

static void privateComputeNodeLevel(graph *g);
static void mapResetNode(BYTE *n1);
static void mapAdjustLevel(BYTE *n1);
static void privateOnUpperNeighbor(graph *g, node *n1, node *n2);
static void privateOnLowerNeighbor(graph *g, node *n1, node *n2);
static void privateBuildAndInitializeMatrices(graph *g);
static void privateUpdateNodePositions();
static int  privateLowestNodePositions();
static void privateAssignNewNodeCoordinates(int vert, int min_pos);

/* ---  public functions  --- */

/* tree_layout()
parameters:
   g         the graph object
   cornerX   x coordinate of the upper left corner of the
             "layout area"
   cornerY   y coordinate of the upper left corner of the
             "layout area"
   distX     horizontal space between node centers in the layout 
   distY     vertical   space between node centers in the layout 
   vertical  if non-zero, the hierarchy is laid out with parent nodes
             at the top, otherwise a left-to-right layout is performed
   crossingIterations    number of times to repeat the down-up procedures
                         to reduce the number of crossing edges.
   placementIterations	 number of times to apply down and up procedures
                         to adjust the position of nodes after edge crossing
			 have been reduced
*/
void tree_layout(graph *g, int cornerX, int cornerY, 
		 int distX, int distY, int vertical, 
		 int crossingIterations, int placementIterations) 
{
  int i,j,k;
  edge *e;
  node *n1, *n2;
  int min_position;
  
  /* there must be at least two selected nodes to operate on */
  if( nodeSelectCount(g)<2 )
    return;
  
  /* initialize variables */
  min_level=max_level=0;
  y_distance=distY;
  y_corner=cornerY;
  x_distance=distX;
  x_corner=cornerX;
  vertical_tree=vertical;
  
  for(i=0; i<MAX_LEVELS; i++) {
    number_of_nodes_at_level[i]=0;
  }

  /* reset the layout book-keeping on the nodes */
  iterate(mapResetNode, g->nodes);
  
  /* assign a level to all selected nodes by a node traversal. 
     Keep track of the lowest node level assigned, save in min_level */
  privateComputeNodeLevel(g);

  /* using min_level, adjust the level of nodes so that 
     the top level will be level zero. In the same iteration, determine
     the highest level number assigned. */
  iterate(mapAdjustLevel, g->nodes);
  
  if( max_level==0 )  /* we cannot operate on a graph with only one level */
    return;

  min_level=FIRST_LEVEL;

  /* build and initialize a matrix representation of the selected
     nodes and their connecting edges */
  privateBuildAndInitializeMatrices(g);
  
  /* permute the matrices' rows and columns to reduce edge crossings */
  if(crossingIterations>0)
    matrixArrayDownUpIterate(matrices, max_level-min_level, crossingIterations);
  
  /* update the nodes' position variable to their position in the
     matrices after the permutation */
  privateUpdateNodePositions();

  /* adjust the nodes' position to achieve a "nice" visual layout, where
     connected nodes are placed as near to each other as possible */
  for(j=0; j<placementIterations; j++) {
    for(i=min_level; i<max_level; i++) {
      matrixPriorityLayoutDown(matrices[i]);
    }
    j++;
    if(j<placementIterations)
      for(i=max_level-1; i>=min_level; i--) {
	matrixPriorityLayoutUp(matrices[i]);
      }
  }
  
  /* after moving node positions, the lowest used node position may have 
     become displaced from zero. We want to find the lowest position
     so that we can bring "leftmost" nodes back to pos zero. */
  min_position=privateLowestNodePositions();

  /* assign new coordinates to nodes, based on their positions and levels */
  privateAssignNewNodeCoordinates(vertical, min_position);
  
  recalculateEdgeEndpointsOfSelectedNodes(g); /* recalculate edge endpoints */
  graphBufferRedrawGraph();              /* make a total redisplay of the
					    graph */
  
  /* clean up */
  for(i=min_level; i<max_level; i++) {
    matrixDestroy(matrices[i]);         /* destroy matrices */
  }
}

/* ---  private functions  --- */

static void privateOnUpperNeighbor(graph *g, node *n1, node *n2) {
  if( !n1->selected )
    return;
  if( n1->tree_layout.level )
    return;
  n1->tree_layout.level=n2->tree_layout.level-1;
  min_level = MIN(min_level, n1->tree_layout.level);
  onNeighborsDo(g, n1, privateOnUpperNeighbor, privateOnLowerNeighbor);
}

static void privateOnLowerNeighbor(graph *g, node *n1, node *n2) {
  if( !n1->selected )
    return;
  if( n1->tree_layout.level )
    return;
  n1->tree_layout.level=n2->tree_layout.level+1;
  onNeighborsDo(g, n1, privateOnUpperNeighbor, privateOnLowerNeighbor);
}

/* assign levels to all selected nodes */
static void privateComputeNodeLevel(graph *g) { 
  node *n;
  int k;
  
  for(k=1; k<=getsize(g->nodes); k++) {
    n=(node *)retrieve(k,g->nodes);
    if( n->selected )
      /* perform a (depth-first) traversal of the selected nodes */
      onNeighborsDo(g, n, privateOnUpperNeighbor, privateOnLowerNeighbor);
  }
}

static void mapResetNode(BYTE *n1) {
  node *n;
  
  n=(node *)n1;
  n->tree_layout.level=0;
  n->tree_layout.position=0;
  n->tree_layout.priority=0;
  n->tree_layout.upper_connectivity=0;
  n->tree_layout.lower_connectivity=0;
}

static void mapAdjustLevel(BYTE *n1) {
  node *n;
  int level;
  
  n=(node *)n1;
  
  if(n->selected) {
    n->tree_layout.level+=(FIRST_LEVEL-min_level);
    level=n->tree_layout.level;
    if(level<MAX_LEVELS) {
      max_level=MAX(max_level, level);
      n->tree_layout.position=number_of_nodes_at_level[level]++;
    }
  }
}

static void privateBuildAndInitializeMatrices(graph *g) {
  int i,j,k;
  node *n1, *n2;
  edge *e;
  
  for(i=min_level; i<max_level; i++) {
    j=0;
    /* create level matrix */
    matrices[i]=matrixCreate(number_of_nodes_at_level[i],
			     number_of_nodes_at_level[i+1]);
    
    /* perform an explicit iteration over the node list 
       to associate node objects with rows and columns in the 
       level matrix */
    for(k=1; k<=getsize(g->nodes); k++) {
      n1=(node *)retrieve(k,g->nodes);
      if( n1->selected && n1->tree_layout.level==i ) {
	matrixAtRowPutNode(matrices[i], n1->tree_layout.position, n1);
      }
      if( n1->selected && n1->tree_layout.level==i+1 ) {
	matrixAtColumnPutNode(matrices[i], n1->tree_layout.position, n1);
      }
    }
    
    /* perform an explicit iteration over the edge list 
       to fill in the matrix elements (put 1's where appropriate).
       Also determine the upper and lower connectivity of the nodes
       along the way */
    for(k=1; k<=getsize(g->edges); k++) {
      e=(edge *)retrieve(k,g->edges);
      n1=e->from;
      n2=e->to;
      if( n1->selected && n2->selected ) {
	if( (n1->tree_layout.level==i && n2->tree_layout.level==i+1) ) {
	  matrixAtPut(matrices[i], n1->tree_layout.position, 
		      n2->tree_layout.position, 1);
	  n1->tree_layout.lower_connectivity++;
	  n2->tree_layout.upper_connectivity++;
	}
	if( (n2->tree_layout.level==i && n1->tree_layout.level==i+1) ) {
	  matrixAtPut(matrices[i], n2->tree_layout.position,
		      n1->tree_layout.position, 1);
	  n2->tree_layout.lower_connectivity++;
	  n1->tree_layout.upper_connectivity++;
	}
      }
    }
  }
}

static void privateUpdateNodePositions() {
  int i,j;
  node *n1;

  for(i=min_level; i<max_level; i++) {
    for(j=0; j<number_of_nodes_at_level[i]; j++) {
      n1=matrixNodeAtRow(matrices[i], j);
      if(n1) {
	n1->tree_layout.position=j;
      }
    }
  }
  i=max_level;
  for(j=0; j<number_of_nodes_at_level[i]; j++) {
    n1=matrixNodeAtColumn(matrices[i-1], j);
    if(n1) {
      n1->tree_layout.position=j;
    }
  }

}

static int privateLowestNodePositions() {
  int i,j,res;
  node *n1;
  
  /* determine lowest position */
  res=100;
  for(i=min_level; i<max_level; i++) {
    for(j=0; j<number_of_nodes_at_level[i]; j++) {
      n1=matrixNodeAtRow(matrices[i], j);
      if(n1) {
	res=MIN(res, n1->tree_layout.position);
      }
    }
  }
  i=max_level;
  for(j=0; j<number_of_nodes_at_level[i]; j++) {
    n1=matrixNodeAtColumn(matrices[i-1], j);
    if(n1) {
      res=MIN(res, n1->tree_layout.position);
    }
  }
  
  return res;
}

static void privateAssignNewNodeCoordinates(int vert, int min_pos) {
  int i,j;
  node *n1;
  
  /* convert position to x coordinates */
  for(i=min_level; i<max_level; i++) {
    for(j=0; j<number_of_nodes_at_level[i]; j++) {
      n1=matrixNodeAtRow(matrices[i], j);
      if(n1) {
	if(vert) {
	  n1->x=x_corner+(n1->tree_layout.position-min_pos)*x_distance;
	  n1->y=y_corner+y_distance*n1->tree_layout.level;
	} 
	else {
	  n1->x=x_corner+x_distance*n1->tree_layout.level;
	  n1->y=y_corner+(n1->tree_layout.position-min_pos)*y_distance;
	}
      }
    }
  }
  i=max_level;
  for(j=0; j<number_of_nodes_at_level[i]; j++) {
    n1=matrixNodeAtColumn(matrices[i-1], j);
    if(n1) {
      if(vert) {
	n1->x=x_corner+(n1->tree_layout.position-min_pos)*x_distance;
	n1->y=y_corner+y_distance*n1->tree_layout.level;
      } 
      else {
	n1->x=x_corner+x_distance*n1->tree_layout.level;
	n1->y=y_corner+(n1->tree_layout.position-min_pos)*y_distance;
      }
    }
  }
}

