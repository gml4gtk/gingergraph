/* $Id: edgeSelection.c,v 1.2 1996/03/04 08:50:05 lupus Exp $ */
 
/***************************************************************
 * This file is part of the Ginger project.                    *
 * Created 10-02-1996                                          *
 * Niels C. Larsen, Aalborg University, Denmark                *
 * (lupus@iesd.auc.dk)                                         *
 * Martin K. Molz, Aalborg University, Denmark                 *
 * (molz@iesd.auc.dk)                                          *
 ***************************************************************/

/* Functions to select and unselect edges. These functions also
   maintain the visual indication of which edges are selected */
 
#include "edgeSelectionWindow.h"
#include "edgeSelection.h"
#include "clist.h"

/* ---  private declarations  --- */

/* currentSelectionNumber is used for defining a total order on the 
   selected edges. It is incremented each time a edge is selected, and
   is reset by edgeUnselectAll() */
static int currentSelectionNumber = 1;

/* the_count is used by edgeSelectCount() */
static int the_count;

/* private function prototypes */
static void privateEdgeSelect(BYTE *b);
static void privateEdgeUnselect(BYTE *b);
static void privateEdgeToggleSelection(BYTE *b);
static void privateEdgeSelectionCount(BYTE *b);
static void edgeBasicSelect(edge *e);
static void edgeBasicUnselect(edge *e);

/* ---  public functions  --- */

/* select the edge, according to the mode */
void edgeSelect(graph *g, edge *e, int mode) {
  switch(mode){
  case ACCUMULATING_MODE:
    edgeBasicSelect(e);
    break;
  case NON_ACCUMULATING_MODE:
    edgeUnselectAll(g);
    edgeBasicSelect(e);
    break;
  }
}

/* unselect the edge independently of the mode */
void edgeUnselect(graph *g, edge *e) {
  edgeBasicUnselect(e);
}

/* toggle whether or not the edge is selected, according to the mode */
void edgeToggleSelection(graph *g, edge *e, int mode) {
  if( e->selected ) 
    edgeUnselect(g,e);
  else
    edgeSelect(g,e,mode);
}

/* select all edges */
void edgeSelectAll(graph *g) {
  iterate(privateEdgeSelect,g->edges);
}

/* unselect all edges */
void edgeUnselectAll(graph *g) {
  currentSelectionNumber = 1;  /* reset the selection number */
  iterate(privateEdgeUnselect,g->edges);
}

/* select the edges that are currently unselected. In accumulating mode
   the is equivalent to "select all", in non-accumulating mode, it is
   an "invert selection" operation */
void edgeSelectUnselected(graph *g, int mode) {
  switch(mode){
  case ACCUMULATING_MODE:
    edgeSelectAll(g);
    break;
  case NON_ACCUMULATING_MODE:
    iterate(privateEdgeToggleSelection,g->edges);
    break;
  }
}

/* return the number of edges currently selected */
int  edgeSelectCount(graph *g) {
  the_count=0;
  iterate(privateEdgeSelectionCount,g->edges);
  return the_count;
}

void edgeSelectInCurrentMode(graph *g, edge *e) {
  edgeSelect(g, e, edgeSelectionWindowGetAccumulationState() );
}

void edgeToggleSelectionInCurrentMode(graph *g, edge *e) {
  edgeToggleSelection(g, e, edgeSelectionWindowGetAccumulationState() );
}

void edgeSelectUnselectedInCurrentMode(graph *g) {
  edgeSelectUnselected(g, edgeSelectionWindowGetAccumulationState() );
}

edge *getFirstSelectedEdge(graph *g) {
  int i;
  edge *e;
  
  for(i=1; i<=getsize(g->edges); i++) {
    e=(edge *)retrieve(i,g->edges);
    if(e->selected)
      return e;
  }
  return 0;
}

/* ---  private functions  --- */


/* basic select/unselect operations which deal with incrementing the 
   currentSelectionNumber, highlighting and displaying the edge */

static void edgeBasicSelect(edge *e) {
  e->selected = currentSelectionNumber++;
  edgeHighlight(e,1);
}

static void edgeBasicUnselect(edge *e){
  e->selected = 0;
  edgeHighlight(e,0);
}

/* the following "privateEdge"-functions are all called by an 
   iterator function in the above functions */

static void privateEdgeSelect(BYTE *b) {
  edge *e;
  e = (edge*)b;
  edgeBasicSelect(e);
}

static void privateEdgeUnselect(BYTE *b) {
  edge *e;
  e = (edge*)b;
  edgeBasicUnselect(e);
}

static void privateEdgeToggleSelection(BYTE *b) {
  edge *e;
  e = (edge*)b;
  if( e->selected==0 )
    edgeBasicSelect(e);
  else
    edgeBasicUnselect(e);
}

static void privateEdgeSelectionCount(BYTE *b) {
  edge *e;
  e = (edge*)b;
  if( e->selected )
    the_count++;
}

