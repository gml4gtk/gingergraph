/* $Id: menu.h,v 1.3 1996/01/30 16:12:18 lupus Exp $ */

/***************************************************************
 * This file is part of the Ginger project.                    *
 * Created 7-1-1996                                            *
 * Niels C. Larsen, Aalborg University, Denmark                *
 * (lupus@iesd.auc.dk)                                         *
 ***************************************************************/

#ifndef MENU_H
#define MENU_H

#include <X11/Intrinsic.h>
#include <X11/StringDefs.h>
#include <X11/Shell.h>

#include <X11/Xaw/MenuButton.h> 
#include <X11/Xaw/SimpleMenu.h> 
#include <X11/Xaw/SmeBSB.h> 
#include <X11/Xaw/SmeLine.h> 

typedef struct {
  char *label;  /* menu item label */
  int active;   /* >0: normal, active entry
                    0: entry not shown
                   -1: indicates a line
                */
  int opcode;
  Widget button;/* menu item button widget */
} menu_item;

int createMenu(char *menuName, Widget activatingButton, 
		menu_item entries[]);
Pixmap menuButtonMark();
void menuSetMark(menu_item entries[], int opcode, int on_off);

#endif
