/* $Id: menuDispatch.c,v 1.13 1996/03/04 08:50:05 lupus Exp $ */

/***************************************************************
 * This file is part of the Ginger project.                    *
 * Created 7-1-1996                                            *
 * Niels C. Larsen, Aalborg University, Denmark                *
 * (lupus@iesd.auc.dk)                                         *
 ***************************************************************/

/* ---  include files  --- */

#include "menuDispatch.h"

#include "nodeSelectionWindow.h"
#include "edgeSelectionWindow.h"
#include "nodeAttributeWindow.h"
#include "edgeAttributeWindow.h"
#include "circularLayoutParameterWindow.h"
#include "arrayLayoutParameterWindow.h"
#include "treeLayoutParameterWindow.h"
#include "promptWindow.h"
#include "graphBuffer.h"
#include "config.h"
#include "options.h"

/* ---  public functions  --- */

void MenuDispatch(Widget w, XtPointer client_data, XtPointer call_data) {
  int opcode=(int) client_data;

  switch(opcode) {
  case FILE_LOAD:
    graphBufferInitiateLoad();
    break;
  case FILE_INSERT:
    graphBufferInitiateInsert();
    break;
  case FILE_SAVE:
    graphBufferSaveCurrentFile();
    break;
  case FILE_SAVE_AS:
    graphBufferInitiateSaveAs();
    break;
  case FILE_NEW:
    graphBufferNewGraph();
    break;
  case FILE_EXPORT_POSTSCRIPT:
    graphBufferInitiateExportPostScript();
    break;
  case FILE_QUIT:
    exit(0);
  case OPTIONS_RENAME_AT_INSERT:
    toggleRenameAtInsert();
    break;
  case OPTIONS_SAVE:
    saveOptions();
    break;
  case SELECTION_NODES:
    nodeSelectionWindowPopup(0,0,0);
    break;
  case SELECTION_EDGES:
    edgeSelectionWindowPopup(0,0,0);
    break;
  case EDGE_DIRECTION_MENU_NORMAL:
  case EDGE_DIRECTION_MENU_INVERSE:
  case EDGE_DIRECTION_MENU_BOTH:
  case EDGE_DIRECTION_MENU_NONE:
    edgeAttributeWindowSetDirection(opcode);
    break;
  case NODE_SHAPE_MENU_RECTANGLE:
  case NODE_SHAPE_MENU_RHOMBE:
  case NODE_SHAPE_MENU_ELLIPSE:
  case NODE_SHAPE_MENU_NONE:
    nodeAttributeWindowSetShape(opcode);
    break;
  case LAYOUT_CIRCULAR:
    circularLayoutParameterWindowPopup(0,0,0);
    break;
  case LAYOUT_ARRAY:
    arrayLayoutParameterWindowPopup(0,0,0);
    break;
  case LAYOUT_TREE:
    treeLayoutParameterWindowPopup(0,0,0);
    break;
  case TREE_DIRECTION_MENU_VERTICAL:
  case TREE_DIRECTION_MENU_HORIZONTAL:
    treeLayoutParameterWindowSetDirection(opcode);
    break;
#if USE_SCHEME
  case OPCODE_SCHEME_PROMPT:
    promptWindowPopup(0,0,0);
    break;
#endif
  }
}

