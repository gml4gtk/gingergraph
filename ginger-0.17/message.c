/* $Id: message.c,v 1.1 1996/02/01 15:50:42 lupus Exp $ */

/***************************************************************
 * This file is part of the Ginger project.                    *
 * Created 1-2-1996                                            *
 * Niels C. Larsen, Aalborg University, Denmark                *
 * (lupus@iesd.auc.dk)                                         *
 ***************************************************************/

#include <string.h>
#include "mainWindow.h"
#include "str.h"
#include "message.h"

/* ---  private declarations  --- */

#define MESSAGE_MAX 256

static char the_message[MESSAGE_MAX];
static int initialized = 0;

/* ---  public functions  --- */

void messageDisplay(char *msg) {
  zeroAddStrncpy(the_message, msg, MESSAGE_MAX);
  initialized = 1;
  mainWindowShowMessage(the_message);
}

void messageAppend(char *msg) {
  
  if( !initialized )
    messageDisplay(msg);
  else {
    if( strlen(the_message)+strlen(msg)+1<MESSAGE_MAX )
      strcat(the_message, msg);
    mainWindowShowMessage(the_message);
  }
  
  initialized = 1;
}
