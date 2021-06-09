/* $Id: symtable.c,v 1.10 1996/03/04 08:50:05 lupus Exp $ */

/***************************************************************
 * This file is part of the Ginger project.                    *
 * Created 2-1-1996                                            *
 * Niels C. Larsen, Aalborg University, Denmark                *
 * (lupus@iesd.auc.dk)                                         *
 ***************************************************************/

/* Symbol table used by the GGL parser */

#include <stdio.h>
#include <string.h>
#include "nodeSelectionWindow.h"
#include "edgeSelectionWindow.h"
#include "nodeSelection.h"
#include "edgeSelection.h"
#include "symtable.h"
#include "hotspot.h"

extern FILE *error_output;  /* in ginger.y */

/* ---  private declarations  --- */

static CLIST *symlist;      /* linked list used as symbol table */

/* ---  public functions  --- */

void resetObj(attribute_obj *a) {
  a->ok=0;
  a->vlabel=0;
  a->vshape=0;
  a->vbitmap=0;
  a->vpixmap=0;
  a->vcontents=0;
  a->veditor=0;
  a->vcolor=0;
  a->vbg_color=0;
  a->vto=0; 
  a->vfrom=0;
  a->vtemplate=0;
  a->vx=0;
  a->vy=0;
  a->vwidth=0;
  a->vheight=0;
  a->vdirection=0;
}

void destroyObj(attribute_obj *a) {
  GingerFree(a->name);
  if(a->vlabel!=0)
    GingerFree(a->label);
  if(a->vbitmap!=0)
    GingerFree(a->bitmap);
  if(a->vpixmap!=0)
    GingerFree(a->pixmap);
  if(a->vcontents!=0)
    GingerFree(a->contents);
  if(a->veditor!=0)
    GingerFree(a->editor);
  if(a->vcolor!=0)
    GingerFree(a->color);
  if(a->vbg_color!=0)
    GingerFree(a->bg_color);
  if(a->vto!=0) 
    GingerFree(a->to); 
  if(a->vfrom!=0)
    GingerFree(a->from);
  if(a->vtemplate!=0)
    GingerFree(a->template);
}

void initTable() {
  symlist=newlist(STRUCT, sizeof(attribute_obj));
}

void addObj(attribute_obj *a) {
  insert_last((BYTE *)a, symlist);
}

void printObj(attribute_obj *a) {
  if(a->vlabel!=0)
    printf("label: %s\n", a->label);
  if(a->vcontents!=0)
    printf("contents: %s\n", a->contents);
  if(a->veditor!=0)
    printf("editor: %s\n", a->editor);
  if(a->vcolor!=0)
    printf("color: %s\n", a->color);
  if(a->vto!=0) 
    printf("to: %s\n", a->to);
  if(a->vfrom!=0)
    printf("from: %s\n", a->from);
  if(a->vtemplate!=0)
    printf("template: %s\n", a->template);
  if(a->vx!=0)
    printf("x: %d\n", a->x);
  if(a->vy!=0)
    printf("y: %d\n", a->y);
  if(a->vwidth!=0)
    printf("width: %d\n", a->width);
  if(a->vheight!=0)
    printf("height: %d\n", a->height);
  if(a->vdirection!=0)
    printf("direction: %d\n", a->direction);
}

void mergeObj(attribute_obj *dest, attribute_obj *src) {
  if(src->vlabel!=0 && dest->vlabel==0) {
    dest->label=GingerNewString(src->label);
    dest->vlabel=1;
  }
  if(src->vbitmap!=0 && dest->vbitmap==0) {
    dest->bitmap=GingerNewString(src->bitmap);
    dest->vbitmap=1;
  }
  if(src->vpixmap!=0 && dest->vpixmap==0) {
    dest->pixmap=GingerNewString(src->pixmap);
    dest->vpixmap=1;
  }
  if(src->vcontents!=0 && dest->vcontents==0) {
    dest->contents=GingerNewString(src->contents);
    dest->vcontents=1;
  }
  if(src->veditor!=0 && dest->veditor==0) {
    dest->editor=GingerNewString(src->editor);
    dest->veditor=1;
  }
  if(src->vcolor!=0 && dest->vcolor==0) {
    dest->color=GingerNewString(src->color);
    dest->vcolor=1;
  }
  if(src->vbg_color!=0 && dest->vbg_color==0) {
    dest->bg_color=GingerNewString(src->bg_color);
    dest->vbg_color=1;
  }
  if(src->vto!=0 && dest->vto==0) { 
    dest->to=GingerNewString(src->to);
    dest->vto=1;
  }
  if(src->vfrom!=0 && dest->vfrom==0) {
    dest->from=GingerNewString(src->from);
    dest->vfrom=1;
  }
  if(src->vx!=0 && dest->vx==0) {
    dest->x=src->x;
    dest->vx=1;
  }
  if(src->vy!=0 && dest->vy==0) {
    dest->y=src->y;
    dest->vy=1;
  }
  if(src->vwidth!=0 && dest->vwidth==0) {
    dest->width=src->width;
    dest->vwidth=1;
  }
  if(src->vheight!=0 && dest->vheight==0) {
    dest->height=src->height;
    dest->vheight=1;
  }
  if(src->vshape!=0 && dest->vshape==0) {
    dest->shape=src->shape;
    dest->vshape=1;
  }
  if(src->vdirection!=0 && dest->vdirection==0) {
    dest->direction=src->direction;
    dest->vdirection=1;
  }
}

attribute_obj *findObjNamed(char *name, int obj_type) {
  int i;
  attribute_obj *res;
  
  for(i=1; i<=getsize(symlist); i++) {
    res=(attribute_obj *)retrieve(i,symlist);
    if( res->type == obj_type )
      if( !strcmp(res->name, name) )
	return res;
  }
  return 0;
}

void semanticError(char *obj_type, char *obj_name, char *message) {
  fprintf(error_output, "%s %s: %s\n", obj_type, obj_name, message);
}

/* return 1 if ok */
int checkNode(attribute_obj *a) {
  attribute_obj *templ;
  
  if( a->vtemplate ) {
    if( (templ=findObjNamed(a->template, T_TEMPLATE))==0 ) {
      semanticError("Node", a->name, "reference to non-existing template");
    } else {
      mergeObj(a,templ);
    }
  }

  if( !a->vx ) {
    a->x=-1;        /* default value will be assigned by createNode */
  }

  if( !a->vy ) {
    a->y=-1;        /* default value will be assigned by createNode */
  }

  if( !a->vwidth )
    a->width = 0;   /* default value will be assigned by createNode */

  if( !a->vheight )
    a->height = 0;  /* default value will be assigned by createNode */
  
  if( !a->vlabel )
    a->label = "";

  return 1;
}

/* return 1 if ok */
int checkTemplate(attribute_obj *a) {
  attribute_obj *templ;
  
  if( a->vtemplate ) {
    if( (templ=findObjNamed(a->template, T_TEMPLATE))==0 ) {
      semanticError("Template", a->name, "refers to non-existing template");
    } else {
      mergeObj(a,templ);
    }
  }
  
  return 1;
}

/* return 1 if ok */
int checkEdge(attribute_obj *a) {
  attribute_obj *templ, *n;

  if( a->vtemplate ) {
    if( (templ=findObjNamed(a->template, T_TEMPLATE))==0 ) {
      semanticError("Edge", a->name, "refers to non-existing template");
    } else {
      mergeObj(a,templ);
    }
  }

  if( a->vto==0 ) {
    semanticError("Edge", a->name, "missing specification of target node");
    return 0;
  }
  if( !(n=findObjNamed(a->to, T_NODE)) ) {
    semanticError("Edge", a->name, "non-existing target node");
    return 0;
  }
  if( !n->ok ) {
    semanticError("Edge", a->name, "bogus target node");
    return 0;
  }
  if( a->vfrom==0 ) {
    semanticError("Edge", a->name, "missing specification of source node");
    return 0;
  }
  if( !(n=findObjNamed(a->from, T_NODE)) ) {
    semanticError("Edge", a->name, "non-existing source node");
    return 0;
  }
  if( !n->ok ) {
    semanticError("Edge", a->name, "bogus source node");
    return 0;
  }
  return 1;
}

void semanticCheck() {
  int i;
  attribute_obj *a;

  /* we need to check (and build) templates first */
  for(i=1; i<=getsize(symlist); i++) {
    a=(attribute_obj *)retrieve(i,symlist);
    if(a->type==T_TEMPLATE) {
      a->ok=checkTemplate(a);
    }
  }
  /* then we can check nodes */
  for(i=1; i<=getsize(symlist); i++) {
    a=(attribute_obj *)retrieve(i,symlist);
    if(a->type==T_NODE) {
      a->ok=checkNode(a);
    }
  }

  /* and finally edges */
  for(i=1; i<=getsize(symlist); i++) {
    a=(attribute_obj *)retrieve(i,symlist);
    if(a->type==T_EDGE) {
      a->ok=checkEdge(a);
    }
  }
}

graph *buildGraphFromSymtable() {
  int i;
  attribute_obj *a, *a1, *a2;
  graph *res;
  node *n;
  edge *e;

  semanticCheck(); 

  res=createGraph();

  /* we need to build the nodes first */
  for(i=1; i<=getsize(symlist); i++) {
    a=(attribute_obj *)retrieve(i,symlist);
    if(a->type==T_NODE) {
      if(a->ok) {
	if( !a->vshape )
	  a->shape = SHAPE_DEFAULT;
	if( a->vbitmap )
	  n = createNode(a->name, a->label, a->shape, a->x, a->y, a->width, a->height, ICON_BITMAP, a->bitmap); 
	else if( a->vpixmap )
	  n = createNode(a->name, a->label, a->shape, a->x, a->y, a->width, a->height, ICON_PIXMAP, a->pixmap); 
	else
	  n = createNode(a->name, a->label, a->shape, a->x, a->y, a->width, a->height, ICON_NONE, ""); 
	if( a->veditor )
	  nodeSetEditor(n, a->editor);
	if( a->vcontents )
	  nodeSetContents(n, a->contents);
	
	if( a->vcolor )
	  nodeSetForeground(n,a->color);
	if( a->vbg_color )
	  nodeSetBackground(n,a->bg_color);
	makeNodeImage(n); /* initialize node image */

	a->n = addNode(res, n);
	freeNode(n);
      }
    }
  }

  /* then we can build the edges */
  for(i=1; i<=getsize(symlist); i++) {
    a=(attribute_obj *)retrieve(i,symlist);
    if(a->type==T_EDGE) {
      if(a->ok) {
	a1=findObjNamed(a->from, T_NODE);
	a2=findObjNamed(a->to, T_NODE);
	e = createEdge(a->name, a1->n, a2->n);
	if( a->vdirection )
	  e->direction = a->direction;
	if( a->vcolor )
	  edgeSetColor(e, a->color);
	addEdge(res,e);
	freeEdge(e);
      }
    }
  }

  return res;
}

graph *mergeGraphFromSymtableWithGraph(graph *g, 
				       int node_clash_mode,
				       int edge_clash_mode) {
  int i, create_node, create_edge, x_offset, y_offset;
  int first;
  attribute_obj *a, *a1, *a2;
  graph *res;
  node *n;
  edge *e;
  char *the_name;
  char buf[256];

  semanticCheck(); 
  
  hotspotGet(&x_offset, &y_offset);
  
  res=g;
  first=1;

  /* we need to build the nodes first */
  for(i=1; i<=getsize(symlist); i++) {
    a=(attribute_obj *)retrieve(i,symlist);
    if(a->type==T_NODE) {
      if(a->ok) {
	create_node=1;
	the_name=a->name;
	n=graphFindNodeNamed(g,a->name);
	if(n) {
	  switch(node_clash_mode) {
	  case CLASH_MODE_MERGE:
	    create_node=0;
	    break;
	  case CLASH_MODE_WARN_AND_RENAME:
	    semanticError("Node", a->name, "multiply defined. Renaming.");
	    /* no break here */
	  case CLASH_MODE_RENAME:
	    the_name=createNewNodeName(g,buf);
	    create_node=1;
	    break;
	  }
	}
	if( create_node ) {
	  a->x+=x_offset;
	  a->y+=y_offset;
	  if( !a->vshape )
	    a->shape = SHAPE_DEFAULT;
	  if( a->vbitmap )
	    n = createNode(the_name, a->label, a->shape, a->x, a->y, a->width, a->height, ICON_BITMAP, a->bitmap); 
	  else if( a->vpixmap )
	    n = createNode(the_name, a->label, a->shape, a->x, a->y, a->width, a->height, ICON_PIXMAP, a->pixmap); 
	  else
	    n = createNode(the_name, a->label, a->shape, a->x, a->y, a->width, a->height, ICON_NONE, ""); 
	  if( a->veditor )
	    nodeSetEditor(n, a->editor);
	  if( a->vcontents )
	    nodeSetContents(n, a->contents);
	  
	  if( a->vcolor )
	    nodeSetForeground(n,a->color);
	  if( a->vbg_color )
	    nodeSetBackground(n,a->bg_color);
	  makeNodeImage(n); /* initialize node image */
	  
	  a->n = addNode(res, n);
	  if(first) {
	    first=0;
	    nodeSelect(res,a->n,nodeSelectionWindowGetAccumulationState());
	  }
	  else
	    nodeSelect(res, a->n, ACCUMULATING_MODE);
	  freeNode(n);
	}
	else
	  a->n = n;  /* use existing node */
      }
    }
  }

  first=1;

  /* then we can build the edges */
  for(i=1; i<=getsize(symlist); i++) {
    a=(attribute_obj *)retrieve(i,symlist);
    if(a->type==T_EDGE) {
      if(a->ok) {
	create_edge=1;
	the_name=a->name;
	e=graphFindEdgeNamed(g,a->name);
	if(e) {
	  switch(edge_clash_mode) {
	  case CLASH_MODE_MERGE:
	    create_edge=0;
	    break;
	  case CLASH_MODE_WARN_AND_RENAME:
	    semanticError("Edge", a->name, "multiply defined. Renaming.");
	    /* no break here */
	  case CLASH_MODE_RENAME:
	    the_name=createNewEdgeName(g,buf);
	    create_edge=1;
	    break;	  
	  }
	}
	if( create_edge ) {
	  a1=findObjNamed(a->from, T_NODE);
	  a2=findObjNamed(a->to, T_NODE);
	  e = createEdge(the_name, a1->n, a2->n);
	  if( a->vdirection )
	    e->direction = a->direction;
	  if( a->vcolor )
	    edgeSetColor(e, a->color);
	  a->e = addEdge(res,e);
	  if(first) {
	    first=0;
	    edgeSelect(res,a->e,edgeSelectionWindowGetAccumulationState());
	  }
	  else
	    edgeSelect(res, a->e, ACCUMULATING_MODE);
	  freeEdge(e);
	}
	else
	  a->e=e;  /* use existing edge */
      }
    }
  }
  
  return res;
}

/* delete everything in the symbol table, deallocate the
   attribute objects and the text strings they refer to.
   Also deallocate the table itself.
*/
void cleanUpTable() {
  int i;
  attribute_obj *a;
  
  /* deallocate the contents of attribute objects  */
  for(i=1; i<=getsize(symlist); i++) {
    a=(attribute_obj *)retrieve(i,symlist);
    destroyObj(a);
  }

  /* delete and deallocate the attribute objects */
  delete_all(symlist);

  /* deallocate the symbol table */
  free(symlist);
}
