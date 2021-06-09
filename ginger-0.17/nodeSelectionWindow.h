/* $Id: nodeSelectionWindow.h,v 1.2 1996/03/04 08:50:05 lupus Exp $ */

/***************************************************************
 * This file is part of the Ginger project.                    *
 * Created 7-2-1996                                            *
 * Niels C. Larsen, Aalborg University, Denmark                *
 * (lupus@iesd.auc.dk)                                         *
 ***************************************************************/

#ifndef NODE_SELECTION_WINDOW_H
#define NODE_SELECTION_WINDOW_H

#include <X11/Intrinsic.h>

void nodeSelectionWindowPopup(Widget w, XtPointer client_data, XtPointer call_data);
int nodeSelectionWindowGetAccumulationState();

#endif
