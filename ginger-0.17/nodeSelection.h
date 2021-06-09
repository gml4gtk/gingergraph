/* $Id: nodeSelection.h,v 1.5 1996/02/26 08:51:43 lupus Exp $ */
 
/***************************************************************
 * This file is part of the Ginger project.                    *
 * Created 07-02-1996                                          *
 * Niels C. Larsen, Aalborg University, Denmark                *
 * (lupus@iesd.auc.dk)                                         *
 * Martin K. Molz, Aalborg University, Denmark                 *
 * (molz@iesd.auc.dk)                                          *
 ***************************************************************/
 
#ifndef NODE_SELECTION_H
#define NODE_SELECTION_H

#include "node.h"
#include "graph.h"

#ifndef ACCUMULATING_MODE
#define ACCUMULATING_MODE     1
#define NON_ACCUMULATING_MODE 0
#endif

void nodeSelect(graph *g, node *n, int mode);
void nodeUnselect(graph *g, node *n);
void nodeToggleSelection(graph *g, node *n, int mode);
void nodeSelectAll(graph *g);
void nodeUnselectAll(graph *g);
void nodeSelectUnselected(graph *g, int mode);
int  nodeSelectCount(graph *g);
node *getFirstSelectedNode(graph *g);

#endif
