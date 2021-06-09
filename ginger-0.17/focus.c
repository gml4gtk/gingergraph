/* $Id: focus.c,v 1.4 1996/02/13 09:18:58 lupus Exp $ */

/***************************************************************
 * This file is part of the Ginger project.                    *
 * Created 18-1-1996                                           *
 * Niels C. Larsen, Aalborg University, Denmark                *
 * (lupus@iesd.auc.dk)                                         *
 ***************************************************************/

/* Functions to establish keyboard control of the input focus 
   between Athena text widgets */

/* ---  include files  --- */

#include "clist.h"            /* circular linked list */
#include "focus.h"

#include <X11/Xaw/Text.h>
#include <X11/Xaw/AsciiText.h> 

/* ---  private declarations  --- */

static CLIST *pairList;
static int group;
static int focusMethod=FOCUS_MOUSE;  /* use mouse focus as default */

static void displayAllCarets();
static void setFocus(Widget root, Widget target, int fgroup);
static focus_pair *nextField(Widget currentField);
static focus_pair *prevField(Widget currentField);

/* ---  public functions  --- */

void initFocus() {
  static int done=0;
  
  if(done)
    return;
  done=1;
  
  pairList=newlist(STRUCT,sizeof(focus_pair)); /* create linked list */
  group=0;
}

int newGroup() {
  return ++group;
}

int currentGroup() {
  return group;
}

void setFocusMethod(int focusMeth) {
  initFocus();  /* make sure list is initialized */
  focusMethod=focusMeth;
  if( focusMethod==FOCUS_MOUSE )
    displayAllCarets();
}

void removeCarets(int fgroup) {
  focus_pair *fp;
  int i;
  
  for(i=1; i<=pairList->size; i++) {
    fp=(focus_pair *)retrieve(i,pairList);
    if(fp->group==fgroup)
      XtVaSetValues(fp->from,
		    XtNdisplayCaret, False,
		    NULL );
  }
}

void saveFocusInfo(Widget from, Widget to, Widget root, 
		   Widget confirm, int newgroup) {
  focus_pair fp;
  
  fp.from = from;
  fp.to   = to;
  fp.root = root;
  fp.confirmButton = confirm;
  if(newgroup) {
    fp.group=newGroup();
  }
  else
    fp.group=currentGroup();
  
  insert_last((BYTE *)&fp, pairList);
}

void jumpNext(Widget currentField) {
  focus_pair *next;

  next=nextField(currentField);
  if(next) {
    setFocus(next->root, next->to, next->group);
  }
}

void jumpPrev(Widget currentField) {
  focus_pair *prev;

  prev=prevField(currentField);
  if(prev) {
    setFocus(prev->root, prev->from, prev->group);
  }
}

void jumpTo(Widget field) {
  focus_pair *next;
  
  next=nextField(field);
  if(next) {
    setFocus(next->root, next->from, next->group);
  }
}

void confirmAction(Widget field) {
  focus_pair *next;
  Widget confirm;
  
  next=nextField(field);
  if(next) {
    confirm=next->confirmButton;
    if(confirm)
      XtCallCallbacks(confirm, XtNcallback, 0);
  }
}

/* ---  private functions  --- */

static void displayAllCarets() {
  focus_pair *fp;
  int i;
  
  for(i=1; i<=pairList->size; i++) {
    fp=(focus_pair *)retrieve(i,pairList);
    XtVaSetValues(fp->from,
		  XtNdisplayCaret, True,
		  NULL );
  }
}

static void setFocus(Widget root, Widget target, int fgroup) {
  Position x,y;
    
  if( focusMethod==FOCUS_KEYB ) {
    removeCarets(fgroup);
    XtSetKeyboardFocus(root, target); 
    
    XtVaSetValues(target,
		  XtNdisplayCaret, True,
		  NULL );
    return;
  }
}

static focus_pair *nextField(Widget currentField) {
  focus_pair *fp;
  int i;
  
  for(i=1; i<=pairList->size; i++) {
    fp=(focus_pair *)retrieve(i,pairList);
    if(fp->from==currentField)
      return fp;
  }
  
  return 0;
}

static focus_pair *prevField(Widget currentField) {
  focus_pair *fp;
  int i;
  
  for(i=1; i<=pairList->size; i++) {
    fp=(focus_pair *)retrieve(i,pairList);
    if(fp->to==currentField)
      return fp;
  }
  
  return 0;
}

