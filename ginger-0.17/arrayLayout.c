/* $Id: arrayLayout.c,v 1.4 1996/03/20 15:13:56 lupus Exp $ */
 
/***************************************************************
 * This file is part of the Ginger project.                    *
 * Created 09-02-1996                                          *
 * Niels C. Larsen, Aalborg University, Denmark                *
 * (lupus@iesd.auc.dk)                                         *
 * Martin K. Molz, Aalborg University, Denmark                 *
 * (molz@iesd.auc.dk)                                          *
 ***************************************************************/

#include "arrayLayout.h"
#include "graphDisplay.h"
#include "nodeSelection.h"
#include "clist.h"

/* ---  private declarations  --- */

static void privateComputeNewNodeLocations();
static void privateNextSelectedNode(BYTE *b);

static graph *currentGraph;
static int upperLeftX, upperLeftY;
static int nodeDistH, nodeDistV; 
static XPoint upperLeft, lowerRight;
static int numberOfNodeInRow;

static int nodeCursor, selectedValue;
static int currentNodeCursor, currentSelectedValue;
static int firstValueFund = 0;

/* ---  public functions  --- */

void array_layout(graph *g, int startPX, int startPY, int distX, int distY, int numberInRow) {
  if( nodeSelectCount(g) ) { 
    currentGraph=g;
    upperLeftX=startPX;
    upperLeftY=startPY;
    nodeDistH=distX;
    nodeDistV=distY;
    numberOfNodeInRow=numberInRow;

    nodeCursor=1; /* Counter to keep track of the iterate-position */
    selectedValue=0; /* Keeps track of the selected value under a iteration */ 
    currentNodeCursor=1; /* Keeps track of the position of the seleted node */
    currentSelectedValue = 0; /* Holds the selected value from iteration to iteration */

    firstValueFund=0;
    
    privateComputeNewNodeLocations();   

  }
}

static void privateComputeNewNodeLocations() { 
  int newX, newY, counter = 1;
  int rowCounter = 1;
  int columnCounter = 1;
  node *nextNode, *n1; 

/*  printf("Antal %d\n", nodeSelectCount(currentGraph)); */
  while( counter <= nodeSelectCount(currentGraph) ) {
/*    printf("I %d\n", counter); */
    counter++;    
    iterate(privateNextSelectedNode,currentGraph->nodes);
    
    nextNode=(node*)retrieve(currentNodeCursor,currentGraph->nodes);
    
    newX=upperLeftX+((columnCounter-1)*nodeDistH);
    newY=upperLeftY+((rowCounter-1)*nodeDistV);
    
    nextNode->x=newX;
    nextNode->y=newY;
    /* moveNode(currentGraph, nextNode, newX, newY); */
    if( columnCounter == numberOfNodeInRow) {
      rowCounter++;
      columnCounter=1;
    }
    else
      columnCounter++;

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
