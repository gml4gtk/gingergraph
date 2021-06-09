/* $Id: main.h,v 1.4 1996/03/22 09:29:42 lupus Exp $ */

/***************************************************************
 * This file is part of the Ginger project.                    *
 * Created 7-1-1996                                            *
 * Niels C. Larsen, Aalborg University, Denmark                *
 * (lupus@iesd.auc.dk)                                         *
 ***************************************************************/

#ifndef GINGER_MAIN_H
#define GINGER_MAIN_H

char *supportFilePath();
char *expandPathOfSupportFile(char *support_file_name, 
			      char *return_buffer, int bufsize);
#endif

