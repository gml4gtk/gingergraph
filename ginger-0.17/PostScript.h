/* $Id: PostScript.h,v 1.3 1996/01/30 16:12:18 lupus Exp $ */

/***************************************************************
 * This file is part of the Ginger project.                    *
 * Created 20-1-1996                                           *
 * Niels C. Larsen, Aalborg University, Denmark                *
 * (lupus@iesd.auc.dk)                                         *
 ***************************************************************/

#ifndef GINGER_POSTSCRIPT_H
#define GINGER_POSTSCRIPT_H

#include <stdio.h>
#include "graph.h"

void PostScriptNode(node *n, FILE *fd);
void PostScriptEdge(edge *e, FILE *fd);
void PostScriptGraph(graph *g, FILE *fd);

#endif
