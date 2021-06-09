/* $Id: focus.h,v 1.3 1996/01/30 16:12:18 lupus Exp $ */

/***************************************************************
 * This file is part of the Ginger project.                    *
 * Created 18-1-1996                                           *
 * Niels C. Larsen, Aalborg University, Denmark                *
 * (lupus@iesd.auc.dk)                                         *
 ***************************************************************/

#ifndef FOCUS_H
#define FOCUS_H

#include <X11/Intrinsic.h>
#include <X11/StringDefs.h>
#include <X11/Shell.h>

#define FRAME_Y_OFFSET 25
#define FRAME_X_OFFSET 5
#define X_OFFSET 1
#define Y_OFFSET 12

#define FOCUS_MOUSE 0  /* "standard" Xaw follow-mouse input focus */
#define FOCUS_KEYB  1  /* keyboard controllet input focus */

typedef struct {
  Widget from;    /* entry field that looses focus by a "next" action 
                     happening in this field */
  Widget to;      /* entry field that receives the focus    */
  Widget root;
  Widget confirmButton; /* "button" activated upon confirm action 
			   (using XtCallCallbacks) */
  int group;      /* logical group of related entry fields  */
} focus_pair;

void initFocus();
int newGroup();
int currentGroup();
void saveFocusInfo(Widget from, Widget to, 
		   Widget root, Widget confirm, int newgroup);
void jumpNext(Widget currentField);
void jumpPrev(Widget currentField);
void jumpTo(Widget field);
void confirmAction(Widget field);
void setFocusMethod(int focusMeth);

#endif /* FOCUS_H */
