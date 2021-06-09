/* $Id: hyper.h,v 1.2 1996/01/30 16:12:18 lupus Exp $ */

/***************************************************************
 * This file is part of the Ginger project.                    *
 * Created 27-1-1996                                           *
 * Niels C. Larsen, Aalborg University, Denmark                *
 * (lupus@iesd.auc.dk)                                         *
 ***************************************************************/

#ifndef HYPER_H
#define HYPER_H

#define LINK_MAX_LEN 80

typedef struct {
  char *name;
  char *nextNode;
  char *body;
  int linenum;
  int size;
} hypertext_node;

void hyperInit(char *hypertextfile);
char *linkAtPosition(char *bodyText, int position);
char *bodyText(char *nodename);
char *nextNode(char *nodename);

#endif
