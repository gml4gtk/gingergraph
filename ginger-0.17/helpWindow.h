/* $Id: helpWindow.h,v 1.2 1996/01/30 16:12:18 lupus Exp $ */

/***************************************************************
 * This file is part of the Ginger project.                    *
 * Created 27-1-1996                                           *
 * Niels C. Larsen, Aalborg University, Denmark                *
 * (lupus@iesd.auc.dk)                                         *
 ***************************************************************/

#ifndef HELP_WINDOW_H
#define HELP_WINDOW_H

void helpWindowPopup(Widget w, XtPointer client_data, XtPointer call_data);
void helpWindowLinkSelection(Widget w, XEvent *event, 
			   String *params, Cardinal *num_params);
void helpWindowDisplayNode(char *helpl);

#endif
