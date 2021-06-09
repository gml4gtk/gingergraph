/* $Id: colormap.h,v 1.1 1996/02/02 22:27:45 lupus Exp $ */

/***************************************************************
 * This file is part of the Ginger project.                    *
 * Created 2-2-1996                                            *
 * Niels C. Larsen, Aalborg University, Denmark                *
 * (lupus@iesd.auc.dk)                                         *
 ***************************************************************/

#ifndef GINGER_COLORMAP_H
#define GINGER_COLORMAP_H

#include <X11/Xlib.h>

GC colormapAllocateColorGC(char *colorname);
void colormapDeallocateColorGC(char *colorname);

#endif
