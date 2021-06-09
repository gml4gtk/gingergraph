/* $Id: allocmem.c,v 1.3 1996/01/17 22:24:56 lupus Exp $ */

/***************************************************************
 * This file is part of the Ginger project.                    *
 * Created 28-12-1995                                          *
 * Niels C. Larsen, Aalborg University, Denmark                *
 * (lupus@iesd.auc.dk)                                         *
 ***************************************************************/

#include <stdlib.h>
#include <stdio.h>

/* 'malloc' function with error checking */
void *AllocateMemory(unsigned size) {
  void *res;
  res=malloc(size);
  if( !res ) {
    fprintf(stderr, "Memory allocation error, terminating.\n");
    exit(1);
  } 
  return res;
}
