/* $Id: menuOpcodes.h,v 1.10 1996/03/01 15:12:53 lupus Exp $ */

/***************************************************************
 * This file is part of the Ginger project.                    *
 * Created 7-1-1996                                            *
 * Niels C. Larsen, Aalborg University, Denmark                *
 * (lupus@iesd.auc.dk)                                         *
 ***************************************************************/

#ifndef MENU_OPCODES_H
#define MENU_OPCODES_H

#define FILE_LOAD            1
#define FILE_INSERT          5
#define FILE_SAVE            10
#define FILE_NEW             15
#define FILE_SAVE_AS         20
#define FILE_EXPORT_POSTSCRIPT 25
#define FILE_QUIT            30

#define SELECTION_NODES      40
#define SELECTION_EDGES      45

#define OPTIONS_RENAME_AT_INSERT 50
#define OPTIONS_SAVE         60

#define LAYOUT_ARRAY         70
#define LAYOUT_TREE          80
#define LAYOUT_CIRCULAR      90

#define EDGE_DIRECTION_MENU_NORMAL      100
#define EDGE_DIRECTION_MENU_INVERSE     110
#define EDGE_DIRECTION_MENU_BOTH        120
#define EDGE_DIRECTION_MENU_NONE        130

#define NODE_SHAPE_MENU_RECTANGLE       200
#define NODE_SHAPE_MENU_RHOMBE          210
#define NODE_SHAPE_MENU_ELLIPSE         220
#define NODE_SHAPE_MENU_NONE            230

#define TREE_DIRECTION_MENU_VERTICAL    300
#define TREE_DIRECTION_MENU_HORIZONTAL  310

#define OPCODE_SCHEME_PROMPT            400

#endif
