/* $Id: stringstack.h,v 1.2 1996/01/30 16:12:18 lupus Exp $ */

/***************************************************************
 * This file is part of the Ginger project.                    *
 * Created 27-1-1996                                           *
 * Niels C. Larsen, Aalborg University, Denmark                *
 * (lupus@iesd.auc.dk)                                         *
 ***************************************************************/

#ifndef STRINGSTACK_H
#define STRINGSTACK_H

#define STACKSIZE 25

void stringpush(char *s);
char *stringpop();

#endif
