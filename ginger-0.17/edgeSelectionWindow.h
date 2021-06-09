/* $Id: edgeSelectionWindow.h,v 1.2 1996/03/04 08:50:05 lupus Exp $ */

/***************************************************************
 * This file is part of the Ginger project.                    *
 * Created 10-02-1996                                          *
 * Niels C. Larsen, Aalborg University, Denmark                *
 * (lupus@iesd.auc.dk)                                         *
 * Martin K. Molz, Aalborg University, Denmark                 *
 * (molz@iesd.auc.dk)                                          *
 ***************************************************************/
 
#ifndef EDGE_SELECTION_WINDOW_H
#define EDGE_SELECTION_WINDOW_H

#include <X11/Intrinsic.h>

void edgeSelectionWindowPopup(Widget w, XtPointer client_data, XtPointer call_data);
int edgeSelectionWindowGetAccumulationState();

#endif
