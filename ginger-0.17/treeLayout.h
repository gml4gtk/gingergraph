/* $Id: treeLayout.h,v 1.1 1996/02/19 10:34:46 lupus Exp $ */
  
/***************************************************************
 * This file is part of the Ginger project.                    *
 * Created 15-02-1996                                          *
 * Niels C. Larsen, Aalborg University, Denmark                *
 * (lupus@iesd.auc.dk)                                         *
 * Martin K. Molz, Aalborg University, Denmark                 *
 * (molz@iesd.auc.dk)                                          *
 ***************************************************************/
 
#ifndef TREE_LAYOUT_H
#define TREE_LAYOUT_H

#include "graph.h"

void tree_layout(graph *g, int cornerX, int cornerY, int distX, int distY, int vertical, int crossingIterations, int placementIterations);

#endif

