/* $Id: circularLayoutParameterWindow.h,v 1.2 1996/02/20 10:17:56 lupus Exp $ */

/***************************************************************
 * This file is part of the Ginger project.                    *
 * Created 13-02-1996                                          *
 * Niels C. Larsen, Aalborg University, Denmark                *
 * (lupus@iesd.auc.dk)                                         *
 * Martin K. Molz, Aalborg University, Denmark                 *
 * (molz@iesd.auc.dk)                                          *
 ***************************************************************/
 
#ifndef CIRCULAR_LAYOUT_PARAMETER_WINDOW_H
#define CIRCULAR_LAYOUT_PARAMETER_WINDOW_H

void circularLayoutParameterWindowPopup(Widget w, XtPointer client_data, XtPointer call_data);
void circularLayoutParameterWindowHotspotChange(int x, int y);

#endif
