/* $Id: promptWindow.h,v 1.4 1996/03/20 08:45:17 lupus Exp $ */

/***************************************************************
 * This file is part of the Ginger project.                    *
 * Created 01-03-1996                                          *
 * Niels C. Larsen, Aalborg University, Denmark                *
 * (lupus@iesd.auc.dk)                                         *
 * Martin K. Molz, Aalborg University, Denmark                 *
 * (molz@iesd.auc.dk)                                          *
 ***************************************************************/

#ifndef PROMPT_WINDOW_H
#define PROMPT_WINDOW_H

#include <X11/Intrinsic.h>

void promptWindowPopup(Widget w, XtPointer client_data, XtPointer call_data);
void promptWindowInteractionStart();
char *promptWindowGetLine(char *return_buf, int buf_size);
void promptWindowConfirmAction(Widget w, XEvent *event, 
			       String *params, Cardinal *num_params);
void promptWindowHistoryBackAction(Widget w, XEvent *event, 
			       String *params, Cardinal *num_params);
void promptWindowInteractionStart();
void promptWindowInsertText(char *s);
void promptWindowAppendText(char *s);

#endif
