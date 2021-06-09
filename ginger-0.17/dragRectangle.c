/* $Id: dragRectangle.c,v 1.1 1996/02/26 08:51:43 lupus Exp $ */

/***************************************************************
 * This file is part of the Ginger project.                    *
 * Created 24-02-1996                                          *
 * Niels C. Larsen, Aalborg University, Denmark                *
 * (lupus@iesd.auc.dk)                                         *
 * Martin K. Molz, Aalborg University, Denmark                 *
 * (molz@iesd.auc.dk)                                          *
 ***************************************************************/

#include <X11/Intrinsic.h>
#include "graph.h"
#include "graphBuffer.h"
#include "graphDisplay.h"
#include "dragRectangle.h"
#include "mathMacros.h"

/* ---  external symbols (from mainWindow.c)  --- */

extern Display *the_display;
extern Drawable the_drawable;
extern GC clear_gc, node_gc;

/* ---  private declarations  --- */

static XPoint p1, p2;  /* p1 is the fixed point */

static void dragRectangleDrawWithGC(GC gc);

/* ---  public functions  --- */

void dragRectangleDraw() {
  dragRectangleDrawWithGC(node_gc);
}

void dragRectangleUndraw() {
  dragRectangleDrawWithGC(clear_gc);
  displayGraphWithinRectangle(graphBufferGetCurrentGraph(),p1,p2);
}

void dragRectangleInitialize() {
  dragRectangleSetFixedPoint(0,0);
}

void dragRectangleSetFixedPoint(int x, int y) {
  p1.x=x;
  p1.y=y;
  p2.x=x;
  p2.y=y;
}

void dragRectangleSetPoint(int x, int y) {
  dragRectangleUndraw();
  p2.x=x;
  p2.y=y;
  dragRectangleDraw();
}

/* ---  private functions  --- */

static void dragRectangleDrawWithGC(GC gc) {
  int x1,y1,x2,y2;
  
  x1=MIN(p1.x,p2.x);
  y1=MIN(p1.y,p2.y);
  x2=MAX(p1.x,p2.x);
  y2=MAX(p1.y,p2.y);
  
  XDrawRectangle(the_display, the_drawable, gc, x1, y1, x2-x1, y2-y1);
}

