/* $Id: elkInterface.h,v 1.1 1996/03/01 15:11:25 lupus Exp $ */
  
/***************************************************************
 * This file is part of the Ginger project.                    *
 * Created 01-03-1996                                          *
 * Niels C. Larsen, Aalborg University, Denmark                *
 * (lupus@iesd.auc.dk)                                         *
 * Martin K. Molz, Aalborg University, Denmark                 *
 * (molz@iesd.auc.dk)                                          *
 ***************************************************************/

#ifndef ELK_INTERFACE_H
#define ELK_INTERFACE_H

void elkInterfaceInitialize(int ac, char **av);
char *elkInterfaceEvalString(char* expr);

#endif
