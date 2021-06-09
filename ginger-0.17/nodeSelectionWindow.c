/* $Id: nodeSelectionWindow.c,v 1.3 1996/02/26 08:51:43 lupus Exp $ */

/***************************************************************
 * This file is part of the Ginger project.                    *
 * Created 7-2-1996                                            *
 * Niels C. Larsen, Aalborg University, Denmark                *
 * (lupus@iesd.auc.dk)                                         *
 ***************************************************************/

#include <X11/Intrinsic.h>
#include <X11/StringDefs.h>
#include <X11/Shell.h>

#include <X11/Xaw/Paned.h>
#include <X11/Xaw/Box.h>
#include <X11/Xaw/Command.h>
#include <X11/Xaw/Toggle.h>

#include "nodeSelectionWindow.h"
#include "nodeSelection.h"

/* ---  private declarations  --- */

extern Widget topLevel;  /* from mainWindow.c */
extern graph *the_graph; /* from graphBuffer.c */

#define BUTTON_WIDTH 140
#define NODE_SELECTION_WIDTH  150
#define NODE_SELECTION_HEIGHT 90

static int popup=0;
static Widget nodeSelectionWindowShell;
static Widget nodeSelectionWindowAccuMode;

/* private functions prototypes */
static void createNodeSelectionWindowWidgets();
static void NodeSelectionWindowClose(Widget w, XtPointer client_data, XtPointer call_data);

/* ---  public functions  --- */

void nodeSelectionWindowPopup(Widget w, XtPointer client_data, XtPointer call_data) {
  if( !popup ) {
    createNodeSelectionWindowWidgets();
    
    XtPopup(nodeSelectionWindowShell, XtGrabNone);
    popup=1;
  }
}

int nodeSelectionWindowGetAccumulationState() {
  Boolean state;

  createNodeSelectionWindowWidgets(); /* make sure widgets are initialized */

  XtVaGetValues(nodeSelectionWindowAccuMode,
                XtNstate, &state,
                NULL);

  return (state==True) ? ACCUMULATING_MODE:NON_ACCUMULATING_MODE;
}

/* ---  private functions  --- */

static void NodeSelectionWindowClose(Widget w, XtPointer client_data, XtPointer call_data) {
  XtPopdown(nodeSelectionWindowShell);
  popup=0;
}

static void NodeSelectionWindowSelectUnselected(Widget w, XtPointer client_data, XtPointer call_data) {
  nodeSelectUnselected(the_graph, nodeSelectionWindowGetAccumulationState());
}

static void createNodeSelectionWindowWidgets() {
  static int done=0;
  
  Widget nodeSelectionWindowPane;
  Widget nodeSelectionWindowBox, nodeSelectionWindowBox2;
  Widget nodeSelectionWindowClose;
  Widget nodeSelectionWindowSelectUnselected;
  
  if(done)
    return;
  done=1;

  nodeSelectionWindowShell = XtVaCreatePopupShell (
				      "nodeSelectionWindowShell",
				      transientShellWidgetClass,
				      topLevel,
				      XtNtitle, "Node selection",
				      NULL );
  
  nodeSelectionWindowPane = XtVaCreateManagedWidget(
				      "nodeSelectionWindowPane",
				      panedWidgetClass,
				      nodeSelectionWindowShell,
				      XtNheight, NODE_SELECTION_HEIGHT,
				      XtNwidth, NODE_SELECTION_WIDTH,
				      NULL);
  
  nodeSelectionWindowBox = XtVaCreateManagedWidget(
				      "nodeSelectionWindowBox",
				      boxWidgetClass,
				      nodeSelectionWindowPane,
				      NULL);

  nodeSelectionWindowClose = XtVaCreateManagedWidget(
				      "nodeSelectionWindowClose",
				      commandWidgetClass,
				      nodeSelectionWindowBox,
				      XtNlabel, "Close",
				      XtNwidth, 60, 
				      NULL);

  nodeSelectionWindowBox2 = XtVaCreateManagedWidget(
				      "nodeSelectionWindowBox2",
				      boxWidgetClass,
				      nodeSelectionWindowPane,
				      NULL);

  nodeSelectionWindowAccuMode = XtVaCreateManagedWidget(
				      "nodeSelectionWindowAccuMode",
				      toggleWidgetClass,
				      nodeSelectionWindowBox2,
				      XtNlabel, "Accumulate",
				      XtNwidth, BUTTON_WIDTH,
				      /* XtNstate, True, */
				      NULL);

  nodeSelectionWindowSelectUnselected = XtVaCreateManagedWidget(
				     "nodeSelectionWindowSelectUnselected",
				     commandWidgetClass,
				     nodeSelectionWindowBox2,
				     XtNlabel, "Select unselected",
				     XtNwidth, BUTTON_WIDTH,
				     NULL);
  
  XtAddCallback(nodeSelectionWindowClose, XtNcallback, NodeSelectionWindowClose, topLevel);
  XtAddCallback(nodeSelectionWindowSelectUnselected, XtNcallback, NodeSelectionWindowSelectUnselected, topLevel);
}
