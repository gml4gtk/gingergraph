/* $Id: mainWindow.c,v 1.33 1996/06/14 12:18:44 lupus Exp $ */

/***************************************************************
 * This file is part of the Ginger project.                    *
 * Created 27-12-1995                                          *
 * Niels C. Larsen, Aalborg University, Denmark                *
 * (lupus@iesd.auc.dk)                                         *
 ***************************************************************/

/* Functions to create, initialize, and pop up the main window */

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include <X11/Intrinsic.h>
#include <X11/StringDefs.h>
#include <X11/Shell.h>

#include <X11/Xaw/Command.h>
#include <X11/Xaw/Paned.h>
#include <X11/Xaw/Form.h>
#include <X11/Xaw/Box.h>
#include <X11/Xaw/Viewport.h>
#include <X11/Xaw/Text.h>
#include <X11/Xaw/AsciiText.h> 

#include "mainWindow.h"
#include "config.h"
#include "mathMacros.h"
#include "icon.h"
#include "focus.h"
#include "menu.h"
#include "menuOpcodes.h"
#include "options.h"
#include "graph.h"
#include "graphIO.h"
#include "graphDisplay.h"
#include "graphBuffer.h"
#include "graphInteraction.h"
#include "PostScript.h"
#include "helpWindow.h"
#include "nodeSelection.h"
#include "nodeSelectionWindow.h"
#include "edgeSelection.h"
#include "edgeSelectionWindow.h"
#include "nodeAttributeWindow.h"
#include "edgeAttributeWindow.h"
#include "promptWindow.h"
#include "hotspot.h"
#include "actions.h"

/* ---  public main window Widgets  --- */

Widget topLevel = NULL;

/* ---  private main window Widgets  --- */

static Widget graphArea, messageField;

/* ---  other private declarations  --- */

#define GRAPH_WINDOW_WIDTH 440
#define GRAPH_WINDOW_HEIGHT 240

#define MAIN_WINDOW_WIDTH  GRAPH_WINDOW_WIDTH+15
#define MAIN_WINDOW_HEIGHT (GRAPH_WINDOW_HEIGHT+135)
#define MW_BUTTON_WIDTH 80
#define MW_NODE_EDGE_BUTTON_WIDTH 120
#define BUTTON_PANE_HEIGHT 28

static XtAppContext app_context;
static ApplicationResources app_resources;

static int graph_width, graph_height;

/* ---  file menu  --- */

static menu_item file_menu[] = {
  {"Load",         1, FILE_LOAD, 0},
  {"Insert",       1, FILE_INSERT, 0},
  {"Save",         1, FILE_SAVE, 0},
  {"Save as",      1, FILE_SAVE_AS, 0},
  {"New",          1, FILE_NEW,  0},
  {"Scheme prompt", USE_SCHEME, OPCODE_SCHEME_PROMPT, 0},
  {"Export PostScript", 1, FILE_EXPORT_POSTSCRIPT, 0},
  {"line",        -1, -1,  0},
  {"Quit",         1, FILE_QUIT, 0},
  {0,0,0,0} 
};

/* ---  layout menu  --- */

static menu_item layout_menu[] = {
  {"Array",         1, LAYOUT_ARRAY, 0},
  {"Hierarchy",     1, LAYOUT_TREE,  0},
  {"Circular",      1, LAYOUT_CIRCULAR, 0},
  {0,0,0,0} 
};

/* ---  selection menu  --- */

static menu_item selection_menu[] = {
  {"Node selection", 1, SELECTION_NODES, 0},
  {"Edge selection", 1, SELECTION_EDGES, 0},
  {0,0,0,0} 
};

/* ---  Xlib bookkeeping for the graph drawing functions  --- */

GC the_gc, clear_gc, node_gc, edge_gc, label_gc;
GC highlight_gc, unhighlight_gc;
Drawable the_drawable;
Display *the_display;
Colormap the_colormap;
Pixel the_background;

/* ---  XFontStruct used for calculating width of labels  --- */

XFontStruct *label_font_struct;

/* ---  private function prototypes  --- */

static void createGC(Display *disp, Drawable draw);
static void MainWindowDeleteSelectedEdges(Widget w, XtPointer client_data, XtPointer call_data);
static void MainWindowConnectNodes(Widget w, XtPointer client_data, XtPointer call_data);
static void MainWindowEditEdges(Widget w, XtPointer client_data, XtPointer call_data);
static void MainWindowEditNodes(Widget w, XtPointer client_data, XtPointer call_data);
static void MainWindowDeleteNodes(Widget w, XtPointer client_data, XtPointer call_data);
static void MainWindowCreateNodes(Widget w, XtPointer client_data, XtPointer call_data);

/* ---  resource list  --- */

static XtResource resources[] = {
  {
    XtNforeground, /* general foreground color */
    XtCForeground,
    XtRPixel,
    sizeof(Pixel),
    XtOffset(ApplicationResourcesPtr, default_fg),
    XtRString,
    XtDefaultForeground
  },
  {
    XtNbackground, /* general background color */
    XtCBackground,
    XtRPixel,
    sizeof(Pixel),
    XtOffset(ApplicationResourcesPtr, default_bg),
    XtRString,
    XtDefaultBackground
  },
  {
    NnodeColor,    /* node border color */
    CnodeColor,
    XtRPixel,
    sizeof(Pixel),
    XtOffset(ApplicationResourcesPtr, node_fg),
    XtRString,
    XtDefaultForeground
  },
  {
    NedgeColor,    /* edge line/arrow color */
    CedgeColor,
    XtRPixel,
    sizeof(Pixel),
    XtOffset(ApplicationResourcesPtr, edge_fg),
    XtRString,
    XtDefaultForeground
  },
  {
    NlabelColor,   /* label color */
    ClabelColor,
    XtRPixel,
    sizeof(Pixel),
    XtOffset(ApplicationResourcesPtr, label_fg),
    XtRString,
    XtDefaultForeground
  },
  {
    NlabelFont,    /* label font */
    ClabelFont,
    XtRFont,       /* saved as a Font */
    sizeof(Font),
    XtOffset(ApplicationResourcesPtr, label_font),
    XtRString,
    XtDefaultFont
  },
  {
    NlabelFont,    /* label font again, this time saved as
		      an XFontStruct */
    ClabelFont,
    XtRFontStruct,
    sizeof(XFontStruct),
    XtOffset(ApplicationResourcesPtr, label_font_struct),
    XtRString,
    XtDefaultFont
  }
};

/* ---  actions  --- */

/* set-focus action */
static void HereAction(Widget w, XEvent *event,
		       String *params, Cardinal *num_params)
{
  jumpTo(w);
}

static void PrevAction(Widget w, XEvent *event,
		       String *params, Cardinal *num_params)
{
  jumpPrev(w);
}

static void NextAction(Widget w, XEvent *event,
		       String *params, Cardinal *num_params)
{
  jumpNext(w);
}

static void ConfirmAction(Widget w, XEvent *event,
		       String *params, Cardinal *num_params)
{
  confirmAction(w);
}

/* update the graph display, in response to expose events */
static void RedisplayGraph(Widget w, XEvent *event, 
			   String *params, Cardinal *num_params)
{
  XPoint p1, p2;
/*
  printf("x %d, y %d, w %d, h %d, count %d\n", (*event).xexpose.x ,
	 (*event).xexpose.y,
	 (*event).xexpose.width ,
	 (*event).xexpose.height ,
	 (*event).xexpose.count );
*/
  p1.x=(*event).xexpose.x;
  p1.y=(*event).xexpose.y;
  p2.x=p1.x+(*event).xexpose.width;
  p2.y=p1.y+(*event).xexpose.height;
  
  if( graphBufferGetCurrentGraph() ) {
    displayGraphWithinRectangle(graphBufferGetCurrentGraph(), p1, p2);
  }
}

/* ---  public functions  --- */

/* set main window title */
void mainWindowSetTitle(char *title) {
  if( topLevel != NULL )
    XtVaSetValues(topLevel,
		  XtNtitle, title,
		  NULL);
}

/* clear the graph drawing area */
void mainWindowClearGraphArea() {
  actionsReset(); /* reset any references in the actions module */
  
  /* clear the graph window */
  XClearWindow(the_display, the_drawable);
}

/* create the main window, its buttons and menus, and register
   actions and callback functions */
void mainWindowInitialize(int argc, char **argv)
{
  /* main window widgets */
  Widget vpane, graphViewport;
  Widget buttonbox1, buttonboxNodes, buttonboxEdges;
  
  /* upper buttons */
  Widget fileMenuButton, optionsMenuButton, selectionMenuButton;
  Widget layoutMenuButton;
  Widget helpButton;
  
  /* node buttons */
  Widget nodeDeleteButton, nodeEditButton, nodeCreateButton;
  
  /* edge buttons */
  Widget edgeDeleteButton, edgeEditButton, edgeConnectNodesButton;
  
  /* actions */
  static XtActionsRec window_actions[] = {
    {"node-drag-start", NodeDragStartAction},
    {"node-drag",       NodeDragAction},
    {"node-drag-end",   NodeDragEndAction},

    {"hotspot-set",     HotspotSetAction},

    {"selection-start", SelectionStartAction},
    {"selection-drag",  SelectionDragAction},
    {"selection-add-end", SelectionAddEndAction},
    {"selection-end",   SelectionEndAction},

    {"selection-move-start", NodeSelectionDragStartAction},
    {"selection-move-drag",  NodeSelectionDragAction},
    {"selection-move-add-end", NodeSelectionDragAddEndAction},
    {"selection-move-end",   NodeSelectionDragEndAction},

    {"extension-click-object",    ElkClickAction},
    {"extension-node-drag-start", ElkNodeDragStartAction},
    {"extension-node-drag",       ElkNodeDragAction},

    {"confirm_action", ConfirmAction},
    {"next_action",    NextAction},
    {"prev_action",    PrevAction},
    {"here_action",    HereAction},
    {"expose_action",  RedisplayGraph},
    {"hypertext_linkselect", helpWindowLinkSelection}
#if USE_SCHEME
    , {"prompt-confirm-action", promptWindowConfirmAction}
    , {"prompt-back-action",    promptWindowHistoryBackAction}
#endif
  };

  /* graph area translations */
  String trans = "<Expose>  : expose_action()";

  /* initialize focus control data structure */
  initFocus();
  
  /* initial graph area size */
  graph_width = GRAPH_WINDOW_WIDTH;
  graph_height= GRAPH_WINDOW_HEIGHT;
  
  /* widget creation */
  topLevel = XtVaAppInitialize(
			       &app_context,
			       "Ginger",
			       NULL, 0,
			       &argc, argv,
			       NULL,
			       NULL );

  /* get resources */
  XtVaGetApplicationResources(topLevel, &app_resources,
			      resources,
			      XtNumber(resources),
			      /* no non-user-configurable resources */
			      NULL);
  
  label_font_struct = app_resources.label_font_struct;  

  /* register actions */
  XtAppAddActions(app_context, window_actions, XtNumber(window_actions)); 

  /* more widget creation */
  vpane = XtVaCreateManagedWidget("vpane", panedWidgetClass, topLevel,
				  XtNwidth, MAIN_WINDOW_WIDTH,
				  XtNheight, MAIN_WINDOW_HEIGHT,
				  NULL);

  buttonbox1 = XtVaCreateManagedWidget("buttonbox1", 
				    boxWidgetClass, 
				    vpane, 
				    NULL);

  graphViewport = XtVaCreateManagedWidget(
				    "graphViewport",
				    viewportWidgetClass,
				    vpane,
				    XtNallowVert,  TRUE,
				    XtNallowHoriz, TRUE,
				    XtNuseRight,   FALSE,
				    XtNuseBottom,  TRUE,
				    NULL);
 
   graphArea = XtVaCreateManagedWidget(
                                  "graphArea",
                                  widgetClass,
                                  graphViewport,
				  XtNwidth, graph_width,
				  XtNheight, graph_height,
                                  NULL);

  XtAugmentTranslations(graphArea, XtParseTranslationTable(trans));
  
  buttonboxNodes = XtVaCreateManagedWidget("buttonboxNodes", 
					 boxWidgetClass, 
					 vpane, 
					 XtNmax, BUTTON_PANE_HEIGHT,
					 XtNmin, BUTTON_PANE_HEIGHT, 
					 NULL);
  
  buttonboxEdges = XtVaCreateManagedWidget("buttonboxEdges", 
					 boxWidgetClass, 
					 vpane, 
					 XtNmax, BUTTON_PANE_HEIGHT,
					 XtNmin, BUTTON_PANE_HEIGHT, 
					 NULL);

  messageField = XtVaCreateManagedWidget(
					 "messageField",
					 labelWidgetClass,
					 vpane,
					 XtNwidth, MAIN_WINDOW_WIDTH,
					 XtNjustify, XtJustifyLeft,
					 XtNresize, FALSE,
					 XtNheight, 24,
					 XtNmax, 24,
					 /* XtNmin, 24, */
					 NULL);

  /* buttons */

  fileMenuButton =  XtVaCreateManagedWidget(
					  "fileMenuButton",
					  menuButtonWidgetClass,
					  buttonbox1,
					  XtNmenuName, "fileMenu",
					  XtNlabel, "File",
					  XtNleftBitmap, menuButtonMark(),
					  XtNwidth, MW_BUTTON_WIDTH,
					  NULL);
  
  createMenu("fileMenu", fileMenuButton, file_menu);

  selectionMenuButton =  XtVaCreateManagedWidget(
					  "selectionMenuButton",
					  menuButtonWidgetClass,
					  buttonbox1,
					  XtNmenuName, "selectionMenu",
					  XtNlabel, "Selection",
					  XtNleftBitmap, menuButtonMark(),
					  /* XtNwidth, MW_BUTTON_WIDTH, */
					  NULL);
  
  createMenu("selectionMenu", selectionMenuButton, selection_menu);

  layoutMenuButton =  XtVaCreateManagedWidget(
					  "layoutMenuButton",
					  menuButtonWidgetClass,
					  buttonbox1,
					  XtNmenuName, "layoutMenu",
					  XtNlabel, "Layout",
					  XtNleftBitmap, menuButtonMark(),
					  XtNwidth, MW_BUTTON_WIDTH,
					  NULL);
  
  createMenu("layoutMenu", layoutMenuButton, layout_menu);

  optionsMenuButton =  XtVaCreateManagedWidget(
					       "optionsMenuButton",
					       menuButtonWidgetClass,
					       buttonbox1,
					       XtNmenuName, "optionsMenu",
					       XtNlabel, "Options",
					       XtNleftBitmap, menuButtonMark(),
					       XtNwidth, MW_BUTTON_WIDTH,
					       NULL);
  
  initializeOptionsMenu(optionsMenuButton);

  helpButton = XtVaCreateManagedWidget(
				  "helpButton",
				  commandWidgetClass,
				  buttonbox1,
				  XtNlabel, "Help",
				  XtNwidth, MW_BUTTON_WIDTH,
				  NULL);
  XtAddCallback(helpButton, XtNcallback, helpWindowPopup, 0);

  /* node buttons */

  XtVaCreateManagedWidget(
			  "nodeLabel",
			  labelWidgetClass,
			  buttonboxNodes,
			  XtNjustify, XtJustifyLeft,
			  XtNlabel, "Nodes",
			  XtNwidth, 55,
			  NULL);

  nodeCreateButton = XtVaCreateManagedWidget(
				  "nodeCreateButton",
				  commandWidgetClass,
				  buttonboxNodes,
				  XtNlabel, "Create",
				  XtNwidth, MW_NODE_EDGE_BUTTON_WIDTH,
				  NULL);
  XtAddCallback(nodeCreateButton, XtNcallback, MainWindowCreateNodes, 0);
  
  nodeEditButton = XtVaCreateManagedWidget(
				  "nodeEditButton",
				  commandWidgetClass,
				  buttonboxNodes,
				  XtNlabel, "Edit",
				  XtNwidth, MW_NODE_EDGE_BUTTON_WIDTH,
				  NULL);
  XtAddCallback(nodeEditButton, XtNcallback, MainWindowEditNodes, 0);

  nodeDeleteButton = XtVaCreateManagedWidget(
				  "nodeDeleteButton",
				  commandWidgetClass,
				  buttonboxNodes,
				  XtNlabel, "Delete",
				  XtNwidth, MW_NODE_EDGE_BUTTON_WIDTH,
				  NULL);
  XtAddCallback(nodeDeleteButton, XtNcallback, MainWindowDeleteNodes, 0);

  /* edge buttons */

  XtVaCreateManagedWidget(
			  "edgeLabel",
			  labelWidgetClass,
			  buttonboxEdges,
			  XtNjustify, XtJustifyLeft,
			  XtNlabel, "Edges",
			  XtNwidth, 55,
			  NULL);

  edgeConnectNodesButton = XtVaCreateManagedWidget(
				  "edgeConnectNodesButton",
				  commandWidgetClass,
				  buttonboxEdges,
				  XtNlabel, "Connect nodes",
				  XtNwidth, MW_NODE_EDGE_BUTTON_WIDTH,
				  NULL);

  XtAddCallback(edgeConnectNodesButton, XtNcallback, MainWindowConnectNodes, 0);

  edgeEditButton = XtVaCreateManagedWidget(
				  "edgeEditButton",
				  commandWidgetClass,
				  buttonboxEdges,
				  XtNlabel, "Edit",
				  XtNwidth, MW_NODE_EDGE_BUTTON_WIDTH,
				  NULL);
  XtAddCallback(edgeEditButton, XtNcallback, MainWindowEditEdges, 0);

  edgeDeleteButton = XtVaCreateManagedWidget(
				  "edgeDeleteButton",
				  commandWidgetClass,
				  buttonboxEdges,
				  XtNlabel, "Delete",
				  XtNwidth, MW_NODE_EDGE_BUTTON_WIDTH,
				  NULL);
  XtAddCallback(edgeDeleteButton, XtNcallback, MainWindowDeleteSelectedEdges, 0);


  /* create program icon */
  createIcon(topLevel); 
  
  XtRealizeWidget(topLevel);

  the_display = XtDisplay(graphArea);
  the_drawable = XtWindow(graphArea);
  the_colormap = XDefaultColormap(the_display, 0);
  
  createGC(the_display, the_drawable);
}

/* hand-over control to the X Toolkit */
void mainWindowXtLoop(void) {
  XtAppMainLoop(app_context);
}

/* update the size of the graph area.
   If the area given is smaller than the default size,
   the default size is used instead. */
void mainWindowSetGraphAreaSize(int new_width, int new_height) {
  int old_w, old_h;

  old_w=graph_width;
  old_h=graph_height;

  graph_width = MAX(new_width,  GRAPH_WINDOW_WIDTH);
  graph_height= MAX(new_height, GRAPH_WINDOW_HEIGHT);

  if( old_w!=graph_width || old_h!=graph_height ) {
    XtVaSetValues(graphArea,
		  XtNwidth,  graph_width,
		  XtNheight, graph_height,
		  NULL);
  }
}

/* display a message in the message field */
void mainWindowShowMessage(char *msg) {
  XtVaSetValues(messageField,
		XtNlabel, msg,
		NULL);
}

/* ---  private functions  --- */

/* callback for nodes.edit button */
static void MainWindowEditNodes(Widget w, XtPointer client_data, XtPointer call_data) { 
  nodeAttributeWindowEdit();
}

/* callback for nodes.delete buttom */
static void MainWindowDeleteNodes(Widget w, XtPointer client_data, XtPointer call_data) {
  int i;
  
  i=graphDisplayDeleteSelectedNodes(graphBufferGetCurrentGraph());  
  
  if( i==0 ) {
    notifyAndOfferHelp("At least one node should be selected first.", "delete");
  }
}

/* callback for nodes.create buttom */
static void MainWindowCreateNodes(Widget w, XtPointer client_data, XtPointer call_data) {
  nodeAttributeWindowCreate();
}

/* callback for edges.delete button */
static void MainWindowDeleteSelectedEdges(Widget w, XtPointer client_data, XtPointer call_data) {
  int i;

  i=graphDisplayDeleteSelectedEdges(graphBufferGetCurrentGraph());

  if( i==0 ) {
    notifyAndOfferHelp("At least one edge should be selected first.", "delete ");
  }
}

/* callback for edges.connect button */
static void MainWindowConnectNodes(Widget w, XtPointer client_data, XtPointer call_data) {
  int i;

  i=connectSelectedNodes(graphBufferGetCurrentGraph());

  if( i==0 ) {
    notifyAndOfferHelp("At least two unconnected nodes should be selected first.", "connect nodes");
  }
}

/* callback for edges.edit button */
static void MainWindowEditEdges(Widget w, XtPointer client_data, XtPointer call_data) {
  int n;
  edge *e;

  n=edgeSelectCount(graphBufferGetCurrentGraph());

  if( n==0 ) {
    notifyAndOfferHelp("At least one edge should be selected first.", "edit ");
    return;
  }
  if( n==1 ) {
    e=getFirstSelectedEdge(graphBufferGetCurrentGraph());
    if(e)
      edgeAttributeWindowDisplayEdge(e);
  }
  else
    edgeAttributeWindowReset();
  
  edgeAttributeWindowPopup(0,0,0);
}

/* create various Graphics Contexts (GCs) used with
   Xlib drawing functions */
static void createGC(Display *disp, Drawable draw) {
  XGCValues values;

  /* GC with default fore- and background */
  values.foreground = app_resources.default_fg;
  the_background = values.background = app_resources.default_bg;
  
  the_gc = XCreateGC(disp, draw,
		     GCForeground | GCBackground, &values);

  /* GC with same fore- and background color; used for clearing
     objects from the display */
  values.foreground = app_resources.default_bg;
  values.background = app_resources.default_bg;
  
  clear_gc = XCreateGC(disp, draw,
		       GCForeground | GCBackground, &values);

  /* GC for drawing nodes */
  values.foreground = app_resources.node_fg;
  values.background = app_resources.default_bg;
  
  node_gc = XCreateGC(disp, draw,
		      GCForeground | GCBackground, &values);

  /* GC for drawing edges */
  values.foreground = app_resources.edge_fg;
  values.background = app_resources.default_bg;
  
  edge_gc = XCreateGC(disp, draw,
		      GCForeground | GCBackground, &values);

  /* GC for drawing labels */
  values.foreground = app_resources.label_fg;
  values.background = app_resources.default_bg;
  values.font       = app_resources.label_font;

  label_gc = XCreateGC(disp, draw,
		       GCForeground | GCBackground | GCFont, &values);

  /* GC for highlighting */
  values.foreground = app_resources.default_fg;
  values.background = app_resources.default_bg;
  values.line_width = NODE_HIGHLIGHT_WIDTH;

  highlight_gc = XCreateGC(disp, draw,
		   GCForeground | GCBackground | GCLineWidth, &values);

  /* GC for un-highlighting */
  values.foreground = app_resources.default_bg;
  values.background = app_resources.default_bg;
  values.line_width = NODE_HIGHLIGHT_WIDTH;

  unhighlight_gc = XCreateGC(disp, draw,
		   GCForeground | GCBackground | GCLineWidth, &values);

}
