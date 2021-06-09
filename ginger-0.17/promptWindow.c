/* $Id: promptWindow.c,v 1.14 1996/03/27 09:04:49 lupus Exp $ */

/***************************************************************
 * This file is part of the Ginger project.                    *
 * Created 01-03-1996                                          *
 * Niels C. Larsen, Aalborg University, Denmark                *
 * (lupus@iesd.auc.dk)                                         *
 * Martin K. Molz, Aalborg University, Denmark                 *
 * (molz@iesd.auc.dk)                                          *
 ***************************************************************/

#include "config.h"

#if USE_SCHEME   /* if not, skip the rest of this file */

#include <X11/Intrinsic.h>
#include <X11/StringDefs.h>
#include <X11/Shell.h>

#include <X11/Xaw/Command.h>
#include <X11/Xaw/Paned.h>
#include <X11/Xaw/Text.h>
#include <X11/Xaw/Box.h>
#include <X11/Xaw/AsciiText.h>

#include <string.h>
#include "promptWindow.h"
#include "str.h"
#include "elkInterface.h"
#include "main.h"

/* ---  private declarations  --- */

extern Widget topLevel;  /* from mainWindow.c */

#define BUTTON_WIDTH    60
#define PWINDOW_WIDTH  400
#define PWINDOW_HEIGHT 200

static XawTextPosition current_pos;

static int popup=0;
static Widget promptWindowShell, promptWindowText;

static char history_buf[256];
static int  history_cursor=0;

/* private functions prototypes */
static void createPromptWindowWidgets();
static void PromptWindowClose(Widget w, XtPointer client_data, XtPointer call_data);
static void PromptWindowClear(Widget w, XtPointer client_data, XtPointer call_data);
static void promptWindowGotoEOF();
static void promptWindowLoadSchemeCode();

/* ---  public functions  --- */

void promptWindowPopup(Widget w, XtPointer client_data, XtPointer call_data) {
  if( !popup ) {
    createPromptWindowWidgets();
    
    XtPopup(promptWindowShell, XtGrabNone);
    popup=1;
  }
}

void promptWindowClear(Widget w, XtPointer client_data, XtPointer call_data) {
  createPromptWindowWidgets(); /* make sure widgets are initialized */
  
  XtVaSetValues(promptWindowText,       
		XtNstring, ">",
		NULL);
  
  promptWindowGotoEOF();
  promptWindowInteractionStart();
}

void promptWindowHistoryBackAction(Widget w, XEvent *event, 
				   String *params, Cardinal *num_params) {
  createPromptWindowWidgets(); /* make sure widgets are initialized */

  if( history_cursor==1 ) {
    promptWindowAppendText(history_buf);
    promptWindowGotoEOF();
    history_cursor=0;
  }
}

void promptWindowConfirmAction(Widget w, XEvent *event, 
			       String *params, Cardinal *num_params) {
  char buf[256], buf2[250];
  char *expr, *res;
  
  createPromptWindowWidgets(); /* make sure widgets are initialized */
  
  expr=promptWindowGetLine(buf,256);
  
  if(strlen(expr)==0)
    return;

  zeroAddStrncpy(history_buf, expr, 256);
  history_cursor=1;

  if(!strcmp(expr, "(exit)")) {
    PromptWindowClose(0,0,0);
    promptWindowAppendText("\n>");
    promptWindowGotoEOF();
    promptWindowInteractionStart();
    return;
  }

  res=elkInterfaceEvalString(expr);
  
  if(res) {
    zeroAddStrncpy(buf2, res, 250);
    sprintf(buf, "\n%s\n", buf2);
  }
  else {
    sprintf(buf, "");               /* do nothing, a message has been
                                        written by the error handler */
  }
  promptWindowAppendText(buf);

  promptWindowAppendText(">");
  promptWindowGotoEOF();
  promptWindowInteractionStart();
}

/* remember the current cursor position */
void promptWindowInteractionStart() {
  createPromptWindowWidgets(); /* make sure widgets are initialized */
  
  current_pos=XawTextGetInsertionPoint(promptWindowText);
}

/* return the text written in the window after the cursor position
   at the most recent call to promptWindowInteractionStart() */
char *promptWindowGetLine(char *return_buf, int buf_size) {
  String p;
  int length;
  
  createPromptWindowWidgets(); /* make sure widgets are initialized */
  
  XtVaGetValues(promptWindowText,       
		XtNstring, &p,
		NULL);
  
  length=strlen(p);
  if(current_pos<length)
    zeroAddStrncpy(return_buf, p+current_pos, buf_size);
  else
    return_buf[0]='\0';
  
  return return_buf;
}

/* insert s at the current cursor position */
void promptWindowInsertText(char *s) {
  XawTextPosition pos;
  XawTextBlock block;

  createPromptWindowWidgets(); /* make sure widgets are initialized */

  block.firstPos=0;
  block.length=strlen(s);
  block.ptr=s;
  block.format=FMT8BIT;
  
  pos=XawTextGetInsertionPoint(promptWindowText);
  XawTextReplace(promptWindowText, pos, pos, &block);
}

/* append s to the window contents */
void promptWindowAppendText(char *s) {
  createPromptWindowWidgets(); /* make sure widgets are initialized */
  
  promptWindowGotoEOF();
  promptWindowInsertText(s);
}

/* ---  private functions  --- */

static void PromptWindowClose(Widget w, XtPointer client_data, XtPointer call_data) {
  XtPopdown(promptWindowShell);
  popup=0;
}

/* return the length of the window text */
static int promptWindowTotalTextLength() {
  String p;
  
  XtVaGetValues(promptWindowText,       
		XtNstring, &p,
		NULL);

  return strlen(p);
}

/* move the insertion caret to the end of the window text */
static void promptWindowGotoEOF() {
  XawTextSetInsertionPoint(promptWindowText, promptWindowTotalTextLength());
}

static void createPromptWindowWidgets() {
  static int done=0;
  char graphScmPath[1024];

  
  Widget promptWindowPane, promptWindowBox;
  Widget promptWindowCloseButton, promptWindowClearButton;

  String trans = "<Key>Return: prompt-confirm-action() \n\
                  <Key>Up: prompt-back-action()";

  if(done)
    return;
  done=1;
  
  history_buf[0]='\0';
  
  promptWindowShell = XtVaCreatePopupShell (
				      "promptWindow",
				      transientShellWidgetClass,
				      topLevel,
				      XtNtitle, "Scheme prompt",
				      NULL );
  
  promptWindowPane = XtVaCreateManagedWidget(
				      "promptWindowPane",
				      panedWidgetClass,
				      promptWindowShell,
				      XtNheight, PWINDOW_HEIGHT,
				      XtNwidth,  PWINDOW_WIDTH,
				      NULL);
  
  promptWindowBox = XtVaCreateManagedWidget(
				      "promptWindowBox",
				      boxWidgetClass,
				      promptWindowPane,
				      NULL);

  promptWindowCloseButton = XtVaCreateManagedWidget(
				      "promptWindowClose",
				      commandWidgetClass,
				      promptWindowBox,
				      XtNlabel, "Close",
				      XtNwidth, BUTTON_WIDTH,
				      NULL);

  promptWindowClearButton = XtVaCreateManagedWidget(
				      "promptWindowClear",
				      commandWidgetClass,
				      promptWindowBox,
				      XtNlabel, "Clear",
				      XtNwidth, BUTTON_WIDTH,
				      NULL);

  promptWindowText = XtVaCreateManagedWidget(
				     "promptWindowText",
				     asciiTextWidgetClass,
				     promptWindowPane,
				     XtNeditType, XawtextEdit,
				     XtNscrollVertical, XawtextScrollAlways,
				     XtNstring, ">",
				     NULL);

  XtOverrideTranslations(promptWindowText, XtParseTranslationTable(trans));

  XtAddCallback(promptWindowCloseButton, XtNcallback, PromptWindowClose, topLevel);
  XtAddCallback(promptWindowClearButton, XtNcallback, promptWindowClear, topLevel);

  /* Uploading the basic graph features */
/*
  expandPathOfSupportFile("/Scheme/graph.scm",graphScmPath,sizeof(graphScmPath));
  promptWindowLoadSchemeCode(graphScmPath);
*/
  
  promptWindowGotoEOF();
  promptWindowInteractionStart();

}

/* load Scheme initialization code */
static void promptWindowLoadSchemeCode(char *schemeCodePath) {
  char *string_to_eval;

  string_to_eval = (char*)AllocateMemory(20+strlen(schemeCodePath));
  strcpy(string_to_eval, "(load \"" );
  strcat(string_to_eval, schemeCodePath);
  strcat(string_to_eval, "\" )");
  /* printf("%s \n", string_to_eval); */
  elkInterfaceEvalString(string_to_eval); 
  free(string_to_eval);
}

#endif

