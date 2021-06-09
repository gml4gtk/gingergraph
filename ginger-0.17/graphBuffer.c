/* $Id: graphBuffer.c,v 1.13 1996/05/27 07:05:29 lupus Exp $ */

/***************************************************************
 * This file is part of the Ginger project.                    *
 * Created 17-1-1996                                           *
 * Niels C. Larsen, Aalborg University, Denmark                *
 * (lupus@iesd.auc.dk)                                         *
 ***************************************************************/

/* Functions to manipulate the current graph */

#include <stdio.h>
#include <string.h>
#include "fileSelector.h"
#include "graphBuffer.h"
#include "graphDisplay.h"
#include "graphIO.h"
#include "mainWindow.h"
#include "message.h"
#include "PostScript.h"
#include "notify.h"
#include "actions.h"

/* ---  global declarations  --- */

graph *the_graph = 0;      /* the current graph */

/* ---  private declarations  --- */

#define GB_LOAD        0
#define GB_SAVE_AS     1
#define GB_EXPORT_PS   2
#define GB_INSERT      3

static int file_operation; /* indicates whether the file selector
			      is activated for a "load", "save as" 
			      "insert" etc. operation */

/* file name of graph in buffer */
static char the_file_name[FS_MAX_DIR_PATH_LENGTH];

/* temporary filename variable for internal use */
static char *filename;
static char temp_file_name[FS_MAX_DIR_PATH_LENGTH];

/* private function prototypes */
static void graphBufferUpdateTitle(char *fname);
static void graphBufferClear();
static void graphBufferSaveFileNoCheck();

/* ---  public functions  --- */

/* initialize graph buffer */
void graphBufferInitialize() {
  the_graph=createGraph(); /* create empty initial graph */
  the_file_name[0]='\0';   /* set file name to ""        */
}

/* return a pointer to the current graph object */
graph *graphBufferGetCurrentGraph() {
  return the_graph;
}

/* return the current file name */
char  *graphBufferGetCurrentFileName() {
  return the_file_name;
}

/* reset graph buffer with a new empty graph */
void graphBufferNewGraph() {
  graphBufferClear();      /* destroy old graph and clear window */
  the_graph=createGraph(); /* create empty initial graph */
  the_file_name[0]='\0';   /* set file name to "" */
  graphBufferUpdateTitle("new file"); /* update window title */
  mainWindowSetGraphAreaSize(0,0);  /* set to default size */
  messageDisplay("new graph");
}

/* load graph from file */
void graphBufferLoadFile(char *fname) {
  FILE *fd;
  graph *new_graph;
  char buf[FS_MAX_DIR_PATH_LENGTH+50];
  int wid,hei;
  
  zeroAddStrncpy(temp_file_name, fname, FS_MAX_DIR_PATH_LENGTH);
  filename=temp_file_name;

  sprintf(buf, "loading graph %s...", fname);
  messageDisplay(buf);
  
  if ((fd = fopen(fname,"r")) == NULL) {
    sprintf(buf, "Can't open file \"%s\"", fname);
    messageAppend(" failed.");
    notifyAndOfferHelp(buf, "can't open file");
    return;
  }

  new_graph = readGraph(fd);  /* read new graph */
  if( new_graph ) {
    graphBufferClear();    /* destroy the "old" graph */
    zeroAddStrncpy(the_file_name, fname, FS_MAX_DIR_PATH_LENGTH);
    graphBufferUpdateTitle(fname); /* set new title */
    the_graph = new_graph;  
    /* determine new dimensions */
    graphMaxCoordinates(the_graph, &wid, &hei); 
    /* adjust the graph area size */
    mainWindowSetGraphAreaSize(wid+GRAPH_MARGIN,hei+GRAPH_MARGIN);
    /* display the new graph */
    displayGraph(the_graph); 
    messageAppend(" done.");
  }
  else
    messageAppend(" failed.");

  fclose(fd); /* close input file */
}

/* insert graph from file in current graph */
void graphBufferInsertFile(char *fname) {
  FILE *fd;
  graph *new_graph;
  char buf[FS_MAX_DIR_PATH_LENGTH+50];
  int wid,hei;
  
  zeroAddStrncpy(temp_file_name, fname, FS_MAX_DIR_PATH_LENGTH);
  filename=temp_file_name;
  
  sprintf(buf, "inserting graph %s...", fname);
  messageDisplay(buf);
  
  if ((fd = fopen(fname,"r")) == NULL) {
    sprintf(buf, "Can't open file \"%s\"", fname);
    messageAppend(" failed.");
    notifyAndOfferHelp(buf, "can't open file");
    return;
  }
  
  new_graph = readAndMergeGraph(the_graph, fd);  /* read new graph */
  if( new_graph ) {
    /* clear the graph area */
    mainWindowClearGraphArea();
    /* determine new dimensions */
    graphMaxCoordinates(the_graph, &wid, &hei); 
    /* adjust the graph area size */
    mainWindowSetGraphAreaSize(wid+GRAPH_MARGIN,hei+GRAPH_MARGIN);
    /* display the new graph */
    displayGraph(the_graph); 
    messageAppend(" done.");
  }
  else
    messageAppend(" failed.");
  
  fclose(fd); /* close input file */
}

/* redisplay the current graph */
void graphBufferRedrawGraph() {
  int wid, hei;
  
  /* clear the graph area */
  mainWindowClearGraphArea();
  /* determine graph dimensions */
  graphMaxCoordinates(the_graph, &wid, &hei); 
  /* adjust the graph area size */
  mainWindowSetGraphAreaSize(wid+GRAPH_MARGIN,hei+GRAPH_MARGIN);
  /* display the graph */
  displayGraph(the_graph); 
}

/* save buffer as a new name */
void graphBufferSaveFile(char *fname) {
  char buf[FS_MAX_DIR_PATH_LENGTH+50];
  FILE *f;

  zeroAddStrncpy(temp_file_name, fname, FS_MAX_DIR_PATH_LENGTH);
  filename=temp_file_name;

  /* check whether the file exists. If so, ask the user before
     overwriting it. */
  if( (f=fopen(fname, "r"))!=NULL ) {
    fclose(f);
    sprintf(buf, "Overwrite existing file \"%s\" ?", fname);
    notifyAskConfirm(buf,graphBufferSaveFileNoCheck,0);
  } 
  else 
    graphBufferSaveFileNoCheck();
}

/* export PostScript representation */
void graphBufferExportPostScript(char *fname) {
  char buf[FS_MAX_DIR_PATH_LENGTH+50];
  char buf2[FS_MAX_DIR_PATH_LENGTH+50];
  FILE *fd;
  
  zeroAddStrncpy(buf, fname, FS_MAX_DIR_PATH_LENGTH);
  
  /* add ".ps" to filename, if appropriate */
  if( !strstr(buf, ".ps") )
    strcat(buf, ".ps");
  
  /* maybe we should check whether the file exists before
     we overwrite it ?  */

  sprintf(buf2, "exporting PostScript to %s...", buf);
  messageDisplay(buf2);
  
  if ((fd = fopen(buf,"w")) == NULL) {
    sprintf(buf2, "Can't write file \"%s\"", buf);
    messageAppend(" failed.");
    notifyAndOfferHelp(buf2, "can't write file");
    return;
  }

  PostScriptGraph(the_graph, fd);

  fclose(fd); /* close output file */
  messageAppend(" done.");
}

/* save the current file; only ask for a filename if the graph
   doesn't have a file name already */
void graphBufferSaveCurrentFile() {
  if( strlen(the_file_name) > 0 ) {
    filename=the_file_name;
    graphBufferSaveFileNoCheck();
  }
  else
    graphBufferInitiateSaveAs();
}

/* open the file selector to prompt for a file name to load */
void graphBufferInitiateLoad() {
  file_operation = GB_LOAD;
  fileSelectorSetTitle("Load graph");
  FileSelectorPopup(0,0,0);
}

/* open the file selector to prompt for a file name to insert */
void graphBufferInitiateInsert() {
  file_operation = GB_INSERT;
  fileSelectorSetTitle("Insert graph");
  FileSelectorPopup(0,0,0);
}

/* open the file selector to prompt for a file name to save */
void graphBufferInitiateSaveAs() {
  file_operation = GB_SAVE_AS;
  fileSelectorSetTitle("Save graph as");
  FileSelectorPopup(0,0,0);
}

void graphBufferInitiateExportPostScript() {
  file_operation = GB_EXPORT_PS;
  fileSelectorSetTitle("Export PostScript to file");
  FileSelectorPopup(0,0,0);
}

/* receive a file name from the file selector in response to 
   either a graphBufferInitiateLoad() or a graphBufferInitiateSaveAs() */
void graphBufferProcessFileSelection(char *fname) {
  switch(file_operation) {
  case GB_LOAD:
    graphBufferLoadFile(fname);
    break;
  case GB_INSERT:
    graphBufferInsertFile(fname);
    break;
  case GB_SAVE_AS:
    graphBufferSaveFile(fname);
    break;
  case GB_EXPORT_PS:
    graphBufferExportPostScript(fname);
    break;
  }
}

/* ---  private functions  --- */

static void graphBufferUpdateTitle(char *fname) {
  char buf[FS_MAX_DIR_PATH_LENGTH];
  char buf2[FS_MAX_DIR_PATH_LENGTH];

  if( strlen(fname)>0 ) {
    zeroAddStrncpy(buf2, fname, FS_MAX_DIR_PATH_LENGTH-strlen("ginger -  "));
    sprintf(buf, "ginger - %s", buf2);
  }
  else
    sprintf(buf, "ginger");
  
  mainWindowSetTitle(buf);
}

/* clear graph buffer */
static void graphBufferClear() {
  if(the_graph)
    destroyGraph(the_graph);
  mainWindowClearGraphArea();
  actionsReset(); /* reset any references in the actions module */
}

/* save graph to file; don't check whether the file exists */
static void graphBufferSaveFileNoCheck() {
  FILE *fd;
  char *fname;
  char buf[FS_MAX_DIR_PATH_LENGTH+50];

  fname=filename;  /* filename was set by graphBufferSaveFile() 
		      or by graphBufferSaveCurrentFile() */

  sprintf(buf, "saving graph %s...", fname);
  messageDisplay(buf);
  
  if ((fd = fopen(fname,"w")) == NULL) {
    sprintf(buf, "Can't write file \"%s\"", fname);
    messageAppend(" failed.");
    notifyAndOfferHelp(buf, "can't write file");
    return;
  }

  zeroAddStrncpy(the_file_name, fname, FS_MAX_DIR_PATH_LENGTH);
  graphBufferUpdateTitle(fname);
  
  printGraph(the_graph, fd);
  
  fclose(fd); /* close output file */
  messageAppend(" done.");
}

