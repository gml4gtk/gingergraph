/* $Id: node.h,v 1.19 1996/03/13 15:43:21 lupus Exp $ */

/***************************************************************
 * This file is part of the Ginger project.                    *
 * Created 28-12-1995                                          *
 * Niels C. Larsen, Aalborg University, Denmark                *
 * (lupus@iesd.auc.dk)                                         *
 ***************************************************************/

#ifndef NODE_H
#define NODE_H

#include <stdio.h>
#include "config.h"
#include "gingerNew.h"
#include "color.h"

#if USE_XPM_LIB
#include <X11/xpm.h>
#endif

#define NODE_MARGIN 3
#define NODE_HIGHLIGHT_WIDTH (NODE_MARGIN-1)
#define DEFAULT_LABEL_HEIGHT 16
#define LABEL_HEIGHT_FROM_BOTTOM 0
#define DEFAULT_NODE_HEIGHT 20
#define DEFAULT_NODE_WIDTH 20

#define SHAPE_NONE      0
#define SHAPE_RECTANGLE 1
#define SHAPE_ELLIPSE   2
#define SHAPE_RHOMBE    3

#define SHAPE_DEFAULT SHAPE_RECTANGLE 

#define ICON_NONE   0
#define ICON_BITMAP 1
#define ICON_PIXMAP 2

typedef struct {
  char *name;          /* node name (must be unique) */
  char *label;         /* node label (need not be unique, and may be 
			  the empty string "") */
  int  x,y;            /* coordinates of node's center */
  int  width, height;  /* node dimensions */
  int  shape;          /* SHAPE_RECTANGLE, SHAPE_ELLIPSE, or SHAPE_NONE */
  color_info foreground; /* information about node colors, used only */
  color_info background; /*  if the these differ from the default
			     node colors */
  
  int label_x, label_y; /* label position relative to upper left corner
			   of the node */
  char *contents;      /* node "contents", usually a filename to be 
			  given as parameter to the editor which follows */
  char *editor;        /* command-line of editor for the node's contents.
			  The contents of contents is appended to this 
			  string to form the actual edit-command */

  int selected;        /* Is > 0 if the node is selected */
 
  /* bookkeeping for the tree layout algoritm */
  struct {
    int level;
    int position;  /* the node's position within the level */
    int upper_connectivity;
    int lower_connectivity;
    int priority;
  } tree_layout;
  
  /* information about the node icon */
  struct {
    int  type;         /* ICON_NONE, ICON_BITMAP, or ICON_PIXMAP */
    int  width, height;/* icon dimensions       */
    char *path;        /* icon file name        */
    Pixmap image;      /* icon bitmap or pixmap */
#if USE_XPM_LIB
    XpmAttributes attr; /* color attributes */
#endif
  } icon;   
  
  Pixmap image;        /* visual presentation of node including label,
			  bitmap/pixmap icon etc. */
#if USE_SCHEME
  unsigned long object;/* identification the corresponding Scheme object. 
			  There will always exist a corresponding 
			  reference to the object on the Scheme side, 
			  therefore this reference does not need to be
			  protected from garbage collection.
		       */
#endif
} node;

/* ---  public functions  --- */

node *createNode(char *name, char *label, int shape, int x, int y,
		 int width, int height, int iconType, char *iconPath);
void destroyNode(node *n);
void freeNode(node *n);
void makeNodeImage(node *n);
void clearNodeImage(node *n);
void nodeReallocImage(node *n);

void displayNodeImageOnScreen(node *n);
void undisplayNodeImageOnScreen(node *n);

void printNode(node *n, FILE *fd);

void nodeSetContents(node *n, char *cont);
void nodeSetEditor(node *n, char *edit);
void nodeRunEditorOnContents(node *n);

void nodeSetForeground(node *n, char *colorname);
void nodeSetBackground(node *n, char *colorname);

void nodeSetName(node *n, char *name);
void nodeSetLabel(node *n, char *label);
void nodeSetIcon(node *n, int icon_type, char *icon_file);

void nodeHighlight(node *n, int highlight_on);

#endif
