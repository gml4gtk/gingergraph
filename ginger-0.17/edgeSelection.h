/* $Id: edgeSelection.h,v 1.3 1996/02/26 08:51:43 lupus Exp $ */
 
/***************************************************************
 * This file is part of the Ginger project.                    *
 * Created 10-02-1996                                          *
 * Niels C. Larsen, Aalborg University, Denmark                *
 * (lupus@iesd.auc.dk)                                         *
 * Martin K. Molz, Aalborg University, Denmark                 *
 * (molz@iesd.auc.dk)                                          *
 ***************************************************************/
 
#ifndef EDGE_SELECTION_H
#define EDGE_SELECTION_H

#include "edge.h"
#include "graph.h"

#ifndef ACCUMULATING_MODE
#define ACCUMULATING_MODE     1
#define NON_ACCUMULATING_MODE 0
#endif

void edgeSelect(graph *g, edge *e, int mode);
void edgeUnselect(graph *g, edge *e);
void edgeToggleSelection(graph *g, edge *e, int mode);
void edgeSelectAll(graph *g);
void edgeUnselectAll(graph *g);
void edgeSelectUnselected(graph *g, int mode);
int  edgeSelectCount(graph *g);

void edgeSelectInCurrentMode(graph *g, edge *e);
void edgeToggleSelectionInCurrentMode(graph *g, edge *e);
void edgeSelectUnselectedInCurrentMode(graph *g);

edge *getFirstSelectedEdge(graph *g);

#endif
