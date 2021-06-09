/* $Id: allocmem.h,v 1.2 1995/12/28 21:52:04 lupus Exp $ */

/***************************************************************
 * This file is part of the Ginger project.                    *
 * Created 28-12-1995                                          *
 * Niels C. Larsen, Aalborg University, Denmark                *
 * (lupus@iesd.auc.dk)                                         *
 ***************************************************************/

#ifndef ALLOCMEM_H
#define ALLOCMEM_H

/* 'malloc' function with error checking */
void *AllocateMemory(unsigned size);

#endif
