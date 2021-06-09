/* $Id: graphIO.h,v 1.3 1996/02/26 08:51:43 lupus Exp $ */

/***************************************************************
 * This file is part of the Ginger project.                    *
 * Created 1-1-1996                                            *
 * Niels C. Larsen, Aalborg University, Denmark                *
 * (lupus@iesd.auc.dk)                                         *
 ***************************************************************/

#ifndef GRAPH_IO_H
#define GRAPH_IO_H

#include <stdio.h>
#include "graph.h"

void printGraph(graph *g, FILE *fd);
graph *readGraph(FILE *fd);
graph *readAndMergeGraph(graph *g, FILE *fd);
void graphIOReadResult(graph *res);

#endif
