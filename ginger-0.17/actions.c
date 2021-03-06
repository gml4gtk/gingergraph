/* $Id: actions.c,v 1.7 1996/03/28 16:04:23 lupus Exp $ */

/***************************************************************
 * This file is part of the Ginger project.                    *
 * Created 24-02-1996                                          *
 * Niels C. Larsen, Aalborg University, Denmark                *
 * (lupus@iesd.auc.dk)                                         *
 * Martin K. Molz, Aalborg University, Denmark                 *
 * (molz@iesd.auc.dk)                                          *
 ***************************************************************/

#include "mainWindow.h"
#include "nodeSelection.h"
#include "edgeSelection.h"
#include "nodeSelectionWindow.h"
#include "edgeSelectionWindow.h"
#include "hotspot.h"
#include "rectangles.h"
#include "dragRectangle.h"
#include "mathMacros.h"
#include "graph.h"
#include "graphBuffer.h"
#include "graphDisplay.h"
#include "elkUpdate.h"

/* ---  private declarations  --- */

static node *current_drag_node, *current_elk_drag_node;
static int  selection_x, selection_y;
static int  selection_dragging=0;
static int  dragging=0, elk_dragging=0;

/* ---  public functions  --- */

void actionsReset() {
  current_drag_node=0;
  selection_dragging=0;
}

void NodeDragStartAction(Widget w, XEvent *event, 
			 String *params, Cardinal *num_params)
{
  int x,y;
  
  x=event->xbutton.x;
  y=event->xbutton.y;
  
  current_drag_node=nodeAtPoint(graphBufferGetCurrentGraph(), x, y); /* reset the "current node" */
}

void NodeDragAction(Widget w, XEvent *event, 
		    String *params, Cardinal *num_params)
{
  if( current_drag_node )
    if( current_drag_node->selected )
      moveSelectedNodes(graphBufferGetCurrentGraph(), current_drag_node, event->xbutton.x, event->xbutton.y); 
}

void NodeDragEndAction(Widget w, XEvent *event, 
		       String *params, Cardinal *num_params)
{
  int wid, hei;
  graph *g;

  g=graphBufferGetCurrentGraph();

  /* when dragging is ended, the graph may have changed 
     size due to moved nodes. We recalculate the size and 
     let the main window adjust if necessary */
  if(g) {
    /* determine new dimensions */
    graphMaxCoordinates(graphBufferGetCurrentGraph(), &wid, &hei); 
    /* adjust the graph area size */
    mainWindowSetGraphAreaSize(wid+GRAPH_MARGIN,hei+GRAPH_MARGIN);
    
    /* notify the extension language about moved nodes */
#if USE_SCHEME
    elkSelectedNodesMoved(g);
#endif
  }
}

void HotspotSetAction(Widget w, XEvent *event, 
		      String *params, Cardinal *num_params)
{
  int x,y;

  x=event->xbutton.x;
  y=event->xbutton.y;
  hotspotSet(x,y);
}

void SelectionStartAction(Widget w, XEvent *event, 
			  String *params, Cardinal *num_params)
{
  selection_x=event->xbutton.x;
  selection_y=event->xbutton.y;
  
  dragRectangleSetFixedPoint(selection_x, selection_y);
}

void SelectionDragAction(Widget w, XEvent *event, 
			 String *params, Cardinal *num_params)
{
  int x,y;
  
  x=event->xbutton.x;
  y=event->xbutton.y;
  
  selection_dragging=1;
  
  dragRectangleSetPoint(x,y);
}

/* this function ought to me moved somewhere else! */
void selectObjectsInRectangle(graph *g, int x1, int y1, int x2, int y2, int accu)
{
  int i, first, state;
  node *n;
  edge *e;
  XPoint rect1, rect2, rect3, rect4;
  
  first=1;
  
  rect3.x=MIN(x1,x2);
  rect3.y=MIN(y1,y2);
  rect4.x=MAX(x1,x2);
  rect4.y=MAX(y1,y2);

  state=nodeSelectionWindowGetAccumulationState();

  for(i=1; i<=getsize(g->nodes); i++) {
    n=(node *)retrieve(i,g->nodes);
    rect1.x = n->x-n->width/2-NODE_MARGIN;
    rect1.y = n->y-n->height/2-NODE_MARGIN;
    rect2.x = n->x+n->width/2+NODE_MARGIN;
    rect2.y = n->y+n->height/2+NODE_MARGIN;
    
    if(rectangleOverlap(rect1,rect2,rect3,rect4))
      if(first && !accu ) {
	first=0;
	nodeSelect(g,n,state);
	/* notify the extension language of the change */
#if USE_SCHEME
	elkNodeSelect(n, n->selected);
#endif
      } else {
	nodeSelect(g,n,ACCUMULATING_MODE);
	/* notify the extension language of the change */
#if USE_SCHEME
	elkNodeSelect(n, n->selected);
#endif
      }
  }

  first=1;
  
  for(i=1; i<=getsize(g->edges); i++) {
    e=(edge *)retrieve(i,g->edges);
    
    if( pointInRectangle(e->p1,rect3,rect4) || pointInRectangle(e->p2,rect3,rect4) )
      if(first && !accu) {
	first=0;
	edgeSelect(g,e,edgeSelectionWindowGetAccumulationState());
	/* notify the extension language of the change */
#if USE_SCHEME
	elkEdgeSelect(e, e->selected);
#endif
      } 
      else {
	edgeSelect(g,e,ACCUMULATING_MODE);
	/* notify the extension language of the change */
#if USE_SCHEME
	elkEdgeSelect(e, e->selected);
#endif
      }
  }
}

void SelectionEndAction(Widget w, XEvent *event, 
			String *params, Cardinal *num_params)
{
  int x,y, state;
  edge *e;
  node *n;
  graph *g;
  
  x=event->xbutton.x;
  y=event->xbutton.y;

  g=graphBufferGetCurrentGraph();
  
  if( !selection_dragging ) {
    n=nodeAtPoint(g, x, y);
    e=edgeAtPoint(g, x, y);
    
    if(n) {
      state=nodeSelectionWindowGetAccumulationState();
      
      nodeToggleSelection(g, n, state);
      /* notify the extension language of the change */
#if USE_SCHEME
      elkNodeSelect(n, n->selected);
#endif
    }
    else if(e) {
      state=edgeSelectionWindowGetAccumulationState();

      edgeToggleSelection(g, e, state);
      /* notify the extension language of the change */
#if USE_SCHEME
      elkEdgeSelect(e, e->selected);
#endif
    }
    else {
      /* notify the extension language of the change */
#if USE_SCHEME
      elkUnselectAllObjects(g);
#endif
      nodeUnselectAll(g);
      edgeUnselectAll(g);
    }
  }
  else {
    dragRectangleUndraw();
    selectObjectsInRectangle(g,selection_x,selection_y,x,y,0);
  }
  
  selection_dragging=0;
}

void SelectionAddEndAction(Widget w, XEvent *event, 
			   String *params, Cardinal *num_params)
{
  int x,y;
  edge *e;
  node *n;
  graph *g;
  
  x=event->xbutton.x;
  y=event->xbutton.y;

  g=graphBufferGetCurrentGraph();
  
  if( !selection_dragging ) {
    n=nodeAtPoint(g, x, y);
    e=edgeAtPoint(g, x, y);
    
    if(n) {
      nodeToggleSelection(g, n, ACCUMULATING_MODE);
      /* notify the extension language of the change */
#if USE_SCHEME
      elkNodeSelect(n, n->selected);
#endif
    }
    else if(e) {
      edgeToggleSelection(g, e, ACCUMULATING_MODE);
      /* notify the extension language of the change */
#if USE_SCHEME
      elkEdgeSelect(e, e->selected);
#endif
    }
  }
  else {
    dragRectangleUndraw();
    selectObjectsInRectangle(g,selection_x,selection_y,x,y,1);
  }
  
  selection_dragging=0;
}

void NodeSelectionDragStartAction(Widget w, XEvent *event, 
			 String *params, Cardinal *num_params)
{
  int x,y;
  
  x=event->xbutton.x;
  y=event->xbutton.y;

  dragging=0;
  
  current_drag_node=nodeAtPoint(graphBufferGetCurrentGraph(), x, y); /* reset the "current node" */
  
  if( !current_drag_node ) { /* if the drag is initiated outside a node */
    SelectionStartAction(w,event,params,num_params);
  }
}

void NodeSelectionDragAction(Widget w, XEvent *event, 
			     String *params, Cardinal *num_params)
{
  if( current_drag_node ) {
    dragging=1;
    NodeDragAction(w,event,params,num_params);
  }
  else
    SelectionDragAction(w,event,params,num_params);
}

void NodeSelectionDragEndAction(Widget w, XEvent *event, 
		       String *params, Cardinal *num_params)
{
  if(dragging)
    NodeDragEndAction(w,event,params,num_params);
  else
    SelectionEndAction(w,event,params,num_params);
  dragging=0;
}

void NodeSelectionDragAddEndAction(Widget w, XEvent *event, 
		       String *params, Cardinal *num_params)
{
  if(dragging)
    NodeDragEndAction(w,event,params,num_params);
  else
    SelectionAddEndAction(w,event,params,num_params);
  dragging=0;
}

void ElkNodeDragStartAction(Widget w, XEvent *event, 
			    String *params, Cardinal *num_params)
{
  int x,y;
  
  x=event->xbutton.x;
  y=event->xbutton.y;
  
  current_elk_drag_node=nodeAtPoint(graphBufferGetCurrentGraph(), x, y); /* reset the "current node" */
}

void ElkNodeDragAction(Widget w, XEvent *event, 
		       String *params, Cardinal *num_params)
{
  int x,y;
  
  x=event->xbutton.x;
  y=event->xbutton.y;
#if USE_SCHEME
  if( current_elk_drag_node ) {
    elkNodeMove(current_elk_drag_node, x, y);
    elk_dragging=1;
  }
#endif
}

void ElkClickAction(Widget w, XEvent *event, 
		    String *params, Cardinal *num_params)
{
  int x,y;
  edge *e;
  node *n;
  graph *g;
  
  x=event->xbutton.x;
  y=event->xbutton.y;
  
  if( elk_dragging ) {
    elk_dragging=0;
    return;
  }
  
  g=graphBufferGetCurrentGraph();
  
  n=nodeAtPoint(g, x, y);
  e=edgeAtPoint(g, x, y);
  
#if USE_SCHEME
  if(n) {
    elkNodeClick(n);
  }
  else if(e) {
    elkEdgeClick(e);
  }
#endif
}

