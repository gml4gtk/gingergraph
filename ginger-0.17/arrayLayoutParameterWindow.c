/* $Id: arrayLayoutParameterWindow.c,v 1.3 1996/02/26 08:51:43 lupus Exp $ */

/***************************************************************
 * This file is part of the Ginger project.                    *
 * Created 13-02-1996                                          *
 * Niels C. Larsen, Aalborg University, Denmark                *
 * (lupus@iesd.auc.dk)                                         *
 * Martin K. Molz, Aalborg University, Denmark                 *
 * (molz@iesd.auc.dk)                                          *
 ***************************************************************/

/* pop-up window for supplying parameters for the 
   array layout algorithm */

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
#include "arrayLayoutParameterWindow.h"
#include "arrayLayout.h"
#include "graphBuffer.h"
#include "focus.h"
#include "str.h"

/* ---  private declarations  --- */

extern Widget topLevel;  /* from mainWindow.c */

#define BUTTON_WIDTH 75
#define LABEL_WIDTH 105
#define FIELD_WIDTH 50

static int popup=0;
static Widget arrayLayoutParameterWindowShell;

/* input fields */
static Widget cornerXInput, cornerYInput, columnsInput;
static Widget XSpaceInput, YSpaceInput;

/* private functions prototypes */
static void createArrayLayoutParameterWindowWidgets();
static void arrayLayoutParameterWindowApply(Widget w, XtPointer client_data, XtPointer call_data);

/* ---  public functions  --- */

void arrayLayoutParameterWindowPopup(Widget w, XtPointer client_data, XtPointer call_data) {
  Position x,y;
  
  if( !popup ) {
    createArrayLayoutParameterWindowWidgets(); /* make sure widgets are initialized */

    /* position the popup window relative to the main window */
    XtTranslateCoords(topLevel,(Position)170, (Position)40,
		      &x, &y);
    
    XtVaSetValues(arrayLayoutParameterWindowShell,
		  XtNx, x,
		  XtNy, y,
		  NULL );

    XtPopup(arrayLayoutParameterWindowShell, XtGrabNone);
    popup=1;
  }
}

void arrayLayoutParameterWindowHotspotChange(int x, int y) {
  char buf[100];
  createArrayLayoutParameterWindowWidgets(); /* make sure widgets are initialized */
  if( popup ) {
    sprintf(buf, "%d", x);
    XtVaSetValues(cornerXInput, XtNstring, buf, NULL);
    sprintf(buf, "%d", y);
    XtVaSetValues(cornerYInput, XtNstring, buf, NULL);
  }
}

/* ---  private functions  --- */

static void arrayLayoutParameterWindowClose(Widget w, XtPointer client_data, XtPointer call_data) {
  XtPopdown(arrayLayoutParameterWindowShell);
  popup=0;
}

static void arrayLayoutParameterWindowApply(Widget w, XtPointer client_data, XtPointer call_data) {
  int x,y,columns,xspace,yspace;
  String p;

  XtVaGetValues(cornerXInput, XtNstring, &p, NULL);
  x=atoi(p);
  XtVaGetValues(cornerYInput, XtNstring, &p, NULL);
  y=atoi(p);
  XtVaGetValues(columnsInput, XtNstring, &p, NULL);
  columns=atoi(p);
  XtVaGetValues(XSpaceInput, XtNstring, &p, NULL);
  xspace=atoi(p);
  XtVaGetValues(YSpaceInput, XtNstring, &p, NULL);
  yspace=atoi(p);
  
  array_layout(graphBufferGetCurrentGraph(), x, y, xspace, yspace, columns);
  
  arrayLayoutParameterWindowClose(w,client_data, call_data);
}

static void createArrayLayoutParameterWindowWidgets() {
  static int done=0;
  
  Widget arrayLayoutParameterWindowPane;
  Widget arrayLayoutParameterWindowForm;
  Widget arrayLayoutParameterWindowBox;
  
  /* buttons */
  Widget apply, close;
  
  /* labels */
  Widget xLabel, yLabel, columnsLabel, XSpaceLabel, YSpaceLabel;
  
  if(done)
    return;
  done=1;
  
  arrayLayoutParameterWindowShell = XtVaCreatePopupShell (
				      "arrayLayoutParameterWindowShell",
				      transientShellWidgetClass,
				      topLevel,
				      XtNtitle, "Layout parameters",
				      NULL );
  
  arrayLayoutParameterWindowPane = XtVaCreateManagedWidget(
				      "arrayLayoutParameterWindowPane",
				      panedWidgetClass,
				      arrayLayoutParameterWindowShell,
				      NULL);
  
  arrayLayoutParameterWindowForm = XtVaCreateManagedWidget(
				      "arrayLayoutParameterWindowForm",
				      formWidgetClass,
				      arrayLayoutParameterWindowPane,
				      NULL);

  xLabel = XtVaCreateManagedWidget(
				      "xLabel",
				      labelWidgetClass,
                                      arrayLayoutParameterWindowForm,
                                      XtNlabel, "Left corner X:",
				      XtNwidth, LABEL_WIDTH,
				      XtNjustify, XtJustifyLeft,
                                      NULL);

  cornerXInput = XtVaCreateManagedWidget(
			              "cornerXInput",
				      asciiTextWidgetClass,
				      arrayLayoutParameterWindowForm,
				      XtNstring, "50",
				      XtNeditType, XawtextEdit,
				      XtNwidth, FIELD_WIDTH,
				      XtNfromHoriz, xLabel,
				      NULL);

  yLabel = XtVaCreateManagedWidget(
				      "yLabel",
				      labelWidgetClass,
                                      arrayLayoutParameterWindowForm,
                                      XtNlabel, "Upper corner Y:",
				      XtNwidth, LABEL_WIDTH,
				      XtNjustify, XtJustifyLeft,
				      XtNfromVert, xLabel,
                                      NULL);

  cornerYInput = XtVaCreateManagedWidget(
			              "cornerYInput",
				      asciiTextWidgetClass,
				      arrayLayoutParameterWindowForm,
				      XtNstring, "50",
				      XtNeditType, XawtextEdit,
				      XtNwidth, FIELD_WIDTH,
				      XtNfromHoriz, yLabel,
				      XtNfromVert, xLabel,
				      NULL);

  columnsLabel = XtVaCreateManagedWidget(
				      "columnsLabel",
				      labelWidgetClass,
                                      arrayLayoutParameterWindowForm,
                                      XtNlabel, "Columns:",
				      XtNwidth, LABEL_WIDTH,
				      XtNjustify, XtJustifyLeft,
				      XtNfromVert, yLabel,
                                      NULL);

  columnsInput = XtVaCreateManagedWidget(
			              "columnsInput",
				      asciiTextWidgetClass,
				      arrayLayoutParameterWindowForm,
				      XtNstring, "4",
				      XtNeditType, XawtextEdit,
				      XtNwidth, FIELD_WIDTH,
				      XtNfromHoriz, columnsLabel,
				      XtNfromVert, yLabel,
				      NULL);

  XSpaceLabel = XtVaCreateManagedWidget(
				      "XSpaceLabel",
				      labelWidgetClass,
                                      arrayLayoutParameterWindowForm,
                                      XtNlabel, "Horizontal space:",
				      XtNwidth, LABEL_WIDTH,
				      XtNjustify, XtJustifyLeft,
				      XtNfromVert, columnsLabel,
                                      NULL);

  XSpaceInput = XtVaCreateManagedWidget(
			              "XSpaceInput",
				      asciiTextWidgetClass,
				      arrayLayoutParameterWindowForm,
				      XtNstring, "50",
				      XtNeditType, XawtextEdit,
				      XtNwidth, FIELD_WIDTH,
				      XtNfromHoriz, XSpaceLabel,
				      XtNfromVert, columnsLabel,
				      NULL);

  YSpaceLabel = XtVaCreateManagedWidget(
				      "YSpaceLabel",
				      labelWidgetClass,
                                      arrayLayoutParameterWindowForm,
                                      XtNlabel, "Vertical space:",
				      XtNwidth, LABEL_WIDTH,
				      XtNjustify, XtJustifyLeft,
				      XtNfromVert, XSpaceLabel,
                                      NULL);

  YSpaceInput = XtVaCreateManagedWidget(
			              "YSpaceInput",
				      asciiTextWidgetClass,
				      arrayLayoutParameterWindowForm,
				      XtNstring, "50",
				      XtNeditType, XawtextEdit,
				      XtNwidth, FIELD_WIDTH,
				      XtNfromHoriz, YSpaceLabel,
				      XtNfromVert, XSpaceLabel,
				      NULL);

  arrayLayoutParameterWindowBox = XtVaCreateManagedWidget(
				      "arrayLayoutParameterWindowBox",
				      boxWidgetClass,
				      arrayLayoutParameterWindowPane,
				      NULL);

  apply = XtVaCreateManagedWidget(
				      "arrayParameterWindowApply",
				      commandWidgetClass,
				      arrayLayoutParameterWindowBox,
				      XtNlabel, "Ok",
				      XtNwidth, BUTTON_WIDTH, 
				      NULL);
  XtAddCallback(apply, XtNcallback, arrayLayoutParameterWindowApply, topLevel); 

  close = XtVaCreateManagedWidget(
				      "arrayLayoutParameterWindowClose",
				      commandWidgetClass,
				      arrayLayoutParameterWindowBox,
				      XtNlabel, "Cancel",
				      XtNwidth, BUTTON_WIDTH, 
				      NULL);

  XtAddCallback(close, XtNcallback, arrayLayoutParameterWindowClose, topLevel);

  saveFocusInfo(cornerXInput, cornerYInput,
		arrayLayoutParameterWindowPane, apply, 1);
  saveFocusInfo(cornerYInput, columnsInput,
		arrayLayoutParameterWindowPane, apply, 0);
  saveFocusInfo(columnsInput, XSpaceInput,
		arrayLayoutParameterWindowPane, apply, 0);
  saveFocusInfo(XSpaceInput, YSpaceInput,
		arrayLayoutParameterWindowPane, apply, 0);
  saveFocusInfo(YSpaceInput, cornerXInput,
		arrayLayoutParameterWindowPane, apply, 0);

  jumpTo(cornerXInput);
}
