/* $Id: hotspot.c,v 1.2 1996/03/04 08:50:05 lupus Exp $ */

/***************************************************************
 * This file is part of the Ginger project.                    *
 * Created 19-02-1996                                          *
 * Niels C. Larsen, Aalborg University, Denmark                *
 * (lupus@iesd.auc.dk)                                         *
 * Martin K. Molz, Aalborg University, Denmark                 *
 * (molz@iesd.auc.dk)                                          *
 ***************************************************************/

#include <X11/Xlib.h>
#include "nodeAttributeWindow.h"

#include "graphBuffer.h"
#include "graphDisplay.h"

#include "arrayLayoutParameterWindow.h"
#include "circularLayoutParameterWindow.h"
#include "treeLayoutParameterWindow.h"

/* ---  private declarations  --- */

#define HOTSPOT_WIDTH  10
#define HOTSPOT_HEIGHT 10

static int x=0,y=0;

extern Display *the_display;     /* from mainWindow.c */
extern Drawable the_drawable;
extern GC highlight_gc, unhighlight_gc;

/* private function prototypes */
static void hotspotDraw();
static void hotspotUndraw();
static void hotspotDrawWithGC(GC gc);

/* ---  public functions  --- */

/* this function is called whenever the hotspot changes */
static void hotspotNotify() {
  
  /* "observers" interested in this event */
  
  nodeAttributeWindowHotspotChange(x,y);
  arrayLayoutParameterWindowHotspotChange(x,y);
  treeLayoutParameterWindowHotspotChange(x,y);
  circularLayoutParameterWindowHotspotChange(x,y);
}

void hotspotSet(int new_x, int new_y) {
  hotspotUndraw();
  x=new_x;
  y=new_y;
  
  hotspotDraw();
  hotspotNotify();
}

void hotspotGet(int *return_x, int *return_y) {
  *return_x=x;
  *return_y=y;
}

/* ---  private functions  --- */

static void hotspotDrawWithGC(GC gc) {
  XDrawLine(the_display, the_drawable, gc, 
	    x-HOTSPOT_WIDTH/2, y,
	    x+HOTSPOT_WIDTH/2, y);
  XDrawLine(the_display, the_drawable, gc, 
	    x, y-HOTSPOT_HEIGHT/2,
	    x, y+HOTSPOT_HEIGHT/2);
}

static void hotspotDraw() {
  hotspotDrawWithGC(highlight_gc);
}

static void hotspotUndraw() {
  XPoint p1, p2;
  
  p1.x=x-HOTSPOT_WIDTH/2;
  p1.y=y-HOTSPOT_HEIGHT/2;
  p2.x=x+HOTSPOT_WIDTH/2;
  p2.y=y+HOTSPOT_HEIGHT/2;
  
  hotspotDrawWithGC(unhighlight_gc);
  displayGraphWithinRectangle(graphBufferGetCurrentGraph(), p1, p2);
}

