/* $Id: str.h,v 1.3 1996/02/20 10:38:30 lupus Exp $ */

/***************************************************************
 * This file is part of the Ginger project.                    *
 * Created 1-1-1996                                            *
 * Niels C. Larsen, Aalborg University, Denmark                *
 * (lupus@iesd.auc.dk)                                         *
 ***************************************************************/

#ifndef STR_H
#define STR_H

int cistrstr(char *s, char *t);
char *makePlain(char *s);
void zeroAddStrncpy(char *dst, char *src, int max);
double strToFloat(char *s);
void chop(char *s);
char *makeCleanString(char *s);
void sortStrings(char *array[], int array_size);

#endif
