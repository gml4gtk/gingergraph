/* $Id: treeLayoutParameterWindow.c,v 1.2 1996/02/20 10:17:56 lupus Exp $ */

/***************************************************************
 * This file is part of the Ginger project.                    *
 * Created 16-02-1996                                          *
 * Niels C. Larsen, Aalborg University, Denmark                *
 * (lupus@iesd.auc.dk)                                         *
 * Martin K. Molz, Aalborg University, Denmark                 *
 * (molz@iesd.auc.dk)                                          *
 ***************************************************************/

/* pop-up window for supplying parameters for the 
   tree layout algorithm */

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
#include "treeLayoutParameterWindow.h"
#include "treeLayout.h"
#include "graphBuffer.h"
#include "focus.h"
#include "menu.h"
#include "menuOpcodes.h"
#include "str.h"

/* ---  private declarations  --- */

/* tree direction menu */
static Widget treeDirectionMenuButton;
static menu_item tree_direction_menu[] = {
  {"Vertical",      1, TREE_DIRECTION_MENU_VERTICAL, 0},
  {"Horizontal",    1, TREE_DIRECTION_MENU_HORIZONTAL, 0},
  {0,0,0,0} 
};
static int current_selection = TREE_DIRECTION_MENU_VERTICAL;

extern Widget topLevel;  /* from mainWindow.c */

#define BUTTON_WIDTH 75
#define LABEL_WIDTH 105
#define FIELD_WIDTH 50

static int popup=0;
static Widget treeLayoutParameterWindowShell;

/* input fields */
static Widget cornerXInput, cornerYInput;
static Widget crossingIterationsInput, placementIterationsInput;
static Widget XSpaceInput, YSpaceInput;

/* private functions prototypes */
static void createTreeLayoutParameterWindowWidgets();
static void treeLayoutParameterWindowApply(Widget w, XtPointer client_data, XtPointer call_data);

/* ---  public functions  --- */

void treeLayoutParameterWindowPopup(Widget w, XtPointer client_data, XtPointer call_data) {
  Position x,y;
  
  if( !popup ) {
    createTreeLayoutParameterWindowWidgets(); /* make sure widgets are initialized */

    /* position the popup window relative to the main window */
    XtTranslateCoords(topLevel,(Position)170, (Position)40,
		      &x, &y);
    
    XtVaSetValues(treeLayoutParameterWindowShell,
		  XtNx, x,
		  XtNy, y,
		  NULL );

    XtPopup(treeLayoutParameterWindowShell, XtGrabNone);
    popup=1;
  }
}

void treeLayoutParameterWindowSetDirection(int d) {
  char *label;

  createTreeLayoutParameterWindowWidgets(); /* make sure widgets are initialized */
  current_selection=d;

  if( d==TREE_DIRECTION_MENU_VERTICAL )
    label="Vertical";
  else
    label="Horizontal";
  
  XtVaSetValues(treeDirectionMenuButton,
                XtNlabel, label,
		XtNwidth, LABEL_WIDTH+FIELD_WIDTH,
                NULL);
}

void treeLayoutParameterWindowHotspotChange(int x, int y) {
  char buf[100];
  createTreeLayoutParameterWindowWidgets(); /* make sure widgets are initialized */
  if( popup ) {
    sprintf(buf, "%d", x);
    XtVaSetValues(cornerXInput, XtNstring, buf, NULL);
    sprintf(buf, "%d", y);
    XtVaSetValues(cornerYInput, XtNstring, buf, NULL);
  }
}

/* ---  private functions  --- */

static void treeLayoutParameterWindowClose(Widget w, XtPointer client_data, XtPointer call_data) {
  XtPopdown(treeLayoutParameterWindowShell);
  popup=0;
}

static void treeLayoutParameterWindowApply(Widget w, XtPointer client_data, XtPointer call_data) {
  int x,y,crossingIterations,placementIterations,xspace,yspace,vertical;
  String p;

  XtVaGetValues(cornerXInput, XtNstring, &p, NULL);
  x=atoi(p);
  XtVaGetValues(cornerYInput, XtNstring, &p, NULL);
  y=atoi(p);
  XtVaGetValues(crossingIterationsInput, XtNstring, &p, NULL);
  crossingIterations=atoi(p);
  XtVaGetValues(placementIterationsInput, XtNstring, &p, NULL);
  placementIterations=atoi(p);
  XtVaGetValues(XSpaceInput, XtNstring, &p, NULL);
  xspace=atoi(p);
  XtVaGetValues(YSpaceInput, XtNstring, &p, NULL);
  yspace=atoi(p);
  if( current_selection==TREE_DIRECTION_MENU_VERTICAL )
    vertical=1;
  else
    vertical=0;
  
  tree_layout(graphBufferGetCurrentGraph(), x, y, xspace, yspace, vertical, crossingIterations, placementIterations);
  
  treeLayoutParameterWindowClose(w,client_data, call_data);
}

static void createTreeLayoutParameterWindowWidgets() {
  static int done=0;
  
  Widget treeLayoutParameterWindowPane;
  Widget treeLayoutParameterWindowForm;
  Widget treeLayoutParameterWindowBox;
  
  /* buttons */
  Widget apply, close;
  
  /* labels */
  Widget xLabel, yLabel;
  Widget crossingIterationsLabel, placementIterationsLabel;
  Widget XSpaceLabel, YSpaceLabel;

  if(done)
    return;
  done=1;
  
  treeLayoutParameterWindowShell = XtVaCreatePopupShell (
				      "treeLayoutParameterWindowShell",
				      transientShellWidgetClass,
				      topLevel,
				      XtNtitle, "Layout parameters",
				      NULL );
  
  treeLayoutParameterWindowPane = XtVaCreateManagedWidget(
				      "treeLayoutParameterWindowPane",
				      panedWidgetClass,
				      treeLayoutParameterWindowShell,
				      NULL);
  
  treeLayoutParameterWindowForm = XtVaCreateManagedWidget(
				      "treeLayoutParameterWindowForm",
				      formWidgetClass,
				      treeLayoutParameterWindowPane,
				      NULL);

  xLabel = XtVaCreateManagedWidget(
				      "xLabel",
				      labelWidgetClass,
                                      treeLayoutParameterWindowForm,
                                      XtNlabel, "Left corner X:",
				      XtNwidth, LABEL_WIDTH,
				      XtNjustify, XtJustifyLeft,
                                      NULL);

  cornerXInput = XtVaCreateManagedWidget(
			              "cornerXInput",
				      asciiTextWidgetClass,
				      treeLayoutParameterWindowForm,
				      XtNstring, "50",
				      XtNeditType, XawtextEdit,
				      XtNwidth, FIELD_WIDTH,
				      XtNfromHoriz, xLabel,
				      NULL);

  yLabel = XtVaCreateManagedWidget(
				      "yLabel",
				      labelWidgetClass,
                                      treeLayoutParameterWindowForm,
                                      XtNlabel, "Upper corner Y:",
				      XtNwidth, LABEL_WIDTH,
				      XtNjustify, XtJustifyLeft,
				      XtNfromVert, xLabel,
                                      NULL);

  cornerYInput = XtVaCreateManagedWidget(
			              "cornerYInput",
				      asciiTextWidgetClass,
				      treeLayoutParameterWindowForm,
				      XtNstring, "50",
				      XtNeditType, XawtextEdit,
				      XtNwidth, FIELD_WIDTH,
				      XtNfromHoriz, yLabel,
				      XtNfromVert, xLabel,
				      NULL);

  XSpaceLabel = XtVaCreateManagedWidget(
				      "XSpaceLabel",
				      labelWidgetClass,
                                      treeLayoutParameterWindowForm,
                                      XtNlabel, "Horizontal space:",
				      XtNwidth, LABEL_WIDTH,
				      XtNjustify, XtJustifyLeft,
				      XtNfromVert, yLabel,
                                      NULL);

  XSpaceInput = XtVaCreateManagedWidget(
			              "XSpaceInput",
				      asciiTextWidgetClass,
				      treeLayoutParameterWindowForm,
				      XtNstring, "50",
				      XtNeditType, XawtextEdit,
				      XtNwidth, FIELD_WIDTH,
				      XtNfromHoriz, XSpaceLabel,
				      XtNfromVert, yLabel,
				      NULL);

  YSpaceLabel = XtVaCreateManagedWidget(
				      "YSpaceLabel",
				      labelWidgetClass,
                                      treeLayoutParameterWindowForm,
                                      XtNlabel, "Vertical space:",
				      XtNwidth, LABEL_WIDTH,
				      XtNjustify, XtJustifyLeft,
				      XtNfromVert, XSpaceLabel,
                                      NULL);

  YSpaceInput = XtVaCreateManagedWidget(
			              "YSpaceInput",
				      asciiTextWidgetClass,
				      treeLayoutParameterWindowForm,
				      XtNstring, "50",
				      XtNeditType, XawtextEdit,
				      XtNwidth, FIELD_WIDTH,
				      XtNfromHoriz, YSpaceLabel,
				      XtNfromVert, XSpaceLabel,
				      NULL);

  crossingIterationsLabel = XtVaCreateManagedWidget(
				      "crossingIterationsLabel",
				      labelWidgetClass,
                                      treeLayoutParameterWindowForm,
                                      XtNlabel, "Crossing iter.:",
				      XtNwidth, LABEL_WIDTH,
				      XtNjustify, XtJustifyLeft,
				      XtNfromVert, YSpaceLabel,
                                      NULL);

  crossingIterationsInput = XtVaCreateManagedWidget(
			              "crossingIterationsInput",
				      asciiTextWidgetClass,
				      treeLayoutParameterWindowForm,
				      XtNstring, "1",
				      XtNeditType, XawtextEdit,
				      XtNwidth, FIELD_WIDTH,
				      XtNfromHoriz, crossingIterationsLabel,
				      XtNfromVert, YSpaceLabel,
				      NULL);

  placementIterationsLabel = XtVaCreateManagedWidget(
				      "placementIterationsLabel",
				      labelWidgetClass,
                                      treeLayoutParameterWindowForm,
                                      XtNlabel, "Placement iter.:",
				      XtNwidth, LABEL_WIDTH,
				      XtNjustify, XtJustifyLeft,
				      XtNfromVert, crossingIterationsLabel,
                                      NULL);

  placementIterationsInput = XtVaCreateManagedWidget(
			              "placementIterationsInput",
				      asciiTextWidgetClass,
				      treeLayoutParameterWindowForm,
				      XtNstring, "3",
				      XtNeditType, XawtextEdit,
				      XtNwidth, FIELD_WIDTH,
				      XtNfromHoriz, placementIterationsLabel,
				      XtNfromVert, crossingIterationsLabel,
				      NULL);

/*  directionLabel = XtVaCreateManagedWidget(
				      "directionLabel",
				      labelWidgetClass,
                                      treeLayoutParameterWindowForm,
                                      XtNlabel, "Direction:",
				      XtNjustify, XtJustifyLeft,
				      XtNfromVert, crossingIterationsLabel,
                                      NULL);
*/
  treeDirectionMenuButton =  XtVaCreateManagedWidget(
				      "treeDirectionMenuButton",
				      menuButtonWidgetClass,
				      treeLayoutParameterWindowForm,
				      XtNmenuName, "treeDirectionMenu",
				      XtNlabel, "Vertical",
				      XtNleftBitmap, menuButtonMark(),
				      XtNwidth, LABEL_WIDTH+FIELD_WIDTH,
				      XtNfromVert, placementIterationsLabel,
				      NULL);
  
  createMenu("treeDirectionMenu", 
	     treeDirectionMenuButton,
	     tree_direction_menu);

  treeLayoutParameterWindowBox = XtVaCreateManagedWidget(
				      "treeLayoutParameterWindowBox",
				      boxWidgetClass,
				      treeLayoutParameterWindowPane,
				      NULL);

  apply = XtVaCreateManagedWidget(
				      "treeParameterWindowApply",
				      commandWidgetClass,
				      treeLayoutParameterWindowBox,
				      XtNlabel, "Ok",
				      XtNwidth, BUTTON_WIDTH, 
				      NULL);
  XtAddCallback(apply, XtNcallback, treeLayoutParameterWindowApply, topLevel); 

  close = XtVaCreateManagedWidget(
				      "treeLayoutParameterWindowClose",
				      commandWidgetClass,
				      treeLayoutParameterWindowBox,
				      XtNlabel, "Cancel",
				      XtNwidth, BUTTON_WIDTH, 
				      NULL);

  XtAddCallback(close, XtNcallback, treeLayoutParameterWindowClose, topLevel);

  saveFocusInfo(cornerXInput, cornerYInput,
		treeLayoutParameterWindowPane, apply, 1);
  saveFocusInfo(cornerYInput, XSpaceInput,
		treeLayoutParameterWindowPane, apply, 0);
  saveFocusInfo(XSpaceInput, YSpaceInput,
		treeLayoutParameterWindowPane, apply, 0);
  saveFocusInfo(YSpaceInput, crossingIterationsInput,
		treeLayoutParameterWindowPane, apply, 0);
  saveFocusInfo(crossingIterationsInput, placementIterationsInput, 
		treeLayoutParameterWindowPane, apply, 0);
  saveFocusInfo(placementIterationsInput, cornerXInput,
		treeLayoutParameterWindowPane, apply, 0);

  jumpTo(cornerXInput);
}
