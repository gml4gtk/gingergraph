/* $Id: notify.c,v 1.3 1996/02/19 10:34:46 lupus Exp $ */

/***************************************************************
 * This file is part of the Ginger project.                    *
 * Created 28-1-1996                                           *
 * Niels C. Larsen, Aalborg University, Denmark                *
 * (lupus@iesd.auc.dk)                                         *
 ***************************************************************/

/* Implements a small notification window with two buttons. 
   The text to be displayed in the notification window,
   the labels of the buttons, and the
   functions to be called when the user activates one of the 
   buttons are given as parameters to functions in this module.
*/
 
#include <X11/Intrinsic.h>
#include <X11/StringDefs.h>
#include <X11/Shell.h>

#include <X11/Xaw/Command.h>
#include <X11/Xaw/Form.h>
#include <X11/Xaw/Dialog.h>
#include <X11/Xaw/Text.h>
#include <X11/Xaw/AsciiText.h>

#include <string.h>
#include <stdio.h>

#include "notify.h"
#include "helpWindow.h"

/* ---  private declarations  --- */

extern Widget topLevel;  /* from mainWindow.c */

/* window and button width */
#define NOTIFY_WIDTH 350
#define NOTIFY_BUTTON_WIDTH (NOTIFY_WIDTH/2)-5

/* widgets */
static Widget notifyShell, notifyTextLabel;
static Widget notifyButton1, notifyButton2;

/* functions to be called at button-press */
static notify_callback_type f1, f2;

static int popup=0;
static char *the_hypertext_node;

/* private functions prototypes */
static void createNotifyWidgets();
static void notifyOfferHelp();

/* ---  public functions  --- */

/* Change the sensitivity of the two buttons */
void notifySetButtonSensitive(int button, int mode) { 
  
  createNotifyWidgets();
   
  switch(button) { 
  case1: 
    if (mode) 
      XtVaSetValues(notifyButton1, 
		    XtNsensitive, True, 
		    NULL);
    else
      XtVaSetValues(notifyButton1, 
		    XtNsensitive, False, 
		    NULL);
    break; 
  case 2:
    if (mode) 
      XtVaSetValues(notifyButton2, 
		    XtNsensitive, True, 
		    NULL);
    else
      XtVaSetValues(notifyButton2, 
		    XtNsensitive, False, 
		    NULL);
    break; 
  }
} 

/* pop-up the notification window, displaying the given text */
void notifyDisplay(char *text)
{
  Position x,y;
  
  createNotifyWidgets();
  
  XtVaSetValues(notifyTextLabel,	
		XtNlabel, text,
		NULL);

  if(!popup) {
    XtTranslateCoords(topLevel,(Position)25, (Position)50,
		      &x, &y);
    
    XtVaSetValues(notifyShell, XtNx, x,
		  XtNy, y,
		  NULL );
  
    popup=1;
    XtPopup(notifyShell, XtGrabNonexclusive);
  }
}

void notifySetTitle(char *title) {
  createNotifyWidgets();  /* make sure widgets are created */
  
  XtVaSetValues(notifyShell,
		XtNtitle, title,
		NULL);
}

void notifySetButtonCallback(int button, notify_callback_type f) {
  switch(button) {
  case 1:
    f1=f;
    break;
  case 2:
    f2=f;
    break;
  }
}

void notifySetButtonLabel(int button, char *label) {
  createNotifyWidgets();  /* make sure widgets are created */
  
  switch(button) {
  case 1:
    XtVaSetValues(notifyButton1,	
		  XtNlabel, label,
		  XtNwidth, NOTIFY_BUTTON_WIDTH,
		  NULL);
    break;
  case 2:
    XtVaSetValues(notifyButton2,	
		  XtNlabel, label,
		  XtNwidth, NOTIFY_BUTTON_WIDTH,
		  NULL);
    break;
  }
}

/* ask the user to confirm something. The buttons will be labelled
   "Ok" and "Cancel". The paramteres "confirm" and "cancel" are the 
   functions to be called when the corresponding button is activated. */
void notifyAskConfirm(char *text, notify_callback_type confirm, 
		      notify_callback_type cancel) {
  notifySetTitle("note");
  notifySetButtonLabel(1,"Ok");
  notifySetButtonLabel(2,"Cancel");
  f1=confirm;
  f2=cancel;
  notifyDisplay(text);
}

/* inform the user about some (error)condition. The user may activate
   the "Help" button in which case the help window will pop up 
   displaying "hypertext_node" */
void notifyAndOfferHelp(char *text, char *hypertext_node) {
  if(hypertext_node)
    the_hypertext_node=hypertext_node;
  else
    the_hypertext_node="nolink";
  
  notifySetTitle("Ginger says...");
  notifySetButtonLabel(1,"All right");
  notifySetButtonLabel(2,"Help");
  f1=0;
  f2=notifyOfferHelp;
  notifyDisplay(text);
}

/* ---  private functions  --- */

static void Button1Callback(Widget w, XtPointer client_data, XtPointer call_data)
{
  popup=0;
  XtPopdown(notifyShell);
  if(f1) {
    (*f1)();
  }
}

static void Button2Callback(Widget w, XtPointer client_data, XtPointer call_data)
{
  popup=0;
  XtPopdown(notifyShell);
  if(f2) {
    (*f2)();
  }
}

static void createNotifyWidgets() {
  static int done=0;
  Widget notifyForm;
  
  if(done)
    return;
  done=1;

  f1=0; f2=0;

  notifyShell = XtVaCreatePopupShell (
				     "notifyShell",
				     transientShellWidgetClass,
				     topLevel,
				     XtNtitle, "note",
				     XtNwidth, NOTIFY_WIDTH,
				     NULL );
  
  notifyForm = XtVaCreateManagedWidget(
				   "notifyForm",
				   formWidgetClass,
				   notifyShell,
				   NULL);
  
  notifyTextLabel = XtVaCreateManagedWidget(
				     "notifyTextLabel",
				     labelWidgetClass,
				     notifyForm,
				     XtNlabel, "none" ,
				     XtNwidth, NOTIFY_WIDTH-10,
				     XtNresize, False,
				     NULL);

  notifyButton1 = XtVaCreateManagedWidget(
				    "notifyButton1",
				    commandWidgetClass,
				    notifyForm,
				    XtNlabel, "Ok",
				    XtNfromVert, notifyTextLabel,
				    XtNwidth, NOTIFY_BUTTON_WIDTH,
				    NULL);

  notifyButton2 = XtVaCreateManagedWidget(
				    "notifyButton2",
				    commandWidgetClass,
				    notifyForm,
				    XtNlabel, "Cancel",
				    XtNfromVert, notifyTextLabel,
				    XtNfromHoriz, notifyButton1,
				    XtNwidth, NOTIFY_BUTTON_WIDTH,
				    NULL);

  XtAddCallback(notifyButton1, XtNcallback, Button1Callback, topLevel);
  XtAddCallback(notifyButton2, XtNcallback, Button2Callback, topLevel);
}

/* pop up the help window to display the relevant node in the 
   hypertext help */
static void notifyOfferHelp() {
  helpWindowDisplayNode(the_hypertext_node);
}

