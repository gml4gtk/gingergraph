/* $Id: icon.c,v 1.2 1996/01/17 22:24:56 lupus Exp $ */

/***************************************************************
 * This file is part of the Ginger project.                    *
 * Created 28-12-1995                                          *
 * Niels C. Larsen, Aalborg University, Denmark                *
 * (lupus@iesd.auc.dk)                                         *
 ***************************************************************/

#include <X11/Intrinsic.h>
#include <X11/StringDefs.h>
#include <X11/Shell.h>

/* include the bitmap data */
#include "icon_bitmap" 

/* create icon pixmap and assign it to the topLevel widget resource
   XtNiconPixmap */
void createIcon(Widget toplevel) {
  Pixmap icon_pm;
  
  icon_pm=XCreateBitmapFromData(XtDisplay(toplevel),
				RootWindowOfScreen(XtScreen(toplevel)),
				icon_bitmap_bits,
				icon_bitmap_width, icon_bitmap_height);

  XtVaSetValues(toplevel, XtNiconPixmap, icon_pm, NULL);
}
