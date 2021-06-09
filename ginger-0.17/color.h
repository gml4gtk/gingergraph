/* $Id: color.h,v 1.1 1996/02/02 22:27:45 lupus Exp $ */

/***************************************************************
 * This file is part of the Ginger project.                    *
 * Created 2-2-1996                                            *
 * Niels C. Larsen, Aalborg University, Denmark                *
 * (lupus@iesd.auc.dk)                                         *
 ***************************************************************/

#ifndef GINGER_COLOR_H
#define GINGER_COLOR_H

#define MAX_COLOR_NAME_LENGTH 40

typedef struct {
  int  is_used;
  char color_name[MAX_COLOR_NAME_LENGTH];
  GC   gc;
} color_info;

#endif
