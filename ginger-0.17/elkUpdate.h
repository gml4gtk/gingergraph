/* $Id: elkUpdate.h,v 1.2 1996/03/28 16:33:45 lupus Exp $ */
  
/***************************************************************
 * This file is part of the Ginger project.                    *
 * Created 27-03-1996                                          *
 * Niels C. Larsen, Aalborg University, Denmark                *
 * (lupus@iesd.auc.dk)                                         *
 * Martin K. Molz, Aalborg University, Denmark                 *
 * (molz@iesd.auc.dk)                                          *
 ***************************************************************/

#ifndef ELK_UPDATE_H
#define ELK_UPDATE_H

#include "graph.h"

void elkNodeClick(node *n);
void elkEdgeClick(edge *e);
void elkNodeMove(node *n, int newx, int newy);
void elkSelectedNodesMoved(graph *g);
void elkNodeSelect(node *n, int selected);
void elkEdgeSelect(edge *e, int selected);
void elkUnselectAllObjects(graph *g);
void elkUnselectAllEdges(graph *g);
void elkUnselectAllNodes(graph *g);
void elkNodePrepareSelect(graph *g, int selection_state);
void elkEdgePrepareSelect(graph *g, int selection_state);

#endif
