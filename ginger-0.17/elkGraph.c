/* $Id: elkGraph.c,v 1.14 1996/04/09 07:11:31 lupus Exp $ */

/***************************************************************
 * This file is part of the Ginger project.                    *
 * Created 01-03-1996                                          *
 * Niels C. Larsen, Aalborg University, Denmark                *
 * (lupus@iesd.auc.dk)                                         *
 * Martin K. Molz, Aalborg University, Denmark                 *
 * (molz@iesd.auc.dk)                                          *
 ***************************************************************/

#include "config.h"

#if USE_SCHEME  /* if not, skip the rest of this file */

#include <string.h>

#include "scheme.h"
#undef HUGE
#include "graph.h"
#include "elkGraph.h"
#include "graphBuffer.h"
#include "graphDisplay.h"
#include "nodeSelection.h"
#include "edgeSelection.h"

/*  ---  private declarations  ---  */

static int nodeType;
static int edgeType;

struct t_node {
  Object unused;
  node *n;
};

struct t_edge {
  Object unused;
  edge *e;
};

#define GET_NODE(obj) ((struct t_node *)POINTER(obj))
#define GET_EDGE(obj) ((struct t_edge *)POINTER(obj))

/* private function prototypes */
static Object p_create_node();
static Object p_create_edge(Object node1, Object node2);

static Object p_ginger_display(Object obj, Object type);
static Object p_ginger_set_attribute(Object obj, Object type, 
			      Object name, Object value);
static Object p_ginger_undisplay(Object obj, Object type);
static Object p_ginger_destroy(Object obj, Object type);
static Object p_ginger_edge_init(Object edge, Object from, Object to);

static Object p_ginger_error(Object a);
static int node_equal(Object n1, Object n2);
static int node_print(Object node, Object port, int raw, int length, int depth);
static int edge_equal(Object e1, Object e2);
static int edge_print(Object edge, Object port, int raw, int length, int depth);


/* ---  public functions  --- */

void installGraphPrimitives() {
/*
  nodeType = Define_Type(0, "node", 0, sizeof(struct t_node),
			     node_equal, node_equal, node_print, 0);
  edgeType = Define_Type(0, "edge", 0, sizeof(struct t_edge),
			     edge_equal, edge_equal, edge_print, 0);

  Define_Primitive(p_create_node, "nodeCreate",0 ,0, EVAL);
  Define_Primitive(p_create_edge, "edgeCreate",2 ,2, EVAL);
*/

  Define_Primitive(p_ginger_display,   "gingerDisplay",   2 ,2, EVAL);
  Define_Primitive(p_ginger_undisplay, "gingerUndisplay", 2 ,2, EVAL);
  Define_Primitive(p_ginger_destroy,   "gingerDestroy",   2 ,2, EVAL);
  Define_Primitive(p_ginger_set_attribute, "gingerSetAttribute", 4 ,4, EVAL);
  Define_Primitive(p_ginger_edge_init, "gingerEdgeInit", 3 ,3, EVAL);

  Define_Primitive(p_ginger_error, "gingerError",1 ,1, EVAL);
}

/* ---  primitives exported to elk  --- */

static Object p_ginger_display(Object obj, Object type) {
  node *temp, *n;
  edge *e;
  graph *g;
  char *name;
  char *symbol;
  char buf[256];
  
  if( TYPE(type)!=T_Symbol ) {
    return obj;
  }
  
  symbol=Get_String(SYMBOL(type)->name);
  
  g=graphBufferGetCurrentGraph();

  if( !strcmp(symbol, "node") ) {   /* if it's a node */
    n = temp = graphFindNodeObject(g, (unsigned long)Get_Integer(obj));
    if(!temp) {
      name = createNewNodeName(g,buf);
      temp = createNode(name, "", SHAPE_DEFAULT, -1, -1,
			0, 0, 0, "");
      n = addNode(g, temp);
      n->object=(unsigned long)Get_Integer(obj);
      freeNode(temp);
      makeNodeImage(n);
    }
    moveNode(g, n, n->x, n->y);
  }
  else if( !strcmp(symbol, "edge") ) {   /* if it's an edge */
    e = graphFindEdgeObject(g, (unsigned long)Get_Integer(obj));
    if(e) {
      drawEdge(e);
    }
  }
  
  return obj;
}

static Object p_ginger_undisplay(Object obj, Object type) {
  node *n;
  edge *e;
  graph *g;
  char *symbol;
  
  if( TYPE(type)!=T_Symbol ) {
    return obj;
  }
  
  symbol=Get_String(SYMBOL(type)->name);
  
  g=graphBufferGetCurrentGraph();

  if( !strcmp(symbol, "node") ) {   /* if it's a node */
    n = graphFindNodeObject(g, (unsigned long)Get_Integer(obj));
    if(n) {
      undisplayNodeImageOnScreen(n);
    }
  }
  else if( !strcmp(symbol, "edge") ) {   /* if it's an edge */
    e = graphFindEdgeObject(g, (unsigned long)Get_Integer(obj));
    if(e) {
      undrawEdge(e);
    }
  }
  
  return obj;
}

static Object p_ginger_destroy(Object obj, Object type) {
  node *n;
  edge *e;
  graph *g;
  char *symbol;
  
  GC_Node2;
  GC_Link2(obj, type);

  if( TYPE(type)!=T_Symbol ) {
    GC_Unlink;
    return obj;
  }
  
  symbol=Get_String(SYMBOL(type)->name);
  
  g=graphBufferGetCurrentGraph();
  
  if( !strcmp(symbol, "node") ) {   /* if it's a node */
    n = graphFindNodeObject(g, (unsigned long)Get_Integer(obj));
    if(n) {
      graphDisplayDeleteNode(g,n);
    }
  }
  else if( !strcmp(symbol, "edge") ) {   /* if it's an edge */
    e = graphFindEdgeObject(g, (unsigned long)Get_Integer(obj));
    if(e) {
      graphDisplayDeleteEdge(g,e);
    }
  }
  
  GC_Unlink;
  return obj;
}

static Object p_ginger_set_attribute(Object obj, Object type, 
				     Object name, Object value) {
  char *the_type, *the_name, *the_value;
  node *temp, *n;
  edge *e;
  graph *g; 
  int val;
  char *newname;
  char buf[256];
  
  if( TYPE(type)!=T_Symbol || TYPE(name)!=T_Symbol ) {
    return obj;
  }
  
  the_type=Get_String(SYMBOL(type)->name);
  the_name=Get_String(SYMBOL(name)->name);
  
  g=graphBufferGetCurrentGraph();
  
  if( !strcmp(the_type, "node") ) {   /* if it's a node */
    n = temp = graphFindNodeObject(g, (unsigned long)Get_Integer(obj));
    if(!temp) {
      newname = createNewNodeName(g,buf);
      temp = createNode(newname, "", SHAPE_DEFAULT, -1, -1,
			0, 0, 0, "");
      n = addNode(g, temp);
      n->object=(unsigned long)Get_Integer(obj);
      makeNodeImage(n);
      freeNode(temp);
    }
    if(!strcmp(the_name,"x")) {
      n->x=Get_Integer(value);
    }
    else if(!strcmp(the_name,"y")) {
      n->y=Get_Integer(value);
    }
    else if(!strcmp(the_name,"width")) {
      val=Get_Integer(value);
      if(n->width!=val) {
	n->width=val;
	nodeReallocImage(n);
	makeNodeImage(n);
      }
    }
    else if(!strcmp(the_name,"height")) {
      val=Get_Integer(value);
      if(n->height!=val) {
	n->height=val;
	nodeReallocImage(n);
	makeNodeImage(n);
      }
    }
    else if(!strcmp(the_name,"selected")) {
      if( Get_Integer(value)!=0 )
	nodeSelect(g,n,ACCUMULATING_MODE);
      else
	nodeUnselect(g,n);
    }
    else if(!strcmp(the_name,"label")) {
      if(TYPE(value)!=T_String)
	return obj;
      nodeSetLabel(n, Get_String(value));
      nodeReallocImage(n);
      makeNodeImage(n);
    }
    else if(!strcmp(the_name,"contents")) {
      if(TYPE(value)!=T_String)
	return obj;
      nodeSetContents(n, Get_String(value));
    }
    else if(!strcmp(the_name,"editor")) {
      if(TYPE(value)!=T_String)
	return obj;
      nodeSetEditor(n, Get_String(value));
    }
    else if(!strcmp(the_name,"foreground")) {
      if(TYPE(value)!=T_String)
	return obj;
      nodeSetForeground(n, Get_String(value));
      makeNodeImage(n);
    }
    else if(!strcmp(the_name,"shape")) {
      if(TYPE(value)!=T_String)
	return obj;
      the_value=Get_String(value);
      if(!strcmp(the_value, "rectangle")) 
	n->shape=SHAPE_RECTANGLE;
      else if(!strcmp(the_value, "none")) 
	n->shape=SHAPE_NONE;
      else if(!strcmp(the_value, "ellipse")) 
	n->shape=SHAPE_ELLIPSE;
      else if(!strcmp(the_value, "rhomb")) 
	n->shape=SHAPE_RHOMBE;
      makeNodeImage(n);
    }
    else if(!strcmp(the_name,"background")) {
      if(TYPE(value)!=T_String)
	return obj;
      nodeSetBackground(n, Get_String(value));
      makeNodeImage(n);
    }
    else if(!strcmp(the_name,"bitmap")) {
      if(TYPE(value)!=T_String)
	return obj;
      nodeSetIcon(n, ICON_BITMAP, Get_String(value));
      nodeReallocImage(n);
      makeNodeImage(n);
    }
    else if(!strcmp(the_name,"pixmap")) {
      if(TYPE(value)!=T_String)
	return obj;
      nodeSetIcon(n, ICON_PIXMAP, Get_String(value));
      nodeReallocImage(n);
      makeNodeImage(n);
    }
  }
  else if( !strcmp(the_type, "edge") ) {   /* if it's an edge */
    e = graphFindEdgeObject(g, (unsigned long)Get_Integer(obj));
    if(!e) {
      return obj;
    }
    if(!strcmp(the_name,"foreground")) {
      if(TYPE(value)!=T_String)
	return obj;
      edgeSetColor(e, Get_String(value));
    }
    else if(!strcmp(the_name,"selected")) {
      if( Get_Integer(value)!=0 )
	edgeSelect(g,e,ACCUMULATING_MODE);
      else
	edgeUnselect(g,e);
    }
    else if(!strcmp(the_name,"from")) {
      n = graphFindNodeObject(g, (unsigned long)Get_Integer(value));
      if(n) {
	e->from=n;
	calculateEdgeEndpoints(e);
      }
    }
    else if(!strcmp(the_name,"to")) {
      n = graphFindNodeObject(g, (unsigned long)Get_Integer(value));
      if(n) {
	e->to=n;
	calculateEdgeEndpoints(e);
      }
    }
    else if(!strcmp(the_name,"direction")) {
      if(TYPE(value)!=T_String)
	return obj;
      the_value=Get_String(value);
      if(!strcmp(the_value, "normal")) 
	e->direction=DIR_NORMAL;
      else if(!strcmp(the_value, "none")) 
	e->direction=DIR_NONE;
      else if(!strcmp(the_value, "inverse")) 
	e->direction=DIR_INVERSE;
      else if(!strcmp(the_value, "both")) 
	e->direction=DIR_BIDIRECTIONAL;
    }
  }
  
  return obj;
}

static Object p_ginger_edge_init(Object the_edge, Object from, Object to) {
  edge *temp, *e;
  node *from_node, *to_node;
  graph *g;
  char *name;
  char buf[256];
  
  g=graphBufferGetCurrentGraph();
  
  e = graphFindEdgeObject(g, (unsigned long)Get_Integer(the_edge));
  if(e) {
    return the_edge;
  }
  
  from_node = graphFindNodeObject(g, (unsigned long)Get_Integer(from));
  to_node = graphFindNodeObject(g, (unsigned long)Get_Integer(to));
  
  if( from_node!=0 && to_node!=0 ) {
    name=createNewEdgeName(g,buf);
    temp = createEdge(name, from_node, to_node);
    
    e = addEdge(g, temp);
    e->object=(unsigned long)Get_Integer(the_edge);
    freeEdge(temp);
  }
  
  return the_edge;
}

static Object p_create_node() {
  Object result;
  node *temp;
  graph *g;
  char *name;
  char buf[256];
  
  g=graphBufferGetCurrentGraph();
  
  name = createNewNodeName(g,buf);
  temp = createNode(name, "", SHAPE_DEFAULT, -1, -1,
		    0, 0, 0, "");
  result = Alloc_Object(sizeof(struct t_node), nodeType, 0);
  GET_NODE(result)->n = addNode(g, temp);
  freeNode(temp);
  makeNodeImage(GET_NODE(result)->n);
  displayNodeImageOnScreen(GET_NODE(result)->n);
  
  return result;
}

static Object p_create_edge(Object node1, Object node2) {
  Object result;
  edge *temp;
  graph *g;
  char *name;
  char buf[256];
  GC_Node2;
    
  GC_Link2(node1, node2);
  
  Check_Type(node1, nodeType);
  Check_Type(node2, nodeType);
  
  g=graphBufferGetCurrentGraph();
  name=createNewEdgeName(g,buf);
  temp = createEdge(name, GET_NODE(node1)->n, GET_NODE(node2)->n);
  
  result = Alloc_Object(sizeof(struct t_edge), edgeType, 0);
  GET_EDGE(result)->e = addEdge(g, temp);
  freeEdge(temp);
  drawEdge(GET_EDGE(result)->e);
  GC_Unlink;
  
  return result;
}

/* error handler */
static Object p_ginger_error(Object a) {
  Object result;
  Object out, str;
  char *p;
  char buf[256];

  GC_Node2;

  out = P_Open_Output_String();
  GC_Link2(a, out);
  General_Print_Object(a, out, 1);
  str = P_Get_Output_String(out);
  p = Get_String(str);
  if(p) {
    if(strlen(p) >= sizeof(buf)-10 )
      p="\n[error message too long to display]\n";
    else {
      sprintf(buf, "\n%s\n", p);
      p=buf;
    }
    promptWindowAppendText(buf);
  }
  
  result=a;
  GC_Unlink;
  
  return result;
}

/* ---  private functions  --- */

static int node_equal(Object n1, Object n2) {
  return GET_NODE(n1)->n == GET_NODE(n2)->n;
}

static int node_print(Object node, Object port, int raw, int length, int depth) {
  Printf(port, "#[node %s]", (GET_NODE(node)->n)->name);
  return 0;
}

static int edge_equal(Object e1, Object e2) {
  return GET_EDGE(e1)->e == GET_EDGE(e2)->e;
}

static int edge_print(Object edge, Object port, int raw, int length, int depth) {
  Printf(port, "#[edge %s]", (GET_EDGE(edge)->e)->name);
  return 0;
}

#endif  /* USE_SCHEME */

