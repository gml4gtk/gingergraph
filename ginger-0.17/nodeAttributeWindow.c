/* $Id: nodeAttributeWindow.c,v 1.7 1996/06/14 12:19:29 lupus Exp $ */

/***************************************************************
 * This file is part of the Ginger project.                    *
 * Created 12-02-1996                                          *
 * Niels C. Larsen, Aalborg University, Denmark                *
 * (lupus@iesd.auc.dk)                                         *
 * Martin K. Molz, Aalborg University, Denmark                 *
 * (molz@iesd.auc.dk)                                          *
 ***************************************************************/

/* pop-up window for editing node attributes */

#include <X11/Intrinsic.h>
#include <X11/StringDefs.h>
#include <X11/Shell.h>

#include <X11/Xaw/Paned.h>
#include <X11/Xaw/Form.h>
#include <X11/Xaw/Box.h>
#include <X11/Xaw/Command.h>
#include <X11/Xaw/Text.h>
#include <X11/Xaw/AsciiText.h>

#include <string.h>
#include <stdlib.h>
#include "nodeAttributeWindow.h"
#include "graphDisplay.h"
#include "graphBuffer.h"
#include "focus.h"
#include "menu.h"
#include "menuOpcodes.h"
#include "str.h"
#include "nodeSelection.h"
#include "nodeSelectionWindow.h"
#include "allocmem.h"
#include "notify.h"

/* ---  private declarations  --- */

extern Widget topLevel;  /* from mainWindow.c */

#define BUTTON_WIDTH 65
#define LABEL_WIDTH 48
#define LARGE_LABEL_WIDTH 80

#define NA_EDIT 5
#define NA_CREATE 10

static int popup=0;
static int windowMode;  /* NA_EDIT or NA_CREATE */
static Widget nodeAttributeWindowShell;
static Widget applyCreate; /* button to edit and create nodes */

/* input fields */
static Widget nodeNameInput;
static Widget nodeLabelInput;
static Widget nodeXInput, nodeYInput, nodeWidthInput, nodeHeightInput;
static Widget nodeFGColorInput, nodeBGColorInput;
static Widget nodeEditorInput, nodeContentsInput;
static Widget nodeIconInput;

/* node shape menu */
static Widget shapeMenuButton;
static menu_item shape_menu[] = {
  {"Rectangle",    1, NODE_SHAPE_MENU_RECTANGLE, 0},
  {"Rhomb",        1, NODE_SHAPE_MENU_RHOMBE, 0},
  {"Ellipse",      1, NODE_SHAPE_MENU_ELLIPSE, 0},
  {"None",         1, NODE_SHAPE_MENU_NONE, 0},
  {0,0,0,0} 
};
static int current_selection = NODE_SHAPE_MENU_RECTANGLE;

/* private functions prototypes */
static void createNodeAttributeWindowWidgets();
static void NodeAttributeWindowClose(Widget w, XtPointer client_data, XtPointer call_data);
static void NodeAttributeWindowEditSelectedNodes(Widget w, XtPointer client_data, XtPointer call_data);
static void nodeAttributeSetTitle(char *title);
static void applyCreateButtonSetLabel(char *label);

/* ---  public functions  --- */

void nodeAttributeWindowEdit() {
  int n;
  node *the_node;
  
  n=nodeSelectCount(graphBufferGetCurrentGraph());
  
  if( n==0 ) {
    notifyAndOfferHelp("At least one node should be selected first.", "edit");
    return;
  }
  if( n==1 ) {
    the_node=getFirstSelectedNode(graphBufferGetCurrentGraph());
    if(the_node)
      nodeAttributeWindowDisplayNode(the_node);
  }
  else
    nodeAttributeWindowReset();

  windowMode=NA_EDIT;
  nodeAttributeSetTitle("Edit node attributes");
  applyCreateButtonSetLabel("Apply");
  nodeAttributeWindowPopup(0,0,0);
}

void nodeAttributeWindowCreate() {
  int n;
  node *the_node;
  
  n=nodeSelectCount(graphBufferGetCurrentGraph());
  
  if( n==1 ) {
    the_node=getFirstSelectedNode(graphBufferGetCurrentGraph());
    if(the_node)
      nodeAttributeWindowDisplayNode(the_node);
  }
/*  else
    nodeAttributeWindowReset();
*/
  windowMode=NA_CREATE;
  nodeAttributeSetTitle("Create node");
  applyCreateButtonSetLabel("Create");
  nodeAttributeWindowPopup(0,0,0);
}

void nodeAttributeWindowPopup(Widget w, XtPointer client_data, XtPointer call_data) {
  if( !popup ) {
    createNodeAttributeWindowWidgets();
    
    XtPopup(nodeAttributeWindowShell, XtGrabNone);
    popup=1;
  }
}

void nodeAttributeWindowSetName(char *s) {
  createNodeAttributeWindowWidgets(); /* make sure widgets are initialized */
  
  XtVaSetValues(nodeNameInput,
                XtNstring, s,
                NULL);
}

void nodeAttributeWindowSetLabel(char *s) {
  createNodeAttributeWindowWidgets(); /* make sure widgets are initialized */
  
  XtVaSetValues(nodeLabelInput,
                XtNstring, s,
                NULL);
}

void nodeAttributeWindowSetX(int x) {
  char buf[40];

  createNodeAttributeWindowWidgets(); /* make sure widgets are initialized */
  
  sprintf(buf, "%d", x);

  XtVaSetValues(nodeXInput,
                XtNstring, buf,
                NULL);
}

void nodeAttributeWindowSetY(int y) {
  char buf[40];

  createNodeAttributeWindowWidgets(); /* make sure widgets are initialized */
  
  sprintf(buf, "%d", y);

  XtVaSetValues(nodeYInput,
                XtNstring, buf,
                NULL);
}

void nodeAttributeWindowSetWidth(int width) {
  char buf[40];

  createNodeAttributeWindowWidgets(); /* make sure widgets are initialized */
  
  sprintf(buf, "%d", width);

  XtVaSetValues(nodeWidthInput,
                XtNstring, buf,
                NULL);
}

void nodeAttributeWindowSetHeight(int height) {
  char buf[40];

  createNodeAttributeWindowWidgets(); /* make sure widgets are initialized */
  
  sprintf(buf, "%d", height);

  XtVaSetValues(nodeHeightInput,
                XtNstring, buf,
                NULL);
}

void nodeAttributeWindowSetFGColor(char *s) {
  createNodeAttributeWindowWidgets(); /* make sure widgets are initialized */
  
  XtVaSetValues(nodeFGColorInput,
                XtNstring, s,
                NULL);
}

void nodeAttributeWindowSetBGColor(char *s) {
  createNodeAttributeWindowWidgets(); /* make sure widgets are initialized */
  
  XtVaSetValues(nodeBGColorInput,
                XtNstring, s,
                NULL);
}

void nodeAttributeWindowSetEditor(char *s) {
  createNodeAttributeWindowWidgets(); /* make sure widgets are initialized */
  
  XtVaSetValues(nodeEditorInput,
                XtNstring, s,
                NULL);
}

void nodeAttributeWindowSetContents(char *s) {
  createNodeAttributeWindowWidgets(); /* make sure widgets are initialized */
  
  XtVaSetValues(nodeContentsInput,
                XtNstring, s,
                NULL);
}

void nodeAttributeWindowSetIcon(char *s) {
  createNodeAttributeWindowWidgets(); /* make sure widgets are initialized */
  
  XtVaSetValues(nodeIconInput,
                XtNstring, s,
                NULL);
}

void nodeAttributeWindowSetShape(int opcode) {
  char *s;
  createNodeAttributeWindowWidgets(); /* make sure widgets are initialized */
  
  current_selection = opcode;

  switch(opcode) {
  case NODE_SHAPE_MENU_RECTANGLE:
    s="Rectangle";
    break;
  case NODE_SHAPE_MENU_RHOMBE:
    s="Rhomb";
    break;
  case NODE_SHAPE_MENU_ELLIPSE:
    s="Ellipse";
    break;
  case NODE_SHAPE_MENU_NONE:
    s="None";
    break;
  }
  XtVaSetValues(shapeMenuButton,
                XtNlabel, s,
		XtNwidth, 130,
                NULL);
}

/* display the attributes of n in the window */
void nodeAttributeWindowDisplayNode(node *n) {
  int shape;
  
  nodeAttributeWindowSetName(n->name);
  nodeAttributeWindowSetLabel(n->label);
  nodeAttributeWindowSetX(n->x);
  nodeAttributeWindowSetY(n->y);
  nodeAttributeWindowSetWidth(n->width);
  nodeAttributeWindowSetHeight(n->height);
  
  switch(n->shape) {
  case SHAPE_RECTANGLE:
    shape = NODE_SHAPE_MENU_RECTANGLE;
    break;
  case SHAPE_RHOMBE:
    shape = NODE_SHAPE_MENU_RHOMBE;
    break;
  case SHAPE_ELLIPSE:
    shape = NODE_SHAPE_MENU_ELLIPSE;
    break;
  case SHAPE_NONE:
    shape = NODE_SHAPE_MENU_NONE;
    break;
  }
  nodeAttributeWindowSetShape(shape);

  if( n->foreground.is_used ) {
    nodeAttributeWindowSetFGColor(n->foreground.color_name);
  }
  else
    nodeAttributeWindowSetFGColor("");
  
  if( n->background.is_used ) {
    nodeAttributeWindowSetBGColor(n->background.color_name);
  }
  else
    nodeAttributeWindowSetBGColor("");
  
  if(n->editor)
    nodeAttributeWindowSetEditor(n->editor);
  else
    nodeAttributeWindowSetEditor("");

  if(n->contents)
    nodeAttributeWindowSetContents(n->contents);
  else
    nodeAttributeWindowSetContents("");
  
  nodeAttributeWindowSetIcon(n->icon.path);
}

/* apply the attributes given in the window to n */
void nodeAttributeWindowApplyToNode(node *n) {
  String p;
  int shape, should_redraw;
  int old_w, old_h;
  int icon_type;
  int new_x, new_y;
  node *n1;
  char buf[256];

  createNodeAttributeWindowWidgets(); /* make sure widgets are initialized */
  should_redraw = 0;
  old_w=n->width;
  old_h=n->height;
  new_x=n->x;
  new_y=n->y;

  undisplayNodeImageOnScreen(n);
  
  XtVaGetValues(nodeNameInput, XtNstring, &p, NULL);
  if( strlen(p)>0 ) {
    if( (n1=graphFindNodeNamed(graphBufferGetCurrentGraph(), p)) ) {
      if( n1!=n ) {
	sprintf(buf, "Node %s already exists.", p);
	notifyAndOfferHelp(buf, "node already exists");
      }
    }
    else
      nodeSetName(n,p);
  }
  
  XtVaGetValues(nodeXInput, XtNstring, &p, NULL);
  if( strlen(p)>0 ) {
    should_redraw = 1;
    new_x=atoi(p);
  }

  XtVaGetValues(nodeYInput, XtNstring, &p, NULL);
  if( strlen(p)>0 ) {
    should_redraw = 1;
    new_y=atoi(p);
  }

  XtVaGetValues(nodeWidthInput, XtNstring, &p, NULL);
  if( strlen(p)>0 ) {
    should_redraw = 1;
    n->width=atoi(p);
  }
  
  XtVaGetValues(nodeHeightInput, XtNstring, &p, NULL);
  if( strlen(p)>0 ) {
    should_redraw = 1;
    n->height=atoi(p);
  }
  
  XtVaGetValues(nodeLabelInput, XtNstring, &p, NULL);
  if( strlen(p)>0 ) {
    should_redraw = 1;
    nodeSetLabel(n,p);
  }

  switch(current_selection) {
  case NODE_SHAPE_MENU_RECTANGLE:
    shape = SHAPE_RECTANGLE;
    break;
  case NODE_SHAPE_MENU_RHOMBE:
    shape = SHAPE_RHOMBE;
    break;
  case NODE_SHAPE_MENU_ELLIPSE:
    shape = SHAPE_ELLIPSE;
    break;
  case NODE_SHAPE_MENU_NONE:
    shape = SHAPE_NONE;
    break;
  }
  
  if( shape!=n->shape ) {
    should_redraw=1;
    n->shape=shape;
  }
  
  XtVaGetValues(nodeFGColorInput, XtNstring, &p,NULL);
  if( strlen(p)>0 ) {
    nodeSetForeground(n,p);
    should_redraw=1;
  }
  
  XtVaGetValues(nodeBGColorInput, XtNstring, &p,NULL);
  if( strlen(p)>0 ) {
    nodeSetBackground(n,p);
    should_redraw=1;
  }
  
  XtVaGetValues(nodeEditorInput, XtNstring, &p,NULL);
  if( strlen(p)>0 ) {
    nodeSetEditor(n,p);
  }
  
  XtVaGetValues(nodeContentsInput, XtNstring, &p,NULL);
  if( strlen(p)>0 ) {
    nodeSetContents(n,p);
  }

  XtVaGetValues(nodeIconInput, XtNstring, &p,NULL);
  if( strlen(p)>0 ) {
    icon_type=ICON_BITMAP;      /* assume bitmap icon as default   */
    if(strstr(p,".xpm"))        /* if the filename contains ".xpm" */
      icon_type=ICON_PIXMAP;    /*  then assume it's a pixmap icon */
    nodeSetIcon(n,icon_type,p);
  }

  /* if the node size has changed, we have to re-allocate the
     node image pixmap */
  if( n->width!=old_w || n->height!=old_h ) {
    if( n->width<=0 ) 
      n->width = DEFAULT_NODE_WIDTH;
    if( n->height<=0 ) 
      n->height = DEFAULT_NODE_HEIGHT;
    nodeReallocImage(n);
  }
  
  makeNodeImage(n);
  moveNode(graphBufferGetCurrentGraph(), n, new_x, new_y);
}

/* reset window, i.e. clear input fields and reset shape menu */
void nodeAttributeWindowReset() {
  nodeAttributeWindowSetName("");
  nodeAttributeWindowSetLabel("");

  XtVaSetValues(nodeXInput, XtNstring, "", NULL);
  XtVaSetValues(nodeYInput, XtNstring, "", NULL);
  XtVaSetValues(nodeWidthInput, XtNstring, "", NULL);
  XtVaSetValues(nodeHeightInput, XtNstring, "", NULL);
  
  nodeAttributeWindowSetEditor("");
  nodeAttributeWindowSetContents("");
  nodeAttributeWindowSetIcon("");
  
  nodeAttributeWindowSetFGColor("");
  nodeAttributeWindowSetBGColor("");
  nodeAttributeWindowSetShape(NODE_SHAPE_MENU_RECTANGLE);
}

void nodeAttributeWindowHotspotChange(int x, int y) {
  if( popup ) {
    nodeAttributeWindowSetX(x);
    nodeAttributeWindowSetY(y);
  }
}

/* ---  private functions  --- */

/* apply the user input in node attribute window to all selected nodes */
static void NodeAttributeWindowEditSelectedNodes(Widget w, XtPointer client_data, XtPointer call_data) {
  int i;
  graph *g;
  node *n, *nInGraph;
  String p;
  char *comment1="Node with name ";
  char *comment2=" allready exist!";
  char *message;
  char nameBuf[256];
  char *name;
  
  switch(windowMode) {
  case NA_EDIT:
    g=graphBufferGetCurrentGraph();
    
    for(i=1; i<=getsize(g->nodes); i++) {
      n=(node *)retrieve(i,g->nodes);
      if( n->selected ) {
	nodeAttributeWindowApplyToNode(n);
      }
    }
    break;
  case NA_CREATE:
    g=graphBufferGetCurrentGraph();
    XtVaGetValues(nodeNameInput, XtNstring, &p, NULL);
    
    if( strlen(p)==0 ) {
      name=createNewNodeName(g,nameBuf);
    } else
      name=p;
    
    if( graphFindNodeNamed(g,name)!=0 ) {
      message=(char *)AllocateMemory(strlen(comment1)+strlen(comment2)+strlen(p)+100);
      strcpy(message, comment1);
      strcat(message, name);
      strcat(message, comment2);
      notifyAndOfferHelp(message, "node already exists");
      free(message);
    }
    else {
      n=createNode(name,"",SHAPE_DEFAULT,-1,-1,0,0,ICON_NONE,"");
      nInGraph=addNode(g,n);
      freeNode(n);
      nodeAttributeWindowApplyToNode(nInGraph);
      nodeSelect(g, nInGraph, nodeSelectionWindowGetAccumulationState());
    }
    break;
  }
}

static void NodeAttributeWindowClose(Widget w, XtPointer client_data, XtPointer call_data) {
  XtPopdown(nodeAttributeWindowShell);
  popup=0;
}

static void NodeAttributeWindowClear(Widget w, XtPointer client_data, XtPointer call_data) {
  nodeAttributeWindowReset();
}

static void createNodeAttributeWindowWidgets() {
  static int done=0;
  
  Widget nodeAttributeWindowPane, nodeAttributeWindowForm;
  Widget nodeAttributeWindowBox;

  /* buttons */
  Widget close, clear;
  
  /* labels */
  Widget nodeNameLabel, nodeLabelLabel;
  Widget nodeXLabel, nodeYLabel, nodeWidthLabel, nodeHeightLabel;
  Widget nodeShapeLabel;
  Widget nodeFGColorLabel, nodeBGColorLabel;
  Widget nodeEditorLabel, nodeContentsLabel;
  Widget nodeIconLabel;

  if(done)
    return;
  done=1;

  nodeAttributeWindowShell = XtVaCreatePopupShell (
				      "nodeAttributeWindowShell",
				      transientShellWidgetClass,
				      topLevel,
				      XtNtitle, "Node attributes",
				      NULL );
  
  nodeAttributeWindowPane = XtVaCreateManagedWidget(
				      "nodeAttributeWindowPane",
				      panedWidgetClass,
				      nodeAttributeWindowShell,
				      NULL);
  
  nodeAttributeWindowForm = XtVaCreateManagedWidget(
				      "nodeAttributeWindowForm",
				      formWidgetClass,
				      nodeAttributeWindowPane,
				      NULL);

  nodeNameLabel = XtVaCreateManagedWidget(
				      "nodeNameLabel",
				      labelWidgetClass,
                                      nodeAttributeWindowForm,
                                      XtNlabel, "Name:",
				      XtNwidth, LABEL_WIDTH,
				      XtNjustify, XtJustifyLeft,
                                      NULL);

  nodeNameInput = XtVaCreateManagedWidget(
			              "nodeNameInput",
				      asciiTextWidgetClass,
				      nodeAttributeWindowForm,
				      XtNstring, "",
				      XtNeditType, XawtextEdit,
				      XtNwidth, 160,
				      XtNfromHoriz, nodeNameLabel,
				      NULL);

  nodeLabelLabel = XtVaCreateManagedWidget(
				      "nodeLabelLabel",
				      labelWidgetClass,
                                      nodeAttributeWindowForm,
                                      XtNlabel, "Label:",
				      XtNwidth, LABEL_WIDTH,
				      XtNjustify, XtJustifyLeft,
				      XtNfromVert, nodeNameLabel,
                                      NULL);

  nodeLabelInput = XtVaCreateManagedWidget(
			              "nodeLabelInput",
				      asciiTextWidgetClass,
				      nodeAttributeWindowForm,
				      XtNstring, "",
				      XtNeditType, XawtextEdit,
				      XtNwidth, 160,
				      XtNfromHoriz, nodeLabelLabel,
				      XtNfromVert, nodeNameLabel,
				      NULL);

  nodeXLabel = XtVaCreateManagedWidget(
				      "nodeXLabel",
				      labelWidgetClass,
                                      nodeAttributeWindowForm,
                                      XtNlabel, "x:",
				      XtNwidth, LABEL_WIDTH,
				      XtNjustify, XtJustifyLeft,
				      XtNfromVert, nodeLabelLabel,
                                      NULL);

  nodeXInput = XtVaCreateManagedWidget(
			              "nodeXInput",
				      asciiTextWidgetClass,
				      nodeAttributeWindowForm,
				      XtNstring, "",
				      XtNeditType, XawtextEdit,
				      XtNwidth, LABEL_WIDTH,
				      XtNfromHoriz, nodeXLabel,
				      XtNfromVert, nodeLabelLabel,
				      NULL);

  nodeWidthLabel = XtVaCreateManagedWidget(
				      "nodeWidthLabel",
				      labelWidgetClass,
                                      nodeAttributeWindowForm,
                                      XtNlabel, "Width:",
				      XtNwidth, LABEL_WIDTH,
				      XtNjustify, XtJustifyLeft,
				      XtNfromHoriz, nodeXInput,
				      XtNfromVert, nodeLabelLabel,
                                      NULL);

  nodeWidthInput = XtVaCreateManagedWidget(
			              "nodeWidthInput",
				      asciiTextWidgetClass,
				      nodeAttributeWindowForm,
				      XtNstring, "",
				      XtNeditType, XawtextEdit,
				      XtNwidth, LABEL_WIDTH,
				      XtNfromHoriz, nodeWidthLabel,
				      XtNfromVert, nodeLabelLabel,
				      NULL);

  nodeYLabel = XtVaCreateManagedWidget(
				      "nodeYLabel",
				      labelWidgetClass,
                                      nodeAttributeWindowForm,
                                      XtNlabel, "y:",
				      XtNwidth, LABEL_WIDTH,
				      XtNjustify, XtJustifyLeft,
				      XtNfromVert, nodeXLabel,
                                      NULL);

  nodeYInput = XtVaCreateManagedWidget(
			              "nodeYInput",
				      asciiTextWidgetClass,
				      nodeAttributeWindowForm,
				      XtNstring, "",
				      XtNeditType, XawtextEdit,
				      XtNwidth, LABEL_WIDTH,
				      XtNfromHoriz, nodeYLabel,
				      XtNfromVert, nodeXLabel,
				      NULL);

  nodeHeightLabel = XtVaCreateManagedWidget(
				      "nodeHeightLabel",
				      labelWidgetClass,
                                      nodeAttributeWindowForm,
                                      XtNlabel, "Height:",
				      XtNwidth, LABEL_WIDTH,
				      XtNjustify, XtJustifyLeft,
				      XtNfromVert, nodeXLabel,
				      XtNfromHoriz, nodeYInput,
                                      NULL);

  nodeHeightInput = XtVaCreateManagedWidget(
			              "nodeHeightInput",
				      asciiTextWidgetClass,
				      nodeAttributeWindowForm,
				      XtNstring, "",
				      XtNeditType, XawtextEdit,
				      XtNwidth, LABEL_WIDTH,
				      XtNfromHoriz, nodeHeightLabel,
				      XtNfromVert, nodeXLabel,
				      NULL);

  nodeShapeLabel = XtVaCreateManagedWidget(
				      "nodeShapeLabel",
				      labelWidgetClass,
                                      nodeAttributeWindowForm,
                                      XtNlabel, "Shape:",
				      XtNwidth, LARGE_LABEL_WIDTH,
				      XtNjustify, XtJustifyLeft,
				      XtNfromVert, nodeYLabel,
                                      NULL);

  shapeMenuButton =  XtVaCreateManagedWidget(
				      "shapeMenuButton",
				      menuButtonWidgetClass,
				      nodeAttributeWindowForm,
				      XtNmenuName, "nodeShapeMenu",
				      XtNlabel, "Rectangle",
				      XtNleftBitmap, menuButtonMark(),
				      XtNwidth, 130,
				      XtNfromVert, nodeYLabel,
				      XtNfromHoriz, nodeShapeLabel,
				      NULL);
  
  createMenu("nodeShapeMenu", shapeMenuButton, shape_menu);

  nodeFGColorLabel = XtVaCreateManagedWidget(
				      "nodeFGColorLabel",
				      labelWidgetClass,
                                      nodeAttributeWindowForm,
                                      XtNlabel, "Foreground:",
				      XtNwidth, LARGE_LABEL_WIDTH,
				      XtNjustify, XtJustifyLeft,
				      XtNfromVert, nodeShapeLabel,
                                      NULL);

  nodeFGColorInput = XtVaCreateManagedWidget(
			              "nodeFGColorInput",
				      asciiTextWidgetClass,
				      nodeAttributeWindowForm,
				      XtNstring, "",
				      XtNeditType, XawtextEdit,
				      XtNwidth, 130,
				      XtNfromHoriz, nodeFGColorLabel,
				      XtNfromVert,  nodeShapeLabel,
				      NULL);

  nodeBGColorLabel = XtVaCreateManagedWidget(
				      "nodeBGColorLabel",
				      labelWidgetClass,
                                      nodeAttributeWindowForm,
                                      XtNlabel, "Background:",
				      XtNwidth, LARGE_LABEL_WIDTH,
				      XtNjustify, XtJustifyLeft,
				      XtNfromVert, nodeFGColorLabel,
                                      NULL);

  nodeBGColorInput = XtVaCreateManagedWidget(
			              "nodeFGColorInput",
				      asciiTextWidgetClass,
				      nodeAttributeWindowForm,
				      XtNstring, "",
				      XtNeditType, XawtextEdit,
				      XtNwidth, 130,
				      XtNfromHoriz, nodeBGColorLabel,
				      XtNfromVert,  nodeFGColorLabel,
				      NULL);

  nodeEditorLabel = XtVaCreateManagedWidget(
				      "nodeEditorLabel",
				      labelWidgetClass,
                                      nodeAttributeWindowForm,
                                      XtNlabel, "Editor:",
				      XtNwidth, LARGE_LABEL_WIDTH,
				      XtNjustify, XtJustifyLeft,
				      XtNfromVert, nodeBGColorLabel,
                                      NULL);

  nodeEditorInput = XtVaCreateManagedWidget(
			              "nodeEditorInput",
				      asciiTextWidgetClass,
				      nodeAttributeWindowForm,
				      XtNstring, "",
				      XtNeditType, XawtextEdit,
				      XtNwidth, 130,
				      XtNfromHoriz, nodeEditorLabel,
				      XtNfromVert,  nodeBGColorLabel,
				      NULL);

  nodeContentsLabel = XtVaCreateManagedWidget(
				      "nodeContentsLabel",
				      labelWidgetClass,
                                      nodeAttributeWindowForm,
                                      XtNlabel, "Contents:",
				      XtNwidth, LARGE_LABEL_WIDTH,
				      XtNjustify, XtJustifyLeft,
				      XtNfromVert, nodeEditorLabel,
                                      NULL);

  nodeContentsInput = XtVaCreateManagedWidget(
			              "nodeContentsInput",
				      asciiTextWidgetClass,
				      nodeAttributeWindowForm,
				      XtNstring, "",
				      XtNeditType, XawtextEdit,
				      XtNwidth, 130,
				      XtNfromHoriz, nodeContentsLabel,
				      XtNfromVert,  nodeEditorLabel,
				      NULL);

  nodeIconLabel = XtVaCreateManagedWidget(
				      "nodeIconLabel",
				      labelWidgetClass,
                                      nodeAttributeWindowForm,
                                      XtNlabel, "Icon file:",
				      XtNwidth, LARGE_LABEL_WIDTH,
				      XtNjustify, XtJustifyLeft,
				      XtNfromVert, nodeContentsLabel,
                                      NULL);

  nodeIconInput = XtVaCreateManagedWidget(
			              "nodeIconInput",
				      asciiTextWidgetClass,
				      nodeAttributeWindowForm,
				      XtNstring, "",
				      XtNeditType, XawtextEdit,
				      XtNwidth, 130,
				      XtNfromHoriz, nodeIconLabel,
				      XtNfromVert,  nodeContentsLabel,
				      NULL);

  nodeAttributeWindowBox = XtVaCreateManagedWidget(
				      "nodeAttributeWindowBox",
				      boxWidgetClass,
				      nodeAttributeWindowPane,
				      NULL);

  applyCreate = XtVaCreateManagedWidget(
				      "nodeAttributeWindowApply",
				      commandWidgetClass,
				      nodeAttributeWindowBox,
				      XtNlabel, "Apply",
				      XtNwidth, BUTTON_WIDTH, 
				      NULL);
  XtAddCallback(applyCreate, XtNcallback, NodeAttributeWindowEditSelectedNodes, topLevel);

  clear = XtVaCreateManagedWidget(
				      "nodeAttributeWindowClear",
				      commandWidgetClass,
				      nodeAttributeWindowBox,
				      XtNlabel, "Clear",
				      XtNwidth, BUTTON_WIDTH, 
				      NULL);
  XtAddCallback(clear, XtNcallback, NodeAttributeWindowClear, topLevel);

  close = XtVaCreateManagedWidget(
				      "nodeAttributeWindowClose",
				      commandWidgetClass,
				      nodeAttributeWindowBox,
				      XtNlabel, "Close",
				      XtNwidth, BUTTON_WIDTH, 
				      NULL);

  XtAddCallback(close, XtNcallback, NodeAttributeWindowClose, topLevel);

  saveFocusInfo(nodeNameInput, nodeLabelInput,
		nodeAttributeWindowPane,
		applyCreate, 1);
  saveFocusInfo(nodeLabelInput, nodeXInput, 
		nodeAttributeWindowPane,
		applyCreate, 0);
  saveFocusInfo(nodeXInput, nodeYInput, 
		nodeAttributeWindowPane,
		applyCreate, 0);
  saveFocusInfo(nodeYInput, nodeWidthInput, 
		nodeAttributeWindowPane,
		applyCreate, 0);
  saveFocusInfo(nodeWidthInput,	nodeHeightInput, 
		nodeAttributeWindowPane,
		applyCreate, 0);
  saveFocusInfo(nodeHeightInput, nodeFGColorInput, 
		nodeAttributeWindowPane,
		applyCreate, 0);
  saveFocusInfo(nodeFGColorInput, nodeBGColorInput, 
		nodeAttributeWindowPane,
		applyCreate, 0);
  saveFocusInfo(nodeBGColorInput, nodeEditorInput, 
		nodeAttributeWindowPane,
		applyCreate, 0);
  saveFocusInfo(nodeEditorInput, nodeContentsInput, 
		nodeAttributeWindowPane,
		applyCreate, 0);
  saveFocusInfo(nodeContentsInput, nodeIconInput, 
		nodeAttributeWindowPane,
		applyCreate, 0);
  saveFocusInfo(nodeIconInput, nodeNameInput, 
		nodeAttributeWindowPane,
		applyCreate, 0);

  jumpTo(nodeNameInput);
}

static void nodeAttributeSetTitle(char *title) {
  createNodeAttributeWindowWidgets(); /* make sure widgets are initialized */
  XtVaSetValues(nodeAttributeWindowShell,
		XtNtitle, title,
		NULL);
}

static void applyCreateButtonSetLabel(char *label) {
  createNodeAttributeWindowWidgets(); /* make sure widgets are initialized */
  XtVaSetValues(applyCreate,
		XtNlabel, label,
		XtNwidth, BUTTON_WIDTH,
		NULL);
}
