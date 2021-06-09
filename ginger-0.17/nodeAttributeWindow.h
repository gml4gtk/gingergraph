/* $Id: nodeAttributeWindow.h,v 1.3 1996/03/04 08:50:05 lupus Exp $ */

/***************************************************************
 * This file is part of the Ginger project.                    *
 * Created 12-02-1996                                          *
 * Niels C. Larsen, Aalborg University, Denmark                *
 * (lupus@iesd.auc.dk)                                         *
 * Martin K. Molz, Aalborg University, Denmark                 *
 * (molz@iesd.auc.dk)                                          *
 ***************************************************************/
 
#ifndef NODE_ATTRIBUTE_WINDOW_H
#define NODE_ATTRIBUTE_WINDOW_H

#include <X11/Intrinsic.h>
#include "node.h"

void nodeAttributeWindowPopup(Widget w, XtPointer client_data, XtPointer call_data);
void nodeAttributeWindowSetName(char *s);
void nodeAttributeWindowSetFGColor(char *s);
void nodeAttributeWindowSetShape(int opcode);
void nodeAttributeWindowDisplayNode(node *n);
void nodeAttributeWindowApplyToNode(node *n);
void nodeAttributeWindowReset();
void nodeAttributeWindowHotspotChange(int x, int y);

#endif
