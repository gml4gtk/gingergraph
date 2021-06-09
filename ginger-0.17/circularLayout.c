/* $Id: circularLayout.c,v 1.3 1996/03/20 15:13:21 lupus Exp $ */
 
/***************************************************************
 * This file is part of the Ginger project.                    *
 * Created 09-02-1996                                          *
 * Niels C. Larsen, Aalborg University, Denmark                *
 * (lupus@iesd.auc.dk)                                         *
 * Martin K. Molz, Aalborg University, Denmark                 *
 * (molz@iesd.auc.dk)                                          *
 ***************************************************************/

#include "circularLayout.h"
#include "graphDisplay.h"
#include "nodeSelection.h"
#include "clist.h"
#include <math.h>

/* ---  private declarations  --- */

static void privateComputeNewNodeLocations();
static void privateNextSelectedNode(BYTE *b);

static double twoPi, peripheryDistance;
static int currentNumberOfPlacedNodes = 0;
static double circleCenterX, circleCenterY, circleRadius;
static graph *currentGraph;
static XPoint upperLeft, lowerRight;
static int nodeCursor, selectedValue;
static int currentNodeCursor, currentSelectedValue;
static int firstValueFund = 0;

/* ---  public functions  --- */

void circle_layout(graph *g, int origoX, int origoY, int radius) {
  if( nodeSelectCount(g) ) { 
    twoPi = 2*acos(-1);
    circleCenterX=origoX; 
    circleCenterY=origoY;
    circleRadius=radius;
    currentGraph=g;
    nodeCursor=1; /* Counter to keep track of the iterate-position */
    selectedValue=0; /* Keeps track of the selected value under a iteration */ 
    currentNodeCursor=1; /* Keeps track of the position of the seleted node */
    currentSelectedValue = 0; /* Holds the selected value from iteration to iteration */
    firstValueFund=0;
    currentNumberOfPlacedNodes = 0;
    peripheryDistance = (twoPi/nodeSelectCount(currentGraph));
    
    privateComputeNewNodeLocations();   
  }
}

static void privateComputeNewNodeLocations() { 
  int newX, newY, counter; 
  node *nextNode, *n1; 
  counter = 1;

/*  printf("Antal %d\n", nodeSelectCount(currentGraph)); */
  while( counter <= nodeSelectCount(currentGraph) ) {
/*    printf("I %d\n", counter); */
    counter++;    

    iterate(privateNextSelectedNode,currentGraph->nodes);
    
    nextNode=(node*)retrieve(currentNodeCursor,currentGraph->nodes);
    newX=circleCenterX + circleRadius*cos(currentNumberOfPlacedNodes*peripheryDistance);
    newY=circleCenterY + circleRadius*sin(-currentNumberOfPlacedNodes*peripheryDistance);
    /* moveNode(currentGraph, nextNode, newX, newY); */
    nextNode->x=newX;
    nextNode->y=newY;

    currentNumberOfPlacedNodes++;

    firstValueFund = 0;
    currentSelectedValue = selectedValue ;
    selectedValue = 0;
    nodeCursor = 1;
    currentNodeCursor = 1;
  }   

  recalculateEdgeEndpointsOfSelectedNodes(currentGraph); /* recalculate edge endpoints */
  graphBufferRedrawGraph();              /* make a total redisplay of the
					    graph */
}

static void privateNextSelectedNode(BYTE *b) {
  node *n;
  n=(node*)b;
  if( !firstValueFund ) {
    if( n->selected > currentSelectedValue ){
      selectedValue = n->selected;
      firstValueFund = 1;
      currentNodeCursor = nodeCursor;
    }
  }
  else
    if( (n->selected > currentSelectedValue) && 
       (n->selected < selectedValue) ) {
      selectedValue = n->selected;
      currentNodeCursor = nodeCursor;
    } 
  nodeCursor++;

/*  printf("n->selected %d \n", n->selected);
  printf("currentSelectedValue %d \n", currentSelectedValue);   
  printf("selectedValue %d \n", selectedValue);  
  printf("nodeCursor %d \n", nodeCursor);  
  printf("currentNodeCursor %d \n", currentNodeCursor); */
}
