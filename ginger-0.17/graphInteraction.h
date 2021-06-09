/* $Id: graphInteraction.h,v 1.2 1996/06/14 12:19:13 lupus Exp $ */

/***************************************************************
 * This file is part of the Ginger project.                    *
 * Created 11-02-1996                                          *
 * Niels C. Larsen, Aalborg University, Denmark                *
 * (lupus@iesd.auc.dk)                                         *
 * Martin K. Molz, Aalborg University, Denmark                 *
 * (molz@iesd.auc.dk)                                          *
 ***************************************************************/

#ifndef GRAPH_INTERACTION_H
#define GRAPH_INTERACTION_H

#include "graph.h"

/* create edges between all selected nodes. Do not create
   an extra edge between nodes which are connected already.
   Do not create an edge from a node to itself. 
   Select created edges in current accumulation mode */
int connectSelectedNodes(graph *g);


#endif
