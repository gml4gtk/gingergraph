/* $Id: actions.h,v 1.3 1996/03/27 16:42:01 lupus Exp $ */

/***************************************************************
 * This file is part of the Ginger project.                    *
 * Created 24-02-1996                                          *
 * Niels C. Larsen, Aalborg University, Denmark                *
 * (lupus@iesd.auc.dk)                                         *
 * Martin K. Molz, Aalborg University, Denmark                 *
 * (molz@iesd.auc.dk)                                          *
 ***************************************************************/

#ifndef ACTIONS_H
#define ACTIONS_H

#include <X11/Intrinsic.h>

void NodeDragStartAction(Widget w, XEvent *event, 
			 String *params, Cardinal *num_params);
void NodeDragAction(Widget w, XEvent *event, 
		    String *params, Cardinal *num_params);
void NodeDragEndAction(Widget w, XEvent *event, 
		 String *params, Cardinal *num_params);

void HotspotSetAction(Widget w, XEvent *event, 
			      String *params, Cardinal *num_params);

void SelectionStartAction(Widget w, XEvent *event, 
			  String *params, Cardinal *num_params);
void SelectionDragAction(Widget w, XEvent *event, 
			 String *params, Cardinal *num_params);
void SelectionEndAction(Widget w, XEvent *event, 
			String *params, Cardinal *num_params);
void SelectionAddEndAction(Widget w, XEvent *event, 
			   String *params, Cardinal *num_params);

void NodeSelectionDragStartAction(Widget w, XEvent *event, 
			  String *params, Cardinal *num_params);
void NodeSelectionDragAction(Widget w, XEvent *event, 
			 String *params, Cardinal *num_params);
void NodeSelectionDragEndAction(Widget w, XEvent *event, 
			String *params, Cardinal *num_params);
void NodeSelectionDragAddEndAction(Widget w, XEvent *event, 
			   String *params, Cardinal *num_params);

void ElkClickAction(Widget w, XEvent *event, 
		    String *params, Cardinal *num_params);
void ElkNodeDragAction(Widget w, XEvent *event, 
		       String *params, Cardinal *num_params);
void ElkNodeDragStartAction(Widget w, XEvent *event, 
			    String *params, Cardinal *num_params);

void actionsReset();

#endif
