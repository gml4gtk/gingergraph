/* $Id: menu.c,v 1.4 1996/01/30 16:12:18 lupus Exp $ */

/***************************************************************
 * This file is part of the Ginger project.                    *
 * Created 7-1-1996                                            *
 * Niels C. Larsen, Aalborg University, Denmark                *
 * (lupus@iesd.auc.dk)                                         *
 ***************************************************************/

/* ---  include files  --- */

#include "menu.h"
#include "menuDispatch.h"
#include "menuMark.h"
#include "menuCheckMark.h"

/* ---  private declarations  --- */

static int currentMenuId=0;

extern Widget topLevel;  /* in mainWindow.c */

static void updateMark(Widget w, int set);

/* ---  public functions  --- */

/* create a menu given an array of entries (menu_items)
   and bind the menu to the menu button "activatingButton" */
int createMenu(char *menuName, Widget activatingButton, 
		menu_item entries[]) {
  Widget theMenu, menuEntry;
  String item;
  int i, res;

  currentMenuId++;
  
  theMenu = XtCreatePopupShell(menuName,
			       simpleMenuWidgetClass,
			       activatingButton,
			       NULL,0);
  
  for(i=0; (item=entries[i].label)!=0; i++) {
    if(entries[i].active>0) {
      menuEntry = entries[i].button = XtVaCreateManagedWidget(item, 
						 smeBSBObjectClass,
						 theMenu, 
						 XtNlabel, item, 
						 XtNleftMargin, 30,
						 XtNrightMargin, 10,
						 NULL);
      XtAddCallback(menuEntry, XtNcallback, MenuDispatch, 
		    (XtPointer) entries[i].opcode);
    } else if(entries[i].active==-1) { 
       /* line-entry */
      menuEntry = XtVaCreateManagedWidget(item, 
					  smeLineObjectClass,
					  theMenu, 
					  NULL);
    }
  }
  return currentMenuId;
}

void menuSetMark(menu_item entries[], int opcode, int on_off) {
  int i;
  for(i=0; entries[i].label!=0; i++)
    if(entries[i].opcode==opcode) {
      updateMark(entries[i].button,on_off);
      return;
    }
}

Pixmap menuButtonMark() {
  static int initialized = 0;
  static Pixmap res;
  
  if(initialized)
    return res;
  
  initialized = 1;
  res=XCreateBitmapFromData(XtDisplay(topLevel),
			    RootWindowOfScreen(XtScreen(topLevel)),
			    menuMark_bits, menuMark_width, 
			    menuMark_height);
  return res;
}

Pixmap menuCheckMark() {
  static int initialized = 0;
  static Pixmap res;
  
  if(initialized)
    return res;
  
  initialized = 1;
  res=XCreateBitmapFromData(XtDisplay(topLevel),
			    RootWindowOfScreen(XtScreen(topLevel)),
			    menuCheckMark_bits, menuCheckMark_width, 
			    menuCheckMark_height);
  return res;
}

/* ---  private functions  --- */

static void updateMark(Widget w, int set) {
  if(!w)
    return;
  if(set)
    XtVaSetValues(w, 
		  XtNleftBitmap, menuCheckMark(),
		  NULL);
  else
    XtVaSetValues(w, 
		  XtNleftBitmap, None,
		  NULL);
}

