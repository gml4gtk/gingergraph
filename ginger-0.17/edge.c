/* $Id: edge.c,v 1.14 1996/04/19 07:06:42 lupus Exp $ */

/***************************************************************
 * This file is part of the Ginger project.                    *
 * Created 28-12-1995                                          *
 * Niels C. Larsen, Aalborg University, Denmark                *
 * (lupus@iesd.auc.dk)                                         *
 ***************************************************************/

/* This file contains code to create, destroy, and draw edges  */

/***************************************************************
 * Parts of the code in this file is based on the Xhdg, a      *
 * Graph Widget for layout of hierarchical directed Graphs,    *
 * Copyright 1994 University of Stuttgart		       *
 *							       *
 * The copyright notices below state the permissions given     *
 * by the original authors.				       *
 ***************************************************************/

/* Copyright 1994 University of Stuttgart
 * This Graph Widget layouts (hierarchical) directed Graphs
 * This Work is based on the X11R5 tree widget
*/

/*
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use, copy,
 * modify, merge, publish, distribute, sublicense, and/or sell copies
 * of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:

 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.

 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT.  IN NO EVENT SHALL THE UNIVERSITY OF STUTTGART OR
 * THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.

 * Except as contained in this notice, the name of the University of
 * Stuttgart or the names of the authors shall not be used in
 * advertising or otherwise to promote the sale, use or other dealings
 * in this Software without prior written authorization from the
 * University of Stuttgart and the authors.
*/

/*
 * $XConsortium: Tree.c,v 1.42 91/02/20 20:06:07 converse Exp $
 *
 * Copyright 1990 Massachusetts Institute of Technology
 * Copyright 1989 Prentice Hall
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose and without fee is hereby granted, provided that the above
 * copyright notice appear in all copies and that both the copyright notice
 * and this permission notice appear in supporting documentation.
 * 
 * M.I.T., Prentice Hall and the authors disclaim all warranties with regard
 * to this software, including all implied warranties of merchantability and
 * fitness.  In no event shall M.I.T., Prentice Hall or the authors be liable
 * for any special, indirect or cosequential damages or any damages whatsoever
 * resulting from loss of use, data or profits, whether in an action of
 * contract, negligence or other tortious action, arising out of or in
 * connection with the use or performance of this software.
 * 
 * Authors:  Jim Fulton, MIT X Consortium,
 *           based on a version by Douglas Young, Prentice Hall
 * 
 * This widget is based on the Tree widget described on pages 397-419 of
 * Douglas Young's book "The X Window System, Programming and Applications 
 * with Xt OSF/Motif Edition."  The layout code has been rewritten to use
 * additional blank space to make the structure of the graph easier to see
 * as well as to support vertical trees.
 */

/* Author of Xhdg: 
           Roland Zink
           Universitaet Stuttgart
           IPVR
           Breitwiesenstrasse 20-22
           D 70565 Stuttgart (Germany)
           EMail Roland.Zink@informatik.uni-stuttgart.de
*/

#include <stdio.h>
#include <string.h>
#include <math.h>

#include "edge.h"
#include "node.h"
#include "mathMacros.h"

#include "colormap.h"

/* ---  external symbols (from mainWindow.c)  --- */

extern Display *the_display;
extern Drawable the_drawable;
extern GC the_gc, clear_gc, edge_gc, highlight_gc, unhighlight_gc;

/* ---  private function prototypes  --- */

static XPoint get_boundary_offset(node *n, XPoint to, XPoint from, int check_postscript);
static XPoint get_boundary_point(node *n, XPoint from, int check_postscript);
static void DrawArrowToEdge(XPoint from, XPoint to, GC gc);

/* ---  public functions  --- */

/* create an edge object */
edge *createEdge(char *name, node *from, node *to) {
  edge *result;

  result=GingerNew(edge);
  result->to=to;
  result->from=from;
  result->name=GingerNewString(name);
  result->direction = DIR_NORMAL;  /* default value */
  result->foreground.is_used = 0;  /* use default edge color */
  result->selected = 0;
  result->highlighted=0;

  calculateEdgeEndpoints(result);

  return result;
}

/* destroy the edge object */
void destroyEdge(edge *e) {
  GingerFree(e->name);
  if( e->foreground.is_used )
    colormapDeallocateColorGC(e->foreground.color_name);
}

/* free the edge object */
void freeEdge(edge *e) {
  GingerFree((char *)e);
}

/* calculate coordinates for the endpoints of the edge object;
   assume nodes are rectangular in shape */
void calculateEdgeEndpoints(edge *e) {
  XPoint node_center;
  
  node_center.x=e->to->x;
  node_center.y=e->to->y;
  e->p1=get_boundary_point(e->from, node_center, 0);

  node_center.x=e->from->x;
  node_center.y=e->from->y;
  e->p2=get_boundary_point(e->to, node_center, 0);
}

/* calculate coordinates for the endpoints of the edge object */
void calculateEdgeEndpointsElliptical(edge *e) {
  XPoint node_center;
  
  node_center.x=e->to->x;
  node_center.y=e->to->y;
  e->p1=get_boundary_point(e->from, node_center, 1);

  node_center.x=e->from->x;
  node_center.y=e->from->y;
  e->p2=get_boundary_point(e->to, node_center, 1);
}

int edgeDistanceFromPoint(edge *e, int x, int y) {
  int a,b,c;
  int delta_x, delta_y;
  int res;
  
  /* if edge has zero length */
  if( e->to->x==e->from->x && e->to->y==e->from->y ) {
    return DIST(e->to->x, e->to->y, x, y);
  }
  
  /* determine equation of the edge line */
  
  /* first find its direction vector */
  delta_x=e->to->x-e->from->x;
  delta_y=e->to->y-e->from->y;
  
  /* create normal vector = parameters in the line equation ax+by+c=0 */
  a=-delta_y;
  b=delta_x;
  
  /* find parameter c */
  c=-a*e->from->x-b*e->from->y;
  
  /* calculate distance from point to line */
  res=ABSOLUTE(a*x+b*y+c)/sqrt(a*a+b*b);

  return res;
}

void edgeSetColor(edge *e, char *colorname) {
  if(strlen(colorname)>0) {
    zeroAddStrncpy(e->foreground.color_name, colorname, MAX_COLOR_NAME_LENGTH);
    e->foreground.gc = colormapAllocateColorGC(colorname);
    e->foreground.is_used=1;
  }
}

/* ---  functions to draw the edge on the display  --- */

void drawEdgeWithGC(edge *e, GC gc) {
  XDrawLine(the_display, the_drawable, gc, 
	    e->p1.x, e->p1.y,
	    e->p2.x, e->p2.y);
  switch( e->direction ) {
  case DIR_NORMAL:
    DrawArrowToEdge(e->p1, e->p2, gc);
    break;
  case DIR_INVERSE:
    DrawArrowToEdge(e->p2, e->p1, gc);
    break;
  case DIR_BIDIRECTIONAL:
    DrawArrowToEdge(e->p1, e->p2, gc);
    DrawArrowToEdge(e->p2, e->p1, gc);
    break;
  case DIR_NONE:
    break;
  }
}

void drawEdge(edge *e) {
  if( e->highlighted )
    drawEdgeWithGC(e, highlight_gc);
  else
    if( e->foreground.is_used )
      drawEdgeWithGC(e, e->foreground.gc);
    else
      drawEdgeWithGC(e, edge_gc);
}

void undrawEdge(edge *e) {
  if( e->highlighted )
    drawEdgeWithGC(e, unhighlight_gc);
  else
    drawEdgeWithGC(e, clear_gc);
}

/* highlight the edge, if highlight_on is true.
   Otherwise "unhighlight" the edge. */
void edgeHighlight(edge *e, int highlight_on) {

  if( highlight_on!=e->highlighted ) {  /* redraw only if the 
					   state is changed */
    undrawEdge(e);
    e->highlighted = highlight_on;
    drawEdge(e);
  }
}

/* print GGL-representation of edge */
void printEdge(edge *e, FILE *fd) {
  /* name */
  fprintf(fd, "(edge %s\n", e->name);
  
  /* from and to nodes */
  fprintf(fd, "  from %s", e->from->name);
  fprintf(fd, " to %s\n", e->to->name);
  
  /* color */
  if( e->foreground.is_used )
    fprintf(fd, "  color \"%s\"\n", e->foreground.color_name);
  
  /* direction */
  if( e->direction != DIR_NORMAL ) {
    fprintf(fd, "  direction ");
    switch(e->direction) {
    case DIR_NONE:
      fprintf(fd, "none\n");
      break;
    case DIR_INVERSE:
      fprintf(fd, "inverse\n");
      break;
    case DIR_BIDIRECTIONAL:
      fprintf(fd, "both\n");
      break;
    }
  }

  fprintf(fd, ")\n");
}

/* ---  private functions  --- */

static XPoint get_boundary_offset(node *n, XPoint to, XPoint from, 
				  int check_postscript) { 
  double angle, r,a,b, sina, cosa;
  double rangle;
  XPoint bp;
  
  if( check_postscript ) {       /* for the PostScript code, we calculate
				    better fitting endpoints */
    if( to.y == from.y )
      { bp.x = n->width/2;
	bp.y = 0;
	return bp;
      }
    if( to.x == from.x )
      { bp.x = 0;
	bp.y = n->height/2;
	return bp;
      }

    /* if node is an ellipse */
    if( n->shape==SHAPE_ELLIPSE ) {
      angle = atan(((double)(to.y - from.y))/((double)(to.x - from.x)));
      a=(double)n->width/2.0;
      b=(double)n->height/2.0;

      if( a<0.1 ) /* make sure we don't divide by zero */
	a=0.1;
      if( b<0.1 )
	b=0.1;

      if( a==b ) /* if circle, radius is simple */
	r=a;
      else {     /* if ellipse, radius is not-so-simple */
	sina=sin(angle);
	cosa=cos(angle);
	r = sqrt(a*a*b*b/(a*a*sina*sina+b*b*cosa*cosa));
      }
      r+=0.5; /* add a margin */
      bp.x  = r*ABSOLUTE(cos(angle));
      bp.y  = r*ABSOLUTE(sin(angle));
      return bp;
    }
    else { /* pretend the node has rectangular shape, PostScript calculation */
      angle = ((double)(to.x - from.x))/((double)(to.y - from.y));
      if( n->height == 0 )
	{ rangle = 1000000; }
      else
	{ rangle = ((double)(n->width))/((double)(n->height)); }
      
      if( ABSOLUTE(angle) < rangle )
	{ bp.x = (short)(angle*(n->height/2));
	  bp.y = n->height/2;
	}
      else
	{ angle = ((double)(to.y - from.y))/((double)(to.x - from.x));
	  bp.x = n->width/2;
	  bp.y = (short)(angle*(n->width/2));
	}
      
      bp.x=ABSOLUTE(bp.x);
      bp.y=ABSOLUTE(bp.y);
      
      return bp;
    }
  }

  /* otherwise (i.e. node has a rectangular shape or we pretend it has),
   and it's not for PostScript use */

  if( to.y == from.y )
    { bp.x = n->width/2 + NODE_MARGIN;
      bp.y = 0;
      return bp;
    }
  if( to.x == from.x )
    { bp.x = 0;
      bp.y = n->height/2 + NODE_MARGIN;
      return bp;
    }

  angle = ((double)(to.x - from.x))/((double)(to.y - from.y));
  if( n->height == 0 )
    { rangle = 1000000; }
  else
    { rangle = ((double)(n->width))/((double)(n->height)); }
  
  if( ABSOLUTE(angle) < rangle )
    { bp.x = (short)(angle*(n->height/2 + NODE_MARGIN));
      bp.y = n->height/2 + NODE_MARGIN;
    }
  else
    { angle = ((double)(to.y - from.y))/((double)(to.x - from.x));
      bp.x = n->width/2 + NODE_MARGIN;
      bp.y = (short)(angle*(n->width/2 + NODE_MARGIN));
    }
  
  bp.x=ABSOLUTE(bp.x);
  bp.y=ABSOLUTE(bp.y);

  return bp;
}

static XPoint get_boundary_point(node *n, XPoint from, int check_postscript) { 
  XPoint offset, result;
  XPoint temp, node_center;
  
  node_center.x = n->x;
  node_center.y = n->y;
  
  if( from.y < node_center.y )
    { if( from.x > node_center.x )
        { 
          offset = get_boundary_offset(n,node_center,from,check_postscript);
          offset.y = -offset.y;
        }
    else
      { offset = get_boundary_offset(n,node_center,from,check_postscript);
	offset.y = -offset.y;
	offset.x = -offset.x;
      }
    }
  else
    { if( from.x <= node_center.x )
        { offset = get_boundary_offset(n,node_center,from,check_postscript);
          offset.x = -offset.x;
        }
    else
      { offset = get_boundary_offset(n,node_center,from,check_postscript);
      }
    }
  result.x = offset.x + n->x;
  result.y = offset.y + n->y;
  
  return result;
}

static void DrawArrowToEdge(XPoint from, XPoint to, GC gc)
{ 
  XPoint p3; 
  XPoint points[3]; 
  double diffx, diffy, length;
  
  diffx = to.x - from.x;
  diffy = to.y - from.y;
  length = sqrt(diffx*diffx + diffy*diffy);
  if( length <1.0 )
    length = 1.0;

  p3.x = to.x - (diffx*(double)ARROW_LENGTH)/length;
  p3.y = to.y - (diffy*(double)ARROW_LENGTH)/length;
  
  points[0].x = to.x;
  points[0].y = to.y;
  points[1].x = p3.x + (diffy*(double)ARROW_WIDTH)/length;
  points[1].y = p3.y - (diffx*(double)ARROW_WIDTH)/length;
  points[2].x = p3.x - (diffy*(double)ARROW_WIDTH)/length;
  points[2].y = p3.y + (diffx*(double)ARROW_WIDTH)/length;
  XFillPolygon(the_display, the_drawable,
               gc, points, 3, Convex, CoordModeOrigin );
}

