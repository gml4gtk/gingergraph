/* $Id: options.c,v 1.6 1996/03/22 10:05:53 lupus Exp $ */

/***************************************************************
 * This file is part of the Ginger project.                    *
 * Created 27-1-1996                                           *
 * Niels C. Larsen, Aalborg University, Denmark                *
 * (lupus@iesd.auc.dk)                                         *
 ***************************************************************/

/* Representation of the program options that may be selected
   on the "options" menu */

#include <stdlib.h>
#include "menuOpcodes.h"
#include "config.h"
#include "options.h"
#include "str.h"
#include "main.h"

/* ---  private declarations  --- */

/* option variables */
static int rename_at_insert;

/* options menu */

static menu_item options_menu_entries[] = {
  {"Rename objects at insert",   1, OPTIONS_RENAME_AT_INSERT, 0},
  {"line",                -1, -1,  0},
  {"Save options",         1, OPTIONS_SAVE, 0},
  {0,0,0,0} 
};

/* private function prototypes */
static void readIntOption(FILE *fp, int *option);
static void defaultOptions();
static void loadOptions();

/* ---  public functions  --- */

void initializeOptionsMenu(Widget activatingButton) {
  createMenu("optionsMenu", activatingButton, options_menu_entries);
  loadOptions();
}

void saveOptions() {
  FILE *fp;
  char buf[256];
  
  if( (fp=fopen(expandPathOfSupportFile(OPTIONS_FILE, buf, 256), "w"))==NULL ) {
    return;
  }
  
  fprintf(fp, "rename at insert\n%d\n", rename_at_insert);
  
  fclose(fp);
}

int toggleRenameAtInsert() {
  rename_at_insert=!rename_at_insert;
  menuSetMark(options_menu_entries, OPTIONS_RENAME_AT_INSERT, rename_at_insert);
  return rename_at_insert;
}

int getCurrentRenameAtInsertMode() {
  return rename_at_insert;
}

/* ---  private functions  --- */

static void readIntOption(FILE *fp, int *option) {
  char buf[80];
  
  /* read and discard describing line */
  if(!feof(fp))
    fgets(buf,80,fp); 
  /* read line containing value */
  if(!feof(fp)) {
    fgets(buf,80,fp);
    *option=atoi(buf);
  }
}

static void defaultOptions() {
  rename_at_insert=1;
}

static void loadOptions() {
  FILE *fp;
  char buf[256];
  
  /* first set options to default values, in case no options file
     exists or it doesn't specify all options (i.e. file is older 
     version) */
 defaultOptions();
  
  if( (fp=fopen(expandPathOfSupportFile(OPTIONS_FILE, buf, 256), "r"))!=NULL ) {
    
    readIntOption(fp, &rename_at_insert);
    
    fclose(fp);
  } 
  
  menuSetMark(options_menu_entries, OPTIONS_RENAME_AT_INSERT, rename_at_insert);
}


