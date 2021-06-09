/* $Id: stringstack.c,v 1.2 1996/01/30 16:12:18 lupus Exp $ */

/***************************************************************
 * This file is part of the Ginger project.                    *
 * Created 27-1-1996                                           *
 * Niels C. Larsen, Aalborg University, Denmark                *
 * (lupus@iesd.auc.dk)                                         *
 ***************************************************************/

/* Implements a simple bounded stack of text strings.
   Used by the hypertext help system */

#include "str.h"
#include "stringstack.h"

/* ---  private declarations  --- */

#define TEXT_MAX_LENGTH 100

static char thestack[STACKSIZE][TEXT_MAX_LENGTH];
static int head=0;
static int num=0;

/* ---  public functions  --- */

void stringpush(char *s) {
  int i;
  zeroAddStrncpy(thestack[head],s,TEXT_MAX_LENGTH-1);
  if(num<STACKSIZE)
    num++;
  if(head<STACKSIZE-1)
    head++;
  else
    head=0;

/*  for(i=0; i<num; i++)
    printf("%d %s\n", head, thestack[i]);
  printf("\n");
*/
}

char *stringpop() {
  int i;
  char *res;

  if(num>0) {
    if(head>0)
      head--;
    else
      head=STACKSIZE-1;
    num--;
  }
  res=thestack[head];
  return res;
}

