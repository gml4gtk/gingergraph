/* $Id: main.c,v 1.14 1996/03/22 10:40:14 lupus Exp $ */

/***************************************************************
 * This file is part of the Ginger project.                    *
 * Created 27-12-1995                                          *
 * Niels C. Larsen, Aalborg University, Denmark                *
 * (lupus@iesd.auc.dk)                                         *
 ***************************************************************/

#include <stdlib.h>
#include <string.h>

#include "main.h"
#include "version.h"
#include "message.h"
#include "mainWindow.h"
#include "focus.h"
#include "config.h"
#include "elkInterface.h"
#include "str.h"
#include "mathMacros.h"

/* ---  private declarations  --- */

#define GINGER_PATH_MAX 1024
static char support_path[GINGER_PATH_MAX];

static void getEnvironmentVariables();
static void setSupportFilePath(char *p);

/* ---  public functions  --- */

char *supportFilePath() {
  return support_path;
}

char *expandPathOfSupportFile(char *support_file_name, 
			      char *return_buffer, int bufsize) {
  int l;
  
  zeroAddStrncpy(return_buffer, support_path, bufsize);
  l=strlen(return_buffer);
  zeroAddStrncpy(return_buffer+l, support_file_name, bufsize-l);
  
  return return_buffer;
}

/* ---  Ginger main function  --- */
main(int argc, char *argv[]) { 
  support_path[0]='\0';
  
  getEnvironmentVariables();
#if USE_SCHEME
  elkInterfaceInitialize(argc,argv);
#endif
  graphBufferInitialize();              /* initialize graph   */
  mainWindowInitialize(argc,argv);      /* create main window */
  messageDisplay(versionGetAsString()); /* show version info */
  
  mainWindowXtLoop();     /* give control to the X Toolkit */
  
  /* never here! */
}

/* ---  private functions  --- */

static void setSupportFilePath(char *p) {
  int l;
  
  l=strlen(p);
  if(l>0)
    if(p[l-1]=='/')
      l--;
  l=MIN(l+1,GINGER_PATH_MAX);
  zeroAddStrncpy(support_path, p, l);
}

static void getEnvironmentVariables() {
  char *focus_method;
  char *support_file_path;

  focus_method=getenv("GINGERFOCUS");
  if(focus_method) {
    if( !strcmp(focus_method, "keyboard") )
      setFocusMethod(FOCUS_KEYB);
  }

  support_file_path=getenv("GINGERLIBPATH");
  if(support_file_path) {
    setSupportFilePath(support_file_path);
  }
  else
    setSupportFilePath(".");
}

