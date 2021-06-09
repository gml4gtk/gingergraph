/* $Id: circularLayoutParameterWindow.c,v 1.3 1996/02/26 08:51:43 lupus Exp $ */

/***************************************************************
 * This file is part of the Ginger project.                    *
 * Created 13-02-1996                                          *
 * Niels C. Larsen, Aalborg University, Denmark                *
 * (lupus@iesd.auc.dk)                                         *
 * Martin K. Molz, Aalborg University, Denmark                 *
 * (molz@iesd.auc.dk)                                          *
 ***************************************************************/

/* pop-up window for supplying parameters for the 
   circular layout algorithm */

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
#include "circularLayoutParameterWindow.h"
#include "graphBuffer.h"
#include "focus.h"
#include "str.h"

/* ---  private declarations  --- */

extern Widget topLevel;  /* from mainWindow.c */

#define BUTTON_WIDTH 65
#define LABEL_WIDTH 77
#define FIELD_WIDTH 57

static int popup=0;
static Widget circularLayoutParameterWindowShell;

/* input fields */
static Widget centerXInput, centerYInput, radiusInput;

/* private functions prototypes */
static void createCircularLayoutParameterWindowWidgets();
static void circularLayoutParameterWindowApply(Widget w, XtPointer client_data, XtPointer call_data);

/* ---  public functions  --- */

void circularLayoutParameterWindowPopup(Widget w, XtPointer client_data, XtPointer call_data) {
  Position x,y;
  
  if( !popup ) {
    createCircularLayoutParameterWindowWidgets(); /* make sure widgets are initialized */

    /* position the popup window relative to the main window */
    XtTranslateCoords(topLevel,(Position)170, (Position)40,
		      &x, &y);
    
    XtVaSetValues(circularLayoutParameterWindowShell,
		  XtNx, x,
		  XtNy, y,
		  NULL );

    XtPopup(circularLayoutParameterWindowShell, XtGrabNone);
    popup=1;
  }
}

void circularLayoutParameterWindowHotspotChange(int x, int y) {
  char buf[100];
  createCircularLayoutParameterWindowWidgets(); /* make sure widgets are initialized */

  if( popup ) {
    sprintf(buf, "%d", x);
    XtVaSetValues(centerXInput, XtNstring, buf, NULL);
    sprintf(buf, "%d", y);
    XtVaSetValues(centerYInput, XtNstring, buf, NULL);
  }
}

/* ---  private functions  --- */

static void circularLayoutParameterWindowClose(Widget w, XtPointer client_data, XtPointer call_data) {
  XtPopdown(circularLayoutParameterWindowShell);
  popup=0;
}

static void circularLayoutParameterWindowApply(Widget w, XtPointer client_data, XtPointer call_data) {
  int x,y,radius;
  String p;

  XtVaGetValues(centerXInput, XtNstring, &p, NULL);
  x=atoi(p);
  XtVaGetValues(centerYInput, XtNstring, &p, NULL);
  y=atoi(p);
  XtVaGetValues(radiusInput, XtNstring, &p, NULL);
  radius=atoi(p);

  circle_layout(graphBufferGetCurrentGraph(), x, y, radius);

  circularLayoutParameterWindowClose(w,client_data, call_data);
}

static void createCircularLayoutParameterWindowWidgets() {
  static int done=0;
  
  Widget circularLayoutParameterWindowPane;
  Widget circularLayoutParameterWindowForm;
  Widget circularLayoutParameterWindowBox;
  
  /* buttons */
  Widget apply, close;
  
  /* labels */
  Widget xLabel, yLabel, radiusLabel;
  
  if(done)
    return;
  done=1;
  
  circularLayoutParameterWindowShell = XtVaCreatePopupShell (
				      "circularLayoutParameterWindowShell",
				      transientShellWidgetClass,
				      topLevel,
				      XtNtitle, "Layout parameters",
				      NULL );
  
  circularLayoutParameterWindowPane = XtVaCreateManagedWidget(
				      "circularLayoutParameterWindowPane",
				      panedWidgetClass,
				      circularLayoutParameterWindowShell,
				      NULL);
  
  circularLayoutParameterWindowForm = XtVaCreateManagedWidget(
				      "circularLayoutParameterWindowForm",
				      formWidgetClass,
				      circularLayoutParameterWindowPane,
				      NULL);

  xLabel = XtVaCreateManagedWidget(
				      "xLabel",
				      labelWidgetClass,
                                      circularLayoutParameterWindowForm,
                                      XtNlabel, "Center X:",
				      XtNwidth, LABEL_WIDTH,
				      XtNjustify, XtJustifyLeft,
                                      NULL);

  centerXInput = XtVaCreateManagedWidget(
			              "centerXInput",
				      asciiTextWidgetClass,
				      circularLayoutParameterWindowForm,
				      XtNstring, "150",
				      XtNeditType, XawtextEdit,
				      XtNwidth, FIELD_WIDTH,
				      XtNfromHoriz, xLabel,
				      NULL);

  yLabel = XtVaCreateManagedWidget(
				      "yLabel",
				      labelWidgetClass,
                                      circularLayoutParameterWindowForm,
                                      XtNlabel, "Center Y:",
				      XtNwidth, LABEL_WIDTH,
				      XtNjustify, XtJustifyLeft,
				      XtNfromVert, xLabel,
                                      NULL);

  centerYInput = XtVaCreateManagedWidget(
			              "centerYInput",
				      asciiTextWidgetClass,
				      circularLayoutParameterWindowForm,
				      XtNstring, "150",
				      XtNeditType, XawtextEdit,
				      XtNwidth, FIELD_WIDTH,
				      XtNfromHoriz, yLabel,
				      XtNfromVert, xLabel,
				      NULL);

  radiusLabel = XtVaCreateManagedWidget(
				      "radiusLabel",
				      labelWidgetClass,
                                      circularLayoutParameterWindowForm,
                                      XtNlabel, "Radius:",
				      XtNwidth, LABEL_WIDTH,
				      XtNjustify, XtJustifyLeft,
				      XtNfromVert, yLabel,
                                      NULL);

  radiusInput = XtVaCreateManagedWidget(
			              "radiusInput",
				      asciiTextWidgetClass,
				      circularLayoutParameterWindowForm,
				      XtNstring, "100",
				      XtNeditType, XawtextEdit,
				      XtNwidth, FIELD_WIDTH,
				      XtNfromHoriz, radiusLabel,
				      XtNfromVert, yLabel,
				      NULL);

  circularLayoutParameterWindowBox = XtVaCreateManagedWidget(
				      "circularLayoutParameterWindowBox",
				      boxWidgetClass,
				      circularLayoutParameterWindowPane,
				      NULL);

  apply = XtVaCreateManagedWidget(
				      "circularParameterWindowApply",
				      commandWidgetClass,
				      circularLayoutParameterWindowBox,
				      XtNlabel, "Ok",
				      XtNwidth, BUTTON_WIDTH, 
				      NULL);
  XtAddCallback(apply, XtNcallback, circularLayoutParameterWindowApply, topLevel); 

  close = XtVaCreateManagedWidget(
				      "circularLayoutParameterWindowClose",
				      commandWidgetClass,
				      circularLayoutParameterWindowBox,
				      XtNlabel, "Cancel",
				      XtNwidth, BUTTON_WIDTH, 
				      NULL);

  XtAddCallback(close, XtNcallback, circularLayoutParameterWindowClose, topLevel);

  saveFocusInfo(centerXInput, centerYInput,
		circularLayoutParameterWindowPane, apply, 1);
  saveFocusInfo(centerYInput, radiusInput,
		circularLayoutParameterWindowPane, apply, 0);
  saveFocusInfo(radiusInput, centerXInput,
		circularLayoutParameterWindowPane, apply, 0);

  jumpTo(centerXInput);
}
