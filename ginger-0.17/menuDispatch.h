/* $Id: menuDispatch.h,v 1.2 1996/01/30 16:12:18 lupus Exp $ */

/***************************************************************
 * This file is part of the Ginger project.                    *
 * Created 7-1-1996                                            *
 * Niels C. Larsen, Aalborg University, Denmark                *
 * (lupus@iesd.auc.dk)                                         *
 ***************************************************************/

#ifndef MENU_DISPATCH_H
#define MENU_DISPATCH_H

/* ---  include files  --- */

#include "menu.h"
#include "menuOpcodes.h"

/* ---  public functions  --- */

void MenuDispatch(Widget w, XtPointer client_data, XtPointer call_data);

#endif
