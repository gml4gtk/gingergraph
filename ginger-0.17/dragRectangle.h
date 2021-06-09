/* $Id: dragRectangle.h,v 1.1 1996/02/26 08:51:43 lupus Exp $ */

/***************************************************************
 * This file is part of the Ginger project.                    *
 * Created 24-02-1996                                          *
 * Niels C. Larsen, Aalborg University, Denmark                *
 * (lupus@iesd.auc.dk)                                         *
 * Martin K. Molz, Aalborg University, Denmark                 *
 * (molz@iesd.auc.dk)                                          *
 ***************************************************************/

#ifndef DRAG_RECTANGLE_H
#define DRAG_RECTANGLE_H

void dragRectangleInitialize();
void dragRectangleSetFixedPoint(int x, int y);
void dragRectangleSetPoint(int x, int y);
void dragRectangleDraw();
void dragRectangleUndraw();

#endif
