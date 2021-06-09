/* $Id: nodeSelection.c,v 1.2 1996/02/13 09:18:58 lupus Exp $ */
 
/***************************************************************
 * This file is part of the Ginger project.                    *
 * Created 08-02-1996                                          *
 * Niels C. Larsen, Aalborg University, Denmark                *
 * (lupus@iesd.auc.dk)                                         *
 * Martin K. Molz, Aalborg University, Denmark                 *
 * (molz@iesd.auc.dk)                                          *
 ***************************************************************/

/* Functions to select and unselect nodes. These functions also
   maintain the visual indication of which nodes are selected */
 
#include "nodeSelection.h"
#include "clist.h"

/* ---  private declarations  --- */

/* currentSelectionNumber is used for defining a total order on the 
   selected nodes. It is incremented each time a node is selected, and
   is reset by nodeUnselectAll() */
static int currentSelectionNumber = 1;

/* the_count is used by nodeSelectCount() */
static int the_count;

/* private function prototypes */
static void privateNodeSelect(BYTE *b);
static void privateNodeUnselect(BYTE *b);
static void privateNodeToggleSelection(BYTE *b);
static void privateNodeSelectionCount(BYTE *b);
static void nodeBasicSelect(node *n);
static void nodeBasicUnselect(node *n);

/* ---  public functions  --- */

/* select the node, according to the mode */
void nodeSelect(graph *g, node *n, int mode) {
  switch(mode){
  case ACCUMULATING_MODE:
    nodeBasicSelect(n);
    break;
  case NON_ACCUMULATING_MODE:
    nodeUnselectAll(g);
    nodeBasicSelect(n);
    break;
  }
}

/* unselect the node independently of the mode */
void nodeUnselect(graph *g, node *n) {
  nodeBasicUnselect(n);
}

/* toggle whether or not the node is selected, according to the mode */
void nodeToggleSelection(graph *g, node *n, int mode) {
  if( n->selected ) 
    nodeUnselect(g,n);
  else
    nodeSelect(g,n,mode);
}

/* select all nodes */
void nodeSelectAll(graph *g) {
  iterate(privateNodeSelect,g->nodes);
}

/* unselect all nodes */
void nodeUnselectAll(graph *g) {
  currentSelectionNumber = 1;  /* reset the selection number */
  iterate(privateNodeUnselect,g->nodes);
}

/* select the nodes that are currently unselected. In accumulating mode
   the is equivalent to "select all", in non-accumulating mode, it is
   an "invert selection" operation */
void nodeSelectUnselected(graph *g, int mode) {
  switch(mode){
  case ACCUMULATING_MODE:
    nodeSelectAll(g);
    break;
  case NON_ACCUMULATING_MODE:
    iterate(privateNodeToggleSelection,g->nodes);
    break;
  }
}

/* return the number of nodes currently selected */
int  nodeSelectCount(graph *g) {
  the_count=0;
  iterate(privateNodeSelectionCount,g->nodes);
  return the_count;
}

node *getFirstSelectedNode(graph *g) {
  int i;
  node *n;
  
  for(i=1; i<=getsize(g->nodes); i++) {
    n=(node *)retrieve(i,g->nodes);
    if(n->selected)
      return n;
  }
  return 0;
}

/* ---  private functions  --- */


/* basic select/unselect operations which deal with incrementing the 
   currentSelectionNumber, highlighting and displaying the node */

static void nodeBasicSelect(node *n) {
  n->selected = currentSelectionNumber++;
  nodeHighlight(n,1);
  displayNodeImageOnScreen(n);
}

static void nodeBasicUnselect(node *n){
  n->selected = 0;
  nodeHighlight(n,0);
  displayNodeImageOnScreen(n);
}

/* the following "privateNode"-functions are all called by an 
   iterator function in the above functions */

static void privateNodeSelect(BYTE *b) {
  node *n;
  n = (node*)b;
  nodeBasicSelect(n);
}

static void privateNodeUnselect(BYTE *b) {
  node *n;
  n = (node*)b;
  nodeBasicUnselect(n);
}

static void privateNodeToggleSelection(BYTE *b) {
  node *n;
  n = (node*)b;
  if( n->selected==0 )
    nodeBasicSelect(n);
  else
    nodeBasicUnselect(n);
}

static void privateNodeSelectionCount(BYTE *b) {
  node *n;
  n = (node*)b;
  if( n->selected )
    the_count++;
}

