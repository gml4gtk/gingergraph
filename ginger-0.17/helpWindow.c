/* $Id: helpWindow.c,v 1.3 1996/03/22 10:04:26 lupus Exp $ */

/***************************************************************
 * This file is part of the Ginger project.                    *
 * Created 27-1-1996                                           *
 * Niels C. Larsen, Aalborg University, Denmark                *
 * (lupus@iesd.auc.dk)                                         *
 ***************************************************************/

#include <X11/Intrinsic.h>
#include <X11/StringDefs.h>
#include <X11/Shell.h>

#include <X11/Xaw/Command.h>
#include <X11/Xaw/Paned.h>
#include <X11/Xaw/Text.h>
#include <X11/Xaw/Box.h>
#include <X11/Xaw/AsciiText.h>

#include "helpWindow.h"
#include "config.h"
#include "hyper.h"
#include "str.h"
#include "stringstack.h"
#include "main.h"

/* ---  private declarations  --- */

extern Widget topLevel;  /* from mainWindow.c */

#define ROOT_NODE "contents"
#define BUTTON_WIDTH 60

static int popup=0;
static char *currentText;
static char *theNextNode="";
static char currentLink[LINK_MAX_LEN];
static Widget helpWindowShell, helpWindowNextLabel, helpWindowText;

/* private functions prototypes */
static void createHelpWindowWidgets();
static void setnextNode();
static void HelpWindowClose(Widget w, XtPointer client_data, XtPointer call_data);
static void HelpWindowNext(Widget w, XtPointer client_data, XtPointer call_data);
static void HelpWindowContents(Widget w, XtPointer client_data, XtPointer call_data);
static void HelpWindowBack(Widget w, XtPointer client_data, XtPointer call_data);

/* ---  public functions  --- */

void helpWindowPopup(Widget w, XtPointer client_data, XtPointer call_data) {
  if( !popup ) {
    createHelpWindowWidgets();
    
    XtPopup(helpWindowShell, XtGrabNone);
    popup=1;
  }
}

void helpWindowLinkSelection(Widget w, XEvent *event, 
			     String *params, Cardinal *num_params)
{
  XawTextPosition n;
  char *link, *text;

  n=XawTextGetInsertionPoint(helpWindowText);

  link=linkAtPosition(currentText,n);
  if(strcmp(link,"nolink")) {   /* if not "nolink" */
    text=bodyText(link);
    if(strcmp(text,"nomatch")) {   /* if not "nomatch" */
      stringpush(currentLink);
      strcpy(currentLink,link);
      theNextNode=nextNode(currentLink);
      setnextNode();
      currentText=text;
      XtVaSetValues(helpWindowText,       
		    XtNstring, currentText,
		    NULL);
    }
  }
}

void helpWindowDisplayNode(char *helpl) {
  char *text;
  if( !popup ) {
    createHelpWindowWidgets();
    
    /*  XtSetSensitive(helpWindowKnap,FALSE); */
    XtPopup(helpWindowShell, XtGrabNone);
    popup=1;
  }

  if(strcmp(helpl,"nolink")) {   /* if not "nolink" */
    text=bodyText(helpl);
    if(strcmp(text,"nomatch")) {   /* if not "nomatch" */
      strcpy(currentLink,helpl);
      theNextNode=nextNode(currentLink);
      setnextNode();
      currentText=text;
      XtVaSetValues(helpWindowText,       
		    XtNstring, currentText,
		    NULL);
    }
  }
}

/* ---  private functions  --- */

static void setnextNode() {
  char buf[LINK_MAX_LEN];
  strcpy(buf, "Next: ");
  zeroAddStrncpy(buf+6, theNextNode, LINK_MAX_LEN-10);
  XtVaSetValues(helpWindowNextLabel,
		XtNlabel, buf,
		NULL);
}  

static void HelpWindowClose(Widget w, XtPointer client_data, XtPointer call_data) {
  XtPopdown(helpWindowShell);
  popup=0;
}

static void HelpWindowNext(Widget w, XtPointer client_data, XtPointer call_data) {
  char *link;

  stringpush(currentLink);
  link=nextNode(currentLink);
  helpWindowDisplayNode(link);
}

static void HelpWindowContents(Widget w, XtPointer client_data, XtPointer call_data) {
  stringpush(currentLink);
  helpWindowDisplayNode(ROOT_NODE);
}

static void HelpWindowBack(Widget w, XtPointer client_data, XtPointer call_data) {
  char *link;

  link=stringpop();
  helpWindowDisplayNode(link);
}

static void createHelpWindowWidgets() {
  static int done=0;
  char buf[256];

  Widget helpWindowPane, helpWindowBox;
  Widget helpWindowClose, helpWindowBack, helpWindowNext;
  Widget helpWindowContents;
  
  String trans = "<Btn1Down>: select-start() hypertext_linkselect()";

  if(done)
    return;
  done=1;
  
  zeroAddStrncpy(currentLink,ROOT_NODE,LINK_MAX_LEN-1);
  hyperInit(expandPathOfSupportFile(HYPER_FILE,buf,256));
  theNextNode=nextNode(currentLink);
  stringpush(currentLink);

  helpWindowShell = XtVaCreatePopupShell (
				      "helpWindowShell",
				      transientShellWidgetClass,
				      topLevel,
				      XtNtitle, "help",
				      NULL );
  
  helpWindowPane = XtVaCreateManagedWidget(
				      "helpWindowPane",
				      panedWidgetClass,
				      helpWindowShell,
				      XtNheight, 340,
				      XtNwidth, 490,
				      NULL);
  
  helpWindowBox = XtVaCreateManagedWidget(
				      "helpWindowBox",
				      boxWidgetClass,
				      helpWindowPane,
				      NULL);

  helpWindowClose = XtVaCreateManagedWidget(
				      "helpWindowClose",
				      commandWidgetClass,
				      helpWindowBox,
				      XtNlabel, "Close",
				      XtNwidth, BUTTON_WIDTH,
				      NULL);

  helpWindowContents = XtVaCreateManagedWidget(
				      "helpWindowContents",
				      commandWidgetClass,
				      helpWindowBox,
				      XtNlabel, "Home",
				      XtNwidth, BUTTON_WIDTH,
				      NULL);

  helpWindowBack = XtVaCreateManagedWidget(
				     "helpWindowBack",
				     commandWidgetClass,
				     helpWindowBox,
				     XtNlabel, "Back",
				     XtNwidth, BUTTON_WIDTH,
				     NULL);
  
  helpWindowNext = XtVaCreateManagedWidget(
				     "helpWindowNext",
				     commandWidgetClass,
				     helpWindowBox,
				     XtNlabel, "Next",
				     XtNwidth, BUTTON_WIDTH,
				     NULL);

  helpWindowNextLabel = XtVaCreateManagedWidget(
				     "helpWindowNextLabel",
				     labelWidgetClass,
				     helpWindowBox,
				     NULL);
  setnextNode();

  helpWindowText = XtVaCreateManagedWidget(
				     "helpWindowText",
				     asciiTextWidgetClass,
				     helpWindowPane,
				     XtNeditType, XawtextRead,
				     /* XtNdisplayCaret, False, */
				     XtNscrollVertical, XawtextScrollWhenNeeded,
				     XtNstring, (currentText=bodyText(ROOT_NODE)),
				     XtNtranslations, XtParseTranslationTable(trans),
				     NULL);

  XtAddCallback(helpWindowClose, XtNcallback, HelpWindowClose, topLevel);
  XtAddCallback(helpWindowNext, XtNcallback, HelpWindowNext, topLevel);
  XtAddCallback(helpWindowBack, XtNcallback, HelpWindowBack, topLevel);
  XtAddCallback(helpWindowContents, XtNcallback, HelpWindowContents, topLevel);
}
