/* $Id: node.c,v 1.20 1996/04/09 07:11:31 lupus Exp $ */

/***************************************************************
 * This file is part of the Ginger project.                    *
 * Created 28-12-1995                                          *
 * Niels C. Larsen, Aalborg University, Denmark                *
 * (lupus@iesd.auc.dk)                                         *
 ***************************************************************/

/* Functions to create, destroy, draw, and highlight nodes 
   and to generate GGL repr. of nodes */

#include <string.h>
#include "colormap.h"
#include "str.h"
#include "node.h"
#include "edge.h"

/* ---  external symbols (from mainWindow.c)  --- */

extern Display *the_display;
extern Drawable the_drawable;
extern GC clear_gc, node_gc, label_gc;
extern GC highlight_gc, unhighlight_gc;
extern XFontStruct *label_font_struct;

/* ---  private declarations  --- */

#define DEFAULT_X 100
#define DEFAULT_Y 100

static void readNodeBitmapIcon(node *n, char *iconPath);
static void readNodePixmapIcon(node *n, char *iconPath);
static int  labelWidth(char *label);
static void labelCoords(node *n);

/* ---  public functions  --- */

node *createNode(char *name, char *label, int shape, 
		 int x, int y, int width, int height,
		 int iconType, char *iconPath)
{
  node *result;
  
  result = GingerNew(node);
  result->name = GingerNewString(name);
  result->label = GingerNewString(label);
  result->icon.path = GingerNewString(iconPath);
  result->icon.type = iconType;
  result->shape = shape;
  result->x = (x<0) ? DEFAULT_X:x;
  result->y = (y<0) ? DEFAULT_Y:y;
  result->width = (width==0) ? DEFAULT_NODE_WIDTH:width;
  result->height= (height==0) ? DEFAULT_NODE_HEIGHT:height;
  result->selected = 0 ;
#if USE_SCHEME
  result->object = 0;
#endif

  /* read icon from file, if node has one */
  switch(iconType) {
  case ICON_BITMAP:
    readNodeBitmapIcon(result, iconPath);
    break;
  case ICON_PIXMAP:
    readNodePixmapIcon(result, iconPath);
    break;
  }

  /* set no contents and editor as default */
  result->contents=0; 
  result->editor=0;

  /* set colors to default, i.e. don't use the node's private 
     color information */
  result->foreground.is_used = 0;
  result->background.is_used = 0;
  
  /* increase node width to hold the label, if necessary */
  if( strlen(result->label)>0 )
    result->width = MAX(result->width, labelWidth(result->label)+2*NODE_MARGIN);
  
  result->image = XCreatePixmap(the_display, the_drawable, 
				result->width+2*NODE_MARGIN, 
				result->height+2*NODE_MARGIN,
				XDisplayPlanes(the_display, 0) );
  /* makeNodeImage(result); */

  return result;
}

/* free the contents of node n */
void destroyNode(node *n) {
  /* printf("in destroy node, %s\n", n->label); */
  GingerFree(n->name);
  GingerFree(n->label);
  GingerFree(n->icon.path);
  if( n->contents )
    GingerFree(n->contents);
  if( n->editor )
    GingerFree(n->editor);

  XFreePixmap(the_display, n->image); /* free node pixmap */

  /* free icon pixmap */
  if( n->icon.type==ICON_BITMAP || n->icon.type==ICON_PIXMAP )
    XFreePixmap(the_display, n->icon.image);
  
  /* free allocated colors in the color icon, if applicable */
#if USE_XPM_LIB
  if( n->icon.type==ICON_PIXMAP ) {
    XFreeColors(the_display, n->icon.attr.colormap,
		n->icon.attr.pixels,
		n->icon.attr.npixels, 0);
  }
#endif
  /* free allocated fore-/background colors */
  if( n->foreground.is_used )
    colormapDeallocateColorGC(n->foreground.color_name);
  if( n->background.is_used )
    colormapDeallocateColorGC(n->background.color_name);
}

/* free the node itself */
void freeNode(node *n) {
  GingerFree((char *)n);
}

void nodeSetContents(node *n, char *cont) {
  if( n->contents )
    GingerFree(n->contents);
  n->contents=GingerNewString(cont);
}

void nodeSetEditor(node *n, char *edit) {
  if( n->editor )
    GingerFree(n->editor);
  n->editor=GingerNewString(edit);
}

void nodeRunEditorOnContents(node *n) {
  char buf[1024];   /* buffer for command line */
  int l, k;
  
  if( !n->editor )  /* if node doesn't have an editor, we can't edit it */
    return;
  
  if( (l=strlen(n->editor))==0 ) /* if node doesn't have an editor with
				    non-zero command name, 
				    we can't edit it */
    return;

  /* copy editor command to buffer */
  zeroAddStrncpy(buf, n->editor, 1024-10);
  
  /* if node has contents, strcat them to buf */
  if( n->contents )
    if( (k=strlen(n->contents))>0 )
      if( l+k<1024-10 ) {
	strcat(buf, " ");
	strcat(buf, n->contents);
      }
  
  /* add " &" to the command line so we don't block to wait for the
     child process */
  strcat(buf, " &");
  
  printf("Running editor on node \"%s\" (%s) ...\n", n->label, buf);
  
  /* fork a child process using the command line in buf */
  system(buf);
}

void nodeSetForeground(node *n, char *colorname) {
  if(strlen(colorname)>0) {
    zeroAddStrncpy(n->foreground.color_name, colorname, MAX_COLOR_NAME_LENGTH);
    n->foreground.gc = colormapAllocateColorGC(colorname);
    n->foreground.is_used=1;
  }
}

void nodeSetBackground(node *n, char *colorname) {
  if(strlen(colorname)>0) {
    zeroAddStrncpy(n->background.color_name, colorname, MAX_COLOR_NAME_LENGTH);
    n->background.gc = colormapAllocateColorGC(colorname);
    n->background.is_used=1;
  }
}

void nodeSetName(node *n, char *name) {
  GingerFree(n->name);
  n->name = GingerNewString(name);
}

void nodeSetLabel(node *n, char *label) {
  GingerFree(n->label);
  n->label = GingerNewString(label);
  
  /* increase node width to hold the label, if necessary */
  n->width = MAX(n->width, labelWidth(n->label)+2*NODE_MARGIN);
}

void nodeSetIcon(node *n, int icon_type, char *icon_file) {
  GingerFree(n->icon.path);
  n->icon.path = GingerNewString(icon_file);
  n->icon.type = icon_type;
  
  /* read icon from file, if node has one */
  switch(icon_type) {
  case ICON_BITMAP:
    readNodeBitmapIcon(n, icon_file);
    break;
  case ICON_PIXMAP:
    readNodePixmapIcon(n, icon_file);
    break;
  }
}

/* ---  drawing functions  --- */

/* clear the node's private pixmap */
void clearNodeImage(node *n) {
  XFillRectangle(the_display, n->image, clear_gc, 0,0, 
		 n->width+2*NODE_MARGIN, 
		 n->height+2*NODE_MARGIN);
}

/* re-allocate node image, typically after a change of node size */
void nodeReallocImage(node *n) {
  
  XFreePixmap(the_display, n->image); /* free node pixmap */
  
  n->image = XCreatePixmap(the_display, the_drawable, 
			   n->width+2*NODE_MARGIN, 
			   n->height+2*NODE_MARGIN,
			   XDisplayPlanes(the_display, 0) );
}

/* draw an image og the node to its private pixmap */
void makeNodeImage(node *n) {
  int l, x, y;
  GC  foreground_gc;
  XPoint points[5];  /* points for rhombe-shape */

  /* draw an image of the node to the node's own pixmap, 
     based on the type of node etc. */
  
  /* used supplied foreground color, if given. Otherwise use
     default node color */
  if( n->foreground.is_used )
    foreground_gc=n->foreground.gc;
  else
    foreground_gc=node_gc;

  /* first clear the pixmap */
  clearNodeImage(n);

  /* fill the node background, if a background color is supplied */
  if( n->background.is_used ) {
    switch(n->shape) {
    case SHAPE_NONE:
    case SHAPE_RECTANGLE:
      XFillRectangle(the_display, n->image, n->background.gc, 
		     NODE_MARGIN, NODE_MARGIN, 
		     n->width, 
		     n->height);
      break;
    case SHAPE_RHOMBE:
      points[0].x = NODE_MARGIN;
      points[0].y = n->height/2+NODE_MARGIN;
      points[1].x = n->width/2+NODE_MARGIN;
      points[1].y = NODE_MARGIN;
      points[2].x = n->width+NODE_MARGIN;
      points[2].y = n->height/2+NODE_MARGIN;
      points[3].x = n->width/2+NODE_MARGIN;
      points[3].y = n->height+NODE_MARGIN;
      XFillPolygon(the_display, n->image, n->background.gc,
		   points, 4, Convex, CoordModeOrigin );
      break;
    case SHAPE_ELLIPSE:
      XFillArc(the_display, n->image, n->background.gc,
	       NODE_MARGIN, NODE_MARGIN,
	       n->width, n->height,
	       0,360*64);
      break;
    }
  }
  
  /* add icon, if node has one */
  switch(n->icon.type) {
  case ICON_BITMAP:
    XCopyPlane(the_display, n->icon.image, n->image, 
	       foreground_gc, 0, 0, 
	       n->icon.width,
	       n->icon.height,
	       NODE_MARGIN+1,
	       NODE_MARGIN+1, 1);
    break;
  case ICON_PIXMAP:
    XCopyArea(the_display, n->icon.image, n->image, 
	      foreground_gc, 0, 0, 
	      n->icon.width,
	      n->icon.height,
	      NODE_MARGIN+1,
	      NODE_MARGIN+1);
    break;
  }

  /* draw the outline of the node, if it has a shape */
  switch(n->shape) {
  case SHAPE_NONE:
    break;
  case SHAPE_RECTANGLE:
    XDrawRectangle(the_display, n->image, foreground_gc, 
		   NODE_MARGIN, NODE_MARGIN, 
		   n->width, n->height);
    break;
  case SHAPE_RHOMBE:
    points[0].x = NODE_MARGIN;
    points[0].y = n->height/2+NODE_MARGIN;
    points[1].x = n->width/2+NODE_MARGIN;
    points[1].y = NODE_MARGIN;
    points[2].x = n->width+NODE_MARGIN;
    points[2].y = n->height/2+NODE_MARGIN;
    points[3].x = n->width/2+NODE_MARGIN;
    points[3].y = n->height+NODE_MARGIN;
    points[4].x = points[0].x;
    points[4].y = points[0].y;
    XDrawLines(the_display, n->image, foreground_gc,
	       points, 5, CoordModeOrigin );
    break;
  case SHAPE_ELLIPSE:
    XDrawArc(the_display, n->image, foreground_gc,
	     NODE_MARGIN, NODE_MARGIN,
	     n->width, n->height,
	     0,360*64);
    break;
  }

  
  /* add label, if node has one */
  if( (l=strlen(n->label)) > 0 ) {
    /* determine coordinates for the label 
       note that coords must be relative to
       the node's pixmap */
    labelCoords(n);
    
    XDrawString(the_display, n->image, label_gc, n->label_x, n->label_y, 
		n->label, l);
  }

  /* if the node is selected, highlight it */
  if( n->selected )
    nodeHighlight(n,1);
}

/* make the node's image "highlighted", if highlight_on is true.
   Otherwise "unhighlight" the node.
   This function affects only the node's private image pixmap. */
void nodeHighlight(node *n, int highlight_on) {
  if( highlight_on ) {
    XDrawRectangle(the_display, n->image, highlight_gc, 
		   NODE_HIGHLIGHT_WIDTH/2, NODE_HIGHLIGHT_WIDTH/2, 
		   n->width+2*NODE_MARGIN-NODE_HIGHLIGHT_WIDTH,
		   n->height+2*NODE_MARGIN-NODE_HIGHLIGHT_WIDTH);
    /* "trim" to create shadow-effect */
    XDrawRectangle(the_display, n->image, clear_gc, 
		   0, 0, 
		   n->width+2*NODE_MARGIN, n->height+2*NODE_MARGIN);
  } 
  else {
    XDrawRectangle(the_display, n->image, unhighlight_gc, 
		   NODE_HIGHLIGHT_WIDTH/2, NODE_HIGHLIGHT_WIDTH/2, 
		   n->width+2*NODE_MARGIN-NODE_HIGHLIGHT_WIDTH,
		   n->height+2*NODE_MARGIN-NODE_HIGHLIGHT_WIDTH);
  }
}

/* copy the node's private pixmap to the graph window */
void displayNodeImageOnScreen(node *n) {
  XCopyArea(the_display, n->image, the_drawable,
	    node_gc, 0, 0, 
	    n->width+2*NODE_MARGIN,
	    n->height+2*NODE_MARGIN,
	    n->x-n->width/2-NODE_MARGIN,
	    n->y-n->height/2-NODE_MARGIN );
}

/* clear the area occupied by the node in the graph window */
void undisplayNodeImageOnScreen(node *n) {
  XFillRectangle(the_display, the_drawable, clear_gc, 
		 n->x-n->width/2-NODE_MARGIN, 
		 n->y-n->height/2-NODE_MARGIN, 
		 n->width+2*NODE_MARGIN, 
		 n->height+2*NODE_MARGIN);
}

/* ---  generate GGL representation of a node  --- */

void printNode(node *n, FILE *fd) {
  /* name */
  fprintf(fd, "(node %s\n", n->name);

  /* label */
  if( strlen(n->label) > 0 )
    fprintf(fd, "  label \"%s\"\n", n->label);

  /* color */
  if( n->foreground.is_used )
    fprintf(fd, "  color \"%s\"\n", n->foreground.color_name);
  if( n->background.is_used )
    fprintf(fd, "  bg_color \"%s\"\n", n->background.color_name);
  
  /* shape */
  if( n->shape != SHAPE_DEFAULT ) {
    switch(n->shape) {
    case SHAPE_NONE:
      fprintf(fd, "  shape none\n");
      break;
    case SHAPE_RECTANGLE:
      fprintf(fd, "  shape rectangle\n");
      break;
    case SHAPE_RHOMBE:
      fprintf(fd, "  shape rhomb\n");
      break;
    case SHAPE_ELLIPSE:
      fprintf(fd, "  shape ellipse\n");
      break;
    }
  }
  
  /* icon */
  switch(n->icon.type) {
  case ICON_BITMAP:
    fprintf(fd, "  bitmap \"%s\"\n", n->icon.path);
    break;
  case ICON_PIXMAP:
    fprintf(fd, "  pixmap \"%s\"\n", n->icon.path);
    break;
  }
  
  /* x,y position */
  fprintf(fd, "  x %d  y %d\n", n->x, n->y);
  
  /* width and height */
  fprintf(fd, "  width %d  height %d\n", n->width, n->height);
  
  /* editor */
  if( n->editor )
    if( strlen(n->editor)>0 )
      fprintf(fd, "  editor \"%s\"\n", n->editor);
  
  /* contents */
  if( n->contents )
    if( strlen(n->contents)>0 )
      fprintf(fd, "  contents \"%s\"\n", n->contents);
  
  fprintf(fd, ")\n");
}

/* ---  private functions  --- */

/* read bitmap from file */
static void readNodeBitmapIcon(node *n, char *iconPath) {
  int xh,yh,res;
  int label_add_height;
  
  label_add_height=0;
  if( strlen(n->label) > 0 )
    label_add_height = DEFAULT_LABEL_HEIGHT;
  
  res=XReadBitmapFile(the_display, the_drawable, iconPath,
		      (unsigned int *)&(n->icon.width),(unsigned int *) &(n->icon.height), 
		      &(n->icon.image), 
		      &xh, &yh);
  if( res==BitmapSuccess ) {   /* adjust node size */
    n->height = MAX(n->height,  
		    n->icon.height+label_add_height+2);
    n->width  = MAX(n->width,  
		    n->icon.width+1);
  }
  else
    n->icon.type = ICON_NONE;
}

/* read pixmap from file; requires the Xpm library */
static void readNodePixmapIcon(node *n, char *iconPath) {
#if !USE_XPM_LIB           /* if we don't have access to Xpm */
  n->icon.type=ICON_NONE;  /* the icon can't be loaded */
  return;
#else
  int xh,yh,res;
  int label_add_height;

  label_add_height=0;
  if( strlen(n->label) > 0 )
    label_add_height = DEFAULT_LABEL_HEIGHT;

  n->icon.attr.colormap  = XDefaultColormap(the_display,0);
  n->icon.attr.closeness = 65000; /* very "sloppy" colors allowed */
  n->icon.attr.valuemask = XpmReturnPixels | XpmColormap 
                            | XpmCloseness | XpmReturnColorTable;
  res=XpmReadFileToPixmap(the_display, the_drawable, iconPath,
			  &(n->icon.image), 0, 
			  &(n->icon.attr) );

  /* printf("%d\n", res); */
  if( res==XpmSuccess ) {   /* adjust node size */
    n->icon.height = n->icon.attr.height;
    n->icon.width  = n->icon.attr.width;
    
    n->height = MAX(n->height,  
		    n->icon.height+label_add_height+2);
    n->width  = MAX(n->width,  
		    n->icon.width+1);
  }
  else
    n->icon.type = ICON_NONE;
#endif
}

/* determine width of the label in pixels, in the current label font */
static int labelWidth(char *label) {
  int l, width;
  
  l=strlen(label);
  
  if( l==0 )
    width = 0;
  else
    width = XTextWidth(label_font_struct, label, l);

  return width;
}

/* determine appropriate coordinates for the label.
   The label is centered if the node is an ellipse or a rhombe,
   otherwise the label is placed in the lower left corner. */
static void labelCoords(node *n) {
  int w;
  
  w=labelWidth(n->label);
  switch( n->shape ) {
  case SHAPE_ELLIPSE: /* center in horizontal and vertical direction */
  case SHAPE_RHOMBE:
    n->label_x=n->width/2-w/2+NODE_MARGIN;       
    n->label_y=n->height/2+DEFAULT_LABEL_HEIGHT/2; 
    break;
  case SHAPE_RECTANGLE: /* place label in lower left corner */
  case SHAPE_NONE:
    n->label_x=2*NODE_MARGIN;
    n->label_y=n->height-LABEL_HEIGHT_FROM_BOTTOM;
    break;
  }
}
