/* $Id: graphBuffer.h,v 1.7 1996/02/26 08:51:43 lupus Exp $ */

/***************************************************************
 * This file is part of the Ginger project.                    *
 * Created 17-1-1996                                           *
 * Niels C. Larsen, Aalborg University, Denmark                *
 * (lupus@iesd.auc.dk)                                         *
 ***************************************************************/

#ifndef GRAPH_BUFFER_H
#define GRAPH_BUFFER_H

#include "graph.h"

/* ---  public functions  --- */

void graphBufferInitialize();
graph *graphBufferGetCurrentGraph();
char  *graphBufferGetCurrentFileName();
void graphBufferNewGraph();           
void graphBufferRedrawGraph();
void graphBufferLoadFile(char *fname);
void graphBufferInsertFile(char *fname);
void graphBufferSaveFile(char *fname);  /* "save as" */
void graphBufferSaveCurrentFile();      /* "save"    */
void graphBufferInitiateLoad();
void graphBufferInitiateInsert();
void graphBufferInitiateSaveAs();
void graphBufferInitiateExportPostScript();
void graphBufferProcessFileSelection(char *fname);

#endif
