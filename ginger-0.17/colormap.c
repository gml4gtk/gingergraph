/* $Id: colormap.c,v 1.2 1996/02/12 13:31:05 lupus Exp $ */

/***************************************************************
 * This file is part of the Ginger project.                    *
 * Created 2-2-1996                                            *
 * Niels C. Larsen, Aalborg University, Denmark                *
 * (lupus@iesd.auc.dk)                                         *
 ***************************************************************/

/* take care of allocating colors and GCs for nodes and edges */

#include <X11/Intrinsic.h>
#include <string.h>
#include <stdio.h>
#include "str.h"
#include "clist.h"
#include "colormap.h"

/* ---  private declarations  --- */

extern Display *the_display;  /* from mainWindow.c */
extern Drawable the_drawable;
extern Colormap the_colormap;
extern Pixel    the_background;

#define MAX_NAME_LENGTH 40

typedef struct {
  char color_name[MAX_NAME_LENGTH];
  GC the_gc;
  int reference_count;
} color_entry;

static CLIST *color_list;

static color_entry *findEntry(char *colorname);
static void colormapInitialize();
static void printColormap();

/* ---  public functions  --- */

GC colormapAllocateColorGC(char *colorname) {
  color_entry *c;
  color_entry new_entry;
  XGCValues   values;
  XColor      color, ex;

  colormapInitialize(); /* make sure colormap is initialized */
  
  /* printColormap();  */ /* test */
  
  if( (c=findEntry(colorname)) ) { /* if we already have the color */
    c->reference_count++;
    return c->the_gc;
  }
  else {  /* make new entry and enter it in the table */
    zeroAddStrncpy(new_entry.color_name, colorname, MAX_NAME_LENGTH);
    new_entry.reference_count=1;
    XAllocNamedColor(the_display, the_colormap, colorname, &color, &ex);
    values.foreground = color.pixel;
    values.background = the_background;
    new_entry.the_gc  = XCreateGC(the_display, the_drawable,
				  GCForeground | GCBackground, &values);
    insert_first((BYTE *)&new_entry, color_list);
    return new_entry.the_gc;
  }
}

void colormapDeallocateColorGC(char *colorname) {
  color_entry *c;

  colormapInitialize(); /* make sure colormap is initialized */
  
  if( (c=findEntry(colorname)) ) { /* if we have the color */
    c->reference_count--;
    if( c->reference_count==0 ) {
      /* deallocate color, GC, and remove item from list */
      XFreeGC(the_display,c->the_gc);
      delete_item((BYTE *)c, color_list);
    }
  }
}

/* ---  private functions  --- */

static color_entry *findEntry(char *colorname) {
  int i;
  color_entry *c;
  
  for(i=1; i<=getsize(color_list); i++) {
    c=(color_entry *)retrieve(i, color_list);
    if(!strcmp(c->color_name, colorname))
      return c;
  }
  return 0;
}

static void colormapInitialize() {
  static int done=0;
  
  if(done)
    return;
  done=1;
  
  color_list = newlist(STRUCT,sizeof(color_entry));
}

/* testing function */
static void printColormap() {
  int i;
  color_entry *c;
  
  for(i=1; i<=getsize(color_list); i++) {
    c=(color_entry *)retrieve(i, color_list);
    printf("table size %d, name: %s, ref: %d\n", getsize(color_list), c->color_name, c->reference_count);
  }
}

