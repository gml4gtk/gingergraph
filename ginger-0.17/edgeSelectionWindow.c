/* $Id: edgeSelectionWindow.c,v 1.2 1996/02/26 08:51:43 lupus Exp $ */

/***************************************************************
 * This file is part of the Ginger project.                    *
 * Created 10-02-1996                                          *
 * Niels C. Larsen, Aalborg University, Denmark                *
 * (lupus@iesd.auc.dk)                                         *
 * Martin K. Molz, Aalborg University, Denmark                 *
 * (molz@iesd.auc.dk)                                          *
 ***************************************************************/

#include <X11/Intrinsic.h>
#include <X11/StringDefs.h>
#include <X11/Shell.h>

#include <X11/Xaw/Paned.h>
#include <X11/Xaw/Box.h>
#include <X11/Xaw/Command.h>
#include <X11/Xaw/Toggle.h>

#include "edgeSelectionWindow.h"
#include "edgeSelection.h"
#include "graphDisplay.h"

/* ---  private declarations  --- */

extern Widget topLevel;  /* from mainWindow.c */
extern graph *the_graph; /* from graphBuffer.c */

#define BUTTON_WIDTH 140
#define EDGE_SELECTION_WIDTH  150
#define EDGE_SELECTION_HEIGHT 90

static int popup=0;
static Widget edgeSelectionWindowShell;
static Widget edgeSelectionWindowAccuMode;

/* private functions prototypes */
static void createEdgeSelectionWindowWidgets();
static void EdgeSelectionWindowClose(Widget w, XtPointer client_data, XtPointer call_data);

/* ---  public functions  --- */

void edgeSelectionWindowPopup(Widget w, XtPointer client_data, XtPointer call_data) {
  if( !popup ) {
    createEdgeSelectionWindowWidgets();
    
    XtPopup(edgeSelectionWindowShell, XtGrabNone);
    popup=1;
  }
}

int edgeSelectionWindowGetAccumulationState() {
  Boolean state;

  createEdgeSelectionWindowWidgets(); /* make sure widgets are initialized */

  XtVaGetValues(edgeSelectionWindowAccuMode,
                XtNstate, &state,
                NULL);

  return (state==True) ? ACCUMULATING_MODE:NON_ACCUMULATING_MODE;
}

/* ---  private functions  --- */

static void EdgeSelectionWindowClose(Widget w, XtPointer client_data, XtPointer call_data) {
  XtPopdown(edgeSelectionWindowShell);
  popup=0;
}

static void EdgeSelectionWindowSelectUnselected(Widget w, XtPointer client_data, XtPointer call_data) {
  edgeSelectUnselected(the_graph, edgeSelectionWindowGetAccumulationState());
}

static void createEdgeSelectionWindowWidgets() {
  static int done=0;
  
  Widget edgeSelectionWindowPane;
  Widget edgeSelectionWindowBox, edgeSelectionWindowBox2;
  Widget edgeSelectionWindowClose;
  Widget edgeSelectionWindowSelectUnselected;
  
  if(done)
    return;
  done=1;

  edgeSelectionWindowShell = XtVaCreatePopupShell (
				      "edgeSelectionWindowShell",
				      transientShellWidgetClass,
				      topLevel,
				      XtNtitle, "Edge selection",
				      NULL );
  
  edgeSelectionWindowPane = XtVaCreateManagedWidget(
				      "edgeSelectionWindowPane",
				      panedWidgetClass,
				      edgeSelectionWindowShell,
				      XtNheight, EDGE_SELECTION_HEIGHT,
				      XtNwidth, EDGE_SELECTION_WIDTH,
				      NULL);
  
  edgeSelectionWindowBox = XtVaCreateManagedWidget(
				      "edgeSelectionWindowBox",
				      boxWidgetClass,
				      edgeSelectionWindowPane,
				      NULL);

  edgeSelectionWindowClose = XtVaCreateManagedWidget(
				      "edgeSelectionWindowClose",
				      commandWidgetClass,
				      edgeSelectionWindowBox,
				      XtNlabel, "Close",
				      XtNwidth, 60, 
				      NULL);

  edgeSelectionWindowBox2 = XtVaCreateManagedWidget(
				      "edgeSelectionWindowBox2",
				      boxWidgetClass,
				      edgeSelectionWindowPane,
				      NULL);

  edgeSelectionWindowAccuMode = XtVaCreateManagedWidget(
				      "edgeSelectionWindowAccuMode",
				      toggleWidgetClass,
				      edgeSelectionWindowBox2,
				      XtNlabel, "Accumulate",
				      XtNwidth, BUTTON_WIDTH,
				      /* XtNstate, True, */
				      NULL);

  edgeSelectionWindowSelectUnselected = XtVaCreateManagedWidget(
				     "edgeSelectionWindowSelectUnselected",
				     commandWidgetClass,
				     edgeSelectionWindowBox2,
				     XtNlabel, "Select unselected",
				     XtNwidth, BUTTON_WIDTH,
				     NULL);
  
  XtAddCallback(edgeSelectionWindowClose, XtNcallback, EdgeSelectionWindowClose, topLevel);
  XtAddCallback(edgeSelectionWindowSelectUnselected, XtNcallback, EdgeSelectionWindowSelectUnselected, topLevel);
}
