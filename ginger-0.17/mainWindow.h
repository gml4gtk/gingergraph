/* $Id: mainWindow.h,v 1.7 1996/02/01 15:50:42 lupus Exp $ */

/***************************************************************
 * This file is part of the Ginger project.                    *
 * Created 27-12-1995                                          *
 * Niels C. Larsen, Aalborg University, Denmark                *
 * (lupus@iesd.auc.dk)                                         *
 ***************************************************************/

#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <X11/Intrinsic.h>

/* ---  application resources  --- */

#define NnodeColor "nodeColor"
#define CnodeColor "NodeColor"
#define NedgeColor "edgeColor"
#define CedgeColor "EdgeColor"
#define NlabelColor "labelColor"
#define ClabelColor "LabelColor"
#define NlabelFont "labelFont"
#define ClabelFont "LabelFont"

typedef struct {
  Pixel default_fg;
  Pixel default_bg;
  Pixel node_fg;
  Pixel edge_fg;
  Pixel label_fg;
  Font  label_font;
  XFontStruct *label_font_struct;
} ApplicationResources, *ApplicationResourcesPtr;

void mainWindowInitialize(int argc, char **argv);
void mainWindowXtLoop(void);
void mainWindowSetTitle(char *title);
void mainWindowClearGraphArea();
void mainWindowSetGraphAreaSize(int new_width, int new_height);
void mainWindowShowMessage(char *msg);

#endif
