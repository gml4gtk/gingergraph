/* $Id: rectangles.h,v 1.3 1996/03/04 08:50:05 lupus Exp $ */

/***************************************************************
 * This file is part of the Ginger project.                    *
 * Created 29-12-1995                                          *
 * Niels C. Larsen, Aalborg University, Denmark                *
 * (lupus@iesd.auc.dk)                                         *
 ***************************************************************/

#ifndef RECTANGLES_H
#define RECTANGLES_H

#include <X11/Xlib.h>

int pointInRectangle(XPoint point, XPoint rect1, XPoint rect2);
int rectangleOverlap(XPoint r11, XPoint r12, XPoint r21, XPoint r22);


#endif
