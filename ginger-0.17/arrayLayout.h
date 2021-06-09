/* $Id: arrayLayout.h,v 1.1 1996/02/14 08:34:11 lupus Exp $ */
  
/***************************************************************
 * This file is part of the Ginger project.                    *
 * Created 09-02-1996                                          *
 * Niels C. Larsen, Aalborg University, Denmark                *
 * (lupus@iesd.auc.dk)                                         *
 * Martin K. Molz, Aalborg University, Denmark                 *
 * (molz@iesd.auc.dk)                                          *
 ***************************************************************/

 
#ifndef ARRAY_LAYOUT_H
#define ARRAY_LAYOUT_H

#include "graph.h"

void array_layout(graph *g, int origoX, int origoY, int distX, int distY, int numberInRow);
 
#endif
 
