/* $Id: parser.h,v 1.1 1996/02/26 08:51:43 lupus Exp $ */

/***************************************************************
 * This file is part of the Ginger project.                    *
 * Created 24-02-1996                                          *
 * Niels C. Larsen, Aalborg University, Denmark                *
 * (lupus@iesd.auc.dk)                                         *
 * Martin K. Molz, Aalborg University, Denmark                 *
 * (molz@iesd.auc.dk)                                          *
 ***************************************************************/

#ifndef PARSER_H
#define PARSER_H

#include "graph.h"

#define PARSER_MODE_NEW   0
#define PARSER_MODE_MERGE 1

void parserSetMode(int mode, graph *g);

#endif
