/* $Id: mathMacros.h,v 1.2 1996/02/12 13:31:05 lupus Exp $ */

/***************************************************************
 * This file is part of the Ginger project.                    *
 * Created 1-1-1996                                            *
 * Niels C. Larsen, Aalborg University, Denmark                *
 * (lupus@iesd.auc.dk)                                         *
 ***************************************************************/

#ifndef MATH_MACROS_H
#define MATH_MACROS_H

#include <math.h>

#define ABSOLUTE(x) ( ((x)<0.0) ? -(x):(x) )

#ifndef MIN
#define MIN(x,y)    ( ((x)<(y)) ? (x):(y) )
#endif

#ifndef MAX
#define MAX(x,y)    ( ((x)>(y)) ? (x):(y) )
#endif

#define DIST(x1,y1,x2,y2) ( sqrt(((x2)-(x1))*((x2)-(x1))+((y2)-(y1))*((y2)-(y1))) )

#endif
