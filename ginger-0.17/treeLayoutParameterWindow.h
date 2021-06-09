/* $Id: treeLayoutParameterWindow.h,v 1.2 1996/02/20 10:17:56 lupus Exp $ */

/***************************************************************
 * This file is part of the Ginger project.                    *
 * Created 16-02-1996                                          *
 * Niels C. Larsen, Aalborg University, Denmark                *
 * (lupus@iesd.auc.dk)                                         *
 * Martin K. Molz, Aalborg University, Denmark                 *
 * (molz@iesd.auc.dk)                                          *
 ***************************************************************/
 
#ifndef TREE_LAYOUT_PARAMETER_WINDOW_H
#define TREE_LAYOUT_PARAMETER_WINDOW_H

void treeLayoutParameterWindowPopup(Widget w, XtPointer client_data, XtPointer call_data);
void treeLayoutParameterWindowSetDirection(int d);
void treeLayoutParameterWindowHotspotChange(int x, int y);

#endif
