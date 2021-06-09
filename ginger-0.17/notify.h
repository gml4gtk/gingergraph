/* $Id: notify.h,v 1.3 1996/02/19 10:34:46 lupus Exp $ */

/***************************************************************
 * This file is part of the Ginger project.                    *
 * Created 28-1-1996                                           *
 * Niels C. Larsen, Aalborg University, Denmark                *
 * (lupus@iesd.auc.dk)                                         *
 ***************************************************************/

#ifndef NOTIFY_H
#define NOTIFY_H

typedef void (*notify_callback_type)(); /* notify_callback_type is a 
					   pointer-to-void-function 
					   type, ex.  void f(); */

/* basic functions */
void notifyDisplay(char *text);
void notifySetTitle(char *title);
void notifySetButtonLabel(int button, char *label);
void notifySetButtonCallback(int button, notify_callback_type f);
void notifySetButtonSensitive(int button, int mode);

/* "higher level" functions */
void notifyAskConfirm(char *text, 
		      notify_callback_type confirm, 
		      notify_callback_type cancel);
void notifyAndOfferHelp(char *text, char *hypertext_node);

#endif
