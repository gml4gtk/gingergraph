/* $Id: options.h,v 1.3 1996/02/26 08:51:43 lupus Exp $ */

/***************************************************************
 * This file is part of the Ginger project.                    *
 * Created 27-1-1996                                           *
 * Niels C. Larsen, Aalborg University, Denmark                *
 * (lupus@iesd.auc.dk)                                         *
 ***************************************************************/

#ifndef OPTIONS_H
#define OPTIONS_H

#include <stdio.h>
#include "menu.h"

void initializeOptionsMenu(Widget activatingButton);
void saveOptions();
int toggleRenameAtInsert();
int getCurrentRenameAtInsertMode();

#endif
