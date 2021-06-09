/* $Id: edgeAttributeWindow.c,v 1.2 1996/02/13 09:18:58 lupus Exp $ */

/***************************************************************
 * This file is part of the Ginger project.                    *
 * Created 11-02-1996                                          *
 * Niels C. Larsen, Aalborg University, Denmark                *
 * (lupus@iesd.auc.dk)                                         *
 * Martin K. Molz, Aalborg University, Denmark                 *
 * (molz@iesd.auc.dk)                                          *
 ***************************************************************/

/* pop-up window for editing edge attributes */

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
#include "edgeAttributeWindow.h"
#include "graphDisplay.h"
#include "graphBuffer.h"
#include "focus.h"
#include "menu.h"
#include "menuOpcodes.h"
#include "str.h"

/* ---  private declarations  --- */

extern Widget topLevel;  /* from mainWindow.c */

#define BUTTON_WIDTH 65

static int popup=0;
static Widget edgeAttributeWindowShell;

/* input fields */
static Widget edgeAttributeWindowEdgeNameInput;
static Widget edgeAttributeWindowEdgeColorInput;

/* edge direction menu */
static Widget edgeAttributeWindowDirectionMenuButton;
static menu_item direction_menu[] = {
  {"Normal",        1, EDGE_DIRECTION_MENU_NORMAL, 0},
  {"Inverse",       1, EDGE_DIRECTION_MENU_INVERSE, 0},
  {"Bidirectional", 1, EDGE_DIRECTION_MENU_BOTH, 0},
  {"Undirected",    1, EDGE_DIRECTION_MENU_NONE, 0},
  {0,0,0,0} 
};
static int current_selection = EDGE_DIRECTION_MENU_NORMAL;

/* private functions prototypes */
static void createEdgeAttributeWindowWidgets();
static void EdgeAttributeWindowClose(Widget w, XtPointer client_data, XtPointer call_data);
static void EdgeAttributeWindowEditSelectedEdges(Widget w, XtPointer client_data, XtPointer call_data);

/* ---  public functions  --- */

void edgeAttributeWindowPopup(Widget w, XtPointer client_data, XtPointer call_data) {
  if( !popup ) {
    createEdgeAttributeWindowWidgets();
    
    XtPopup(edgeAttributeWindowShell, XtGrabNone);
    popup=1;
  }
}

void edgeAttributeWindowSetName(char *n) {
  createEdgeAttributeWindowWidgets(); /* make sure widgets are initialized */
  
  XtVaSetValues(edgeAttributeWindowEdgeNameInput,
                XtNstring, n,
                NULL);
}

void edgeAttributeWindowSetColor(char *n) {
  createEdgeAttributeWindowWidgets(); /* make sure widgets are initialized */
  
  XtVaSetValues(edgeAttributeWindowEdgeColorInput,
                XtNstring, n,
                NULL);
}

void edgeAttributeWindowSetDirection(int opcode) {
  char *n;
  createEdgeAttributeWindowWidgets(); /* make sure widgets are initialized */

  current_selection = opcode;

  switch(opcode) {
  case EDGE_DIRECTION_MENU_NORMAL:
    n="Normal";
    break;
  case EDGE_DIRECTION_MENU_INVERSE:
    n="Inverse";
    break;
  case EDGE_DIRECTION_MENU_BOTH:
    n="Bidirectional";
    break;
  case EDGE_DIRECTION_MENU_NONE:
    n="Undirected";
    break;
  }
  XtVaSetValues(edgeAttributeWindowDirectionMenuButton,
                XtNlabel, n,
		XtNwidth, 140,
                NULL);
}

/* display the attributes of e in the window */
void edgeAttributeWindowDisplayEdge(edge *e) {
  int dir;
  
  edgeAttributeWindowSetName(e->name);
  
  if( e->foreground.is_used ) {
    edgeAttributeWindowSetColor(e->foreground.color_name);
  }
  else
    edgeAttributeWindowSetColor("");
  
  switch(e->direction) {
  case DIR_NORMAL:
    dir = EDGE_DIRECTION_MENU_NORMAL;
    break;
  case DIR_INVERSE:
    dir = EDGE_DIRECTION_MENU_INVERSE;
    break;
  case DIR_BIDIRECTIONAL:
    dir = EDGE_DIRECTION_MENU_BOTH;
    break;
  case DIR_NONE:
    dir = EDGE_DIRECTION_MENU_NONE;
    break;
  }
  edgeAttributeWindowSetDirection(dir);
}

/* apply the attributes given in the window to e */
void edgeAttributeWindowApplyToEdge(edge *e) {
  String p;
  int dir, should_redraw;
  
  should_redraw = 0;
  
  switch(current_selection) {
  case EDGE_DIRECTION_MENU_NORMAL:
    dir = DIR_NORMAL;
    break;
  case EDGE_DIRECTION_MENU_INVERSE:
    dir = DIR_INVERSE;
    break;
  case EDGE_DIRECTION_MENU_BOTH:
    dir = DIR_BIDIRECTIONAL;
    break;
  case EDGE_DIRECTION_MENU_NONE:
    dir = DIR_NONE;
    break;
  }
  
  if( dir!=e->direction ) {
    should_redraw = 1;
    undrawEdge(e);
    e->direction = dir;
    calculateEdgeEndpoints(e);
  }

  XtVaGetValues(edgeAttributeWindowEdgeColorInput,
                XtNstring, &p,
                NULL);
  
  if( strlen(p)>0 ) {
    should_redraw = 1;
    edgeSetColor(e,p);
  }
  
  if( should_redraw ) {
    drawEdge(e);
  }
}

/* reset window, i.e. clear input fields and reset direction menu */
void edgeAttributeWindowReset() {
  edgeAttributeWindowSetName("");
  edgeAttributeWindowSetColor("");
  edgeAttributeWindowSetDirection(EDGE_DIRECTION_MENU_NORMAL);
}

/* ---  private functions  --- */

/* apply the user input in edge attribute window to all selected edges */
static void EdgeAttributeWindowEditSelectedEdges(Widget w, XtPointer client_data, XtPointer call_data) {
  int i;
  graph *g;
  edge *e;
  
  g=graphBufferGetCurrentGraph();
  
  for(i=1; i<=getsize(g->edges); i++) {
    e=(edge *)retrieve(i,g->edges);
    if( e->selected ) {
      edgeAttributeWindowApplyToEdge(e);
    }
  }
}

static void EdgeAttributeWindowClose(Widget w, XtPointer client_data, XtPointer call_data) {
  XtPopdown(edgeAttributeWindowShell);
  popup=0;
}

static void EdgeAttributeWindowClear(Widget w, XtPointer client_data, XtPointer call_data) {
  edgeAttributeWindowReset();
}

static void createEdgeAttributeWindowWidgets() {
  static int done=0;
  
  Widget edgeAttributeWindowPane, edgeAttributeWindowForm;
  Widget edgeAttributeWindowBox;

  /* buttons */
  Widget edgeAttributeWindowApply;
  Widget edgeAttributeWindowClose;
  Widget edgeAttributeWindowClear;

  /* labels */
  Widget edgeAttributeWindowEdgeNameLabel;
  Widget edgeAttributeWindowEdgeDirectionLabel;
  Widget edgeAttributeWindowEdgeColorLabel;

  if(done)
    return;
  done=1;

  edgeAttributeWindowShell = XtVaCreatePopupShell (
				      "edgeAttributeWindowShell",
				      transientShellWidgetClass,
				      topLevel,
				      XtNtitle, "Edge attributes",
				      NULL );
  
  edgeAttributeWindowPane = XtVaCreateManagedWidget(
				      "edgeAttributeWindowPane",
				      panedWidgetClass,
				      edgeAttributeWindowShell,
				      NULL);
  
  edgeAttributeWindowForm = XtVaCreateManagedWidget(
				      "edgeAttributeWindowForm",
				      formWidgetClass,
				      edgeAttributeWindowPane,
				      NULL);

  edgeAttributeWindowEdgeNameLabel = XtVaCreateManagedWidget(
				      "edgeAttributeWindowEdgeNameLabel",
				      labelWidgetClass,
                                      edgeAttributeWindowForm,
                                      XtNlabel, "Name:",
				      XtNjustify, XtJustifyLeft,
                                      NULL);

  edgeAttributeWindowEdgeNameInput = XtVaCreateManagedWidget(
			              "edgeAttributeWindowEdgeNameInput",
				      asciiTextWidgetClass,
				      edgeAttributeWindowForm,
				      XtNstring, "",
				      XtNeditType, XawtextEdit,
				      XtNwidth, 160,
				      XtNfromHoriz, edgeAttributeWindowEdgeNameLabel,
				      NULL);

  edgeAttributeWindowEdgeDirectionLabel = XtVaCreateManagedWidget(
				      "edgeAttributeWindowEdgeDirectionLabel",
				      labelWidgetClass,
                                      edgeAttributeWindowForm,
                                      XtNlabel, "Direction:",
				      XtNjustify, XtJustifyLeft,
				      XtNfromVert, edgeAttributeWindowEdgeNameLabel,
                                      NULL);

  edgeAttributeWindowDirectionMenuButton =  XtVaCreateManagedWidget(
				      "edgeAttributeWindowDirectionMenuButton",
				      menuButtonWidgetClass,
				      edgeAttributeWindowForm,
				      XtNmenuName, "edgeDirectionMenu",
				      XtNlabel, "Normal",
				      XtNleftBitmap, menuButtonMark(),
				      XtNwidth, 140,
				      XtNfromVert, edgeAttributeWindowEdgeNameLabel,
				      XtNfromHoriz, edgeAttributeWindowEdgeDirectionLabel,
				      NULL);
  
  createMenu("edgeDirectionMenu", 
	     edgeAttributeWindowDirectionMenuButton,
	     direction_menu);

  edgeAttributeWindowEdgeColorLabel = XtVaCreateManagedWidget(
				      "edgeAttributeWindowEdgeColorLabel",
				      labelWidgetClass,
                                      edgeAttributeWindowForm,
                                      XtNlabel, "Color:",
				      XtNjustify, XtJustifyLeft,
				      XtNfromVert, edgeAttributeWindowEdgeDirectionLabel,
                                      NULL);

  edgeAttributeWindowEdgeColorInput = XtVaCreateManagedWidget(
			              "edgeAttributeWindowEdgeColorInput",
				      asciiTextWidgetClass,
				      edgeAttributeWindowForm,
				      XtNstring, "",
				      XtNeditType, XawtextEdit,
				      XtNwidth, 160,
				      XtNfromHoriz, edgeAttributeWindowEdgeColorLabel,
				      XtNfromVert, edgeAttributeWindowEdgeDirectionLabel,
				      NULL);

  edgeAttributeWindowBox = XtVaCreateManagedWidget(
				      "edgeAttributeWindowBox",
				      boxWidgetClass,
				      edgeAttributeWindowPane,
				      NULL);

  edgeAttributeWindowApply = XtVaCreateManagedWidget(
				      "edgeAttributeWindowApply",
				      commandWidgetClass,
				      edgeAttributeWindowBox,
				      XtNlabel, "Apply",
				      XtNwidth, BUTTON_WIDTH, 
				      NULL);
  XtAddCallback(edgeAttributeWindowApply, XtNcallback, EdgeAttributeWindowEditSelectedEdges, topLevel);

  edgeAttributeWindowClear = XtVaCreateManagedWidget(
				      "edgeAttributeWindowClear",
				      commandWidgetClass,
				      edgeAttributeWindowBox,
				      XtNlabel, "Clear",
				      XtNwidth, BUTTON_WIDTH, 
				      NULL);
  XtAddCallback(edgeAttributeWindowClear, XtNcallback, EdgeAttributeWindowClear, topLevel);

  edgeAttributeWindowClose = XtVaCreateManagedWidget(
				      "edgeAttributeWindowClose",
				      commandWidgetClass,
				      edgeAttributeWindowBox,
				      XtNlabel, "Close",
				      XtNwidth, BUTTON_WIDTH, 
				      NULL);

  XtAddCallback(edgeAttributeWindowClose, XtNcallback, EdgeAttributeWindowClose, topLevel);

  saveFocusInfo(edgeAttributeWindowEdgeNameInput, 
		edgeAttributeWindowEdgeColorInput,
		edgeAttributeWindowPane,
		edgeAttributeWindowApply, 1);
  saveFocusInfo(edgeAttributeWindowEdgeColorInput,
		edgeAttributeWindowEdgeNameInput, 
		edgeAttributeWindowPane,
		edgeAttributeWindowApply, 0);

  jumpTo(edgeAttributeWindowEdgeNameInput);
}
