/* $Id: edgeAttributeWindow.h,v 1.2 1996/03/04 08:50:05 lupus Exp $ */

/***************************************************************
 * This file is part of the Ginger project.                    *
 * Created 11-02-1996                                          *
 * Niels C. Larsen, Aalborg University, Denmark                *
 * (lupus@iesd.auc.dk)                                         *
 * Martin K. Molz, Aalborg University, Denmark                 *
 * (molz@iesd.auc.dk)                                          *
 ***************************************************************/
 
#ifndef EDGE_ATTRIBUTE_WINDOW_H
#define EDGE_ATTRIBUTE_WINDOW_H

#include "edge.h"

#include <X11/Intrinsic.h>

void edgeAttributeWindowPopup(Widget w, XtPointer client_data, XtPointer call_data);
void edgeAttributeWindowSetName(char *n);
void edgeAttributeWindowSetColor(char *n);
void edgeAttributeWindowSetDirection(int opcode);
void edgeAttributeWindowDisplayEdge(edge *e);
void edgeAttributeWindowApplyToEdge(edge *e);
void edgeAttributeWindowReset();

#endif
