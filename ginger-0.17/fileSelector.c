/* $Id: fileSelector.c,v 1.6 1996/02/20 10:38:30 lupus Exp $ */

/***************************************************************
 * This file is part of the Ginger project.                    *
 * Created 6-1-1996                                            *
 * Niels C. Larsen, Aalborg University, Denmark                *
 * (lupus@iesd.auc.dk)                                         *
 ***************************************************************/

/* ---  description  --- */

/***************************************************************
 * The file selector consists of a pop-up window containing    *
 * input fields for (A) directory path and (B) file name. The  *
 * list of files in the directory (A) is shown in a scrollable *
 * list, where files can be selected with the mouse.	       *
 * 							       *
 * Public functions exist to pop up/down the window, set/get   *
 * the directory, etc.					       *
 * 							       *
 * Edit the FileSelectorOk() function to specify what action   *
 * to perform when a file is selected and confirmed 	       *
 * by the user.						       *
 ***************************************************************/

/* ---  include files  --- */

#include "fileSelector.h"

#include <X11/Xaw/Command.h>
#include <X11/Xaw/Paned.h>
#include <X11/Xaw/Text.h>
#include <X11/Xaw/Box.h>
#include <X11/Xaw/AsciiText.h>
#include <X11/Xaw/List.h>
#include <X11/Xaw/Viewport.h>

#include <string.h>
#include <unistd.h>

#include "str.h"
#include "focus.h"
#include "Directory.h"

#include "graphBuffer.h"

/* ---  window and button dimensions  --- */

#define FS_HEIGHT 200
#define FS_WIDTH  335
#define FS_BUTTON_WIDTH 60

/* ---  private function prototypes  --- */

static void fileSelectorInit(void);
static void createFileSelectorWidgets(void);
static int updateDirectoryList(char *dir_path);
static int indxOfCurrentSelection(void);
static void directoryListSelectionCallBack(Widget w,
              XtPointer client_data, XtPointer call_data);

/* ---  private variable declarations  --- */

static Widget fileSelectorShell, fileSelectorDirectoryList,
       fileSelectorDirectoryPathInput,
       fileSelectorFileInput;

/* array of strings (file/dir-names) shown in the directory list */
static char *directoryStr[FS_MAX_DIR_ENTRIES]; 

/* is the window popped up */
static int popup=0;

extern Widget topLevel;  /* in mainWindow.c */

/* ---  public functions  --- */

/* update directory path and filename input fields, and 
   the directory list */
void fileSelectorSetDirectory(char *dir_path) {
  createFileSelectorWidgets(); /* make sure widgets are initialized */
  XtVaSetValues(fileSelectorDirectoryPathInput,
                XtNstring, dir_path,
                NULL);
  XtVaSetValues(fileSelectorFileInput,
		XtNstring, "",
		NULL);

  updateDirectoryList(dir_path);
}

/* read the directory path from the input field to buf */
void fileSelectorGetDirectory(char *buf, int bufsiz) {
  String p;
  
  createFileSelectorWidgets(); /* make sure widgets are initialized */
  XtVaGetValues(fileSelectorDirectoryPathInput,
                XtNstring, &p,
                NULL);

  zeroAddStrncpy(buf, p, bufsiz);
}

/* read the file name from the input field to buf */
void fileSelectorGetFile(char *buf, int bufsiz) {
  String p;
  
  createFileSelectorWidgets(); 
  XtVaGetValues(fileSelectorFileInput,
                XtNstring, &p,
                NULL);
  
  zeroAddStrncpy(buf, p, bufsiz);
}

/* set the window title */
void fileSelectorSetTitle(char *title) {
  createFileSelectorWidgets(); /* make sure widgets are initialized */
  XtVaSetValues(fileSelectorShell,
		XtNtitle, title,
		NULL);
}

void FileSelectorOk(Widget w, XtPointer client_data, XtPointer call_data)
{
  char fname[FS_MAX_DIR_PATH_LENGTH];
  char fname2[FS_MAX_DIR_PATH_LENGTH];
  int l;
  
  fileSelectorGetDirectory(fname, FS_MAX_DIR_PATH_LENGTH);
  if((l=strlen(fname))==0)  /* if no directory is selected, ignore action */
    return;

  if( fname[l-1] != '/' )
    strcat(fname, "/");

  fileSelectorGetFile(fname2, FS_MAX_DIR_PATH_LENGTH);
  if((l=strlen(fname2))==0)  /* if no file is selected, ignore action */
    return;
  
  if( !strcmp(fname2, "..") ) {
    FileSelectorUpDirectory(0,0,0);
    return;
  }

  strcat(fname, fname2);
  
  FileSelectorPopdown(w, client_data, call_data);
  
  /* do something with fname */
  graphBufferProcessFileSelection(fname);

}

void FileSelectorPopup(Widget w, XtPointer client_data, XtPointer call_data)
{
  Position x,y;
  char buf[FS_MAX_DIR_PATH_LENGTH];
  
  if( !popup ) {
    createFileSelectorWidgets();  /* make sure widgets are initialized */
    
    /* position the popup window relative to the main window */
    XtTranslateCoords(topLevel,(Position)30, (Position)40,
		      &x, &y);
    
    XtVaSetValues(fileSelectorShell, 
		  XtNx, x,
		  XtNy, y,
		  NULL );

    /* rescan the directory */
    fileSelectorGetDirectory(buf, FS_MAX_DIR_PATH_LENGTH);
    updateDirectoryList(buf);

    /* pop up the window */
    popup=1;
    XtPopup(fileSelectorShell, XtGrabNone);
  }
}

void FileSelectorPopdown(Widget w, XtPointer client_data, XtPointer call_data)
{
  if( popup ) {
    XtPopdown(fileSelectorShell);
    popup=0;
  }
}

/* go "up" one level in the directory hierarchy. */
void FileSelectorUpDirectory(Widget w, XtPointer client_data, XtPointer call_data) {
  char buf[FS_MAX_DIR_PATH_LENGTH];
  int l,i;

  fileSelectorGetDirectory(buf, FS_MAX_DIR_PATH_LENGTH);

  l=strlen(buf);
  if(l--==0) /* if directory path is empty, then do nothing */
    return;

  if( *(buf+l)=='/' )  /* if there's a trailing '/', skip it */
    l--;

  /* "remove" the last path component in the directory path */
  while( l>=0 ) {       /* scan buf backwards until a '/' is found */
    if( *(buf+l)=='/' ) {
      *(buf+l+1)='\0';  /* terminate string here */
      break;
    }
    l--;
  }

  /* set directory */
  fileSelectorSetDirectory(buf);
}

void FileSelectorScanDirectory(Widget w, XtPointer client_data, XtPointer call_data)
{
  char buf[FS_MAX_DIR_PATH_LENGTH];

  fileSelectorGetDirectory(buf, FS_MAX_DIR_PATH_LENGTH);
  if( !updateDirectoryList(buf) ) {
    if(directoryStr[0]!=0) {
      XtFree(directoryStr[0]);
      directoryStr[0]=XtNewString(" ");
    }
    XawListChange(fileSelectorDirectoryList, directoryStr, 1, 0, TRUE);
  }
}

/* ---  private functions  --- */

static void fileSelectorInit(void) {
  int i;
  for(i=0; i<FS_MAX_DIR_ENTRIES; i++)
    directoryStr[i]=0;
}

static int updateDirectoryList(char *dir_path) {
  DirEntry entry;
  Directory directory;
  int i;
  char buf[FS_MAX_DIR_PATH_LENGTH];

  if (DirectoryOpen(dir_path,&directory) == FALSE)
    {
      /* fprintf(stderr,"Can't open directory '%s'\n",dir_path); */
      return 0;
    }

  i=0;
  while( DirectoryReadNextEntry(&directory,&entry)
         && i<FS_MAX_DIR_ENTRIES-1 )
    {
      if(directoryStr[i]!=0)
	XtFree(directoryStr[i]);
      if( FTypeIsDir(DirEntryType(&entry)) ) {
	sprintf(buf, "[%s]", DirEntryFileName(&entry) );
	directoryStr[i]=XtNewString( buf );
      }
      else
	directoryStr[i]=XtNewString( DirEntryFileName(&entry) );
      i++;
    }
  if(directoryStr[i]!=0) {
    XtFree(directoryStr[i]);
    directoryStr[i]=0;
  }
  
  sortStrings(directoryStr, i);
  XawListChange(fileSelectorDirectoryList, directoryStr, i, 0, TRUE);
  return 1;
}

static int indxOfCurrentSelection(void) {   /* returns -1, if none is selected */
  XawListReturnStruct *res;
  res=XawListShowCurrent(fileSelectorDirectoryList);
  if( res->list_index==XAW_LIST_NONE )
    return -1;
  else
    return res->list_index;
}

static void directoryListSelectionCallBack(Widget w, XtPointer client_data, XtPointer call_data)
{
  int i,l;
  char buf[FS_MAX_DIR_PATH_LENGTH], buf2[FS_MAX_DIR_PATH_LENGTH];
  char *p;

  i=indxOfCurrentSelection();
  if(i<0)
    return;

  p=directoryStr[i];

  if( *p=='[' ) {  /* remove the '[' ']' chars from directory names */
    strcpy(buf2, (p+1));
    chop(buf2);
  }
  else
    strcpy(buf2, directoryStr[i]);

  if( !strcmp(buf2, ".") )  /* if the current directory "." is selected,
			       nothing needs to be changed */
    return;

  if( !strcmp(buf2, "..") ) {  /* select ".." => go "up" one level */
    FileSelectorUpDirectory(0,0,0);
    return;
  }

  /* construct full path by concatenating the dir path and 
     file/dir name */
  fileSelectorGetDirectory(buf, FS_MAX_DIR_PATH_LENGTH);
  if((l=strlen(buf))==0)  /* if no file is selected, ignore action */
    return;

  if( buf[l-1] != '/' )
    strcat(buf, "/");
  strcat(buf, buf2);

  /* if the full path can be opened as a directory, do that */
  if( updateDirectoryList(buf) ) {
    XtVaSetValues(fileSelectorDirectoryPathInput,
		  XtNstring, buf,
		  NULL);
    XtVaSetValues(fileSelectorFileInput,
		  XtNstring, "",
		  NULL);
  }
  /* otherwise it may be a file, so put the filename into the 
     filename input field */
  else {
    XtVaSetValues(fileSelectorFileInput,
		  XtNstring, buf2,
		  NULL);
  }
}

/* create and initialize widgets etc. This function may be
   called multiple times, but only performs "real" action the
   first time it is called. */
static void createFileSelectorWidgets(void) {
  static int done=0;
  char buf[FS_MAX_DIR_PATH_LENGTH];
  char *p;

  Widget fileSelectorPane,
    fileSelectorForm,
    fileSelectorScanButton, fileSelectorUpButton,
    fileSelectorClearFileButton,
    fileSelectorViewp,
    fileSelectorOk,
    fileSelectorCancel;

  if(done)
    return;
  done=1;

  fileSelectorInit();

  fileSelectorShell = XtVaCreatePopupShell (
				      "fileSelectorShell",
				      transientShellWidgetClass,
				      topLevel,
				      XtNtitle, "File selector",
				      NULL );

  fileSelectorPane = XtVaCreateManagedWidget(
				       "fileSelectorPane",
				       panedWidgetClass,
				       fileSelectorShell,
				       XtNheight, FS_HEIGHT,
				       XtNwidth, FS_WIDTH,
				       NULL);

  fileSelectorForm = XtVaCreateManagedWidget(
				   "fileSelectorForm",
				   formWidgetClass,
				   fileSelectorPane,
				   NULL);

  fileSelectorDirectoryPathInput = XtVaCreateManagedWidget(
			         "fileSelectorDirectoryPathInput",
				 asciiTextWidgetClass,
				 fileSelectorForm,
				 XtNstring, "",
				 XtNeditType, XawtextEdit,
				 XtNwidth, FS_WIDTH-2*FS_BUTTON_WIDTH-20,
				 NULL);

  fileSelectorScanButton = XtVaCreateManagedWidget(
				     "fileSelectorScanButton",
				     commandWidgetClass,
				     fileSelectorForm,
				     XtNlabel, "Scan",
				     XtNwidth, FS_BUTTON_WIDTH,
				     XtNfromHoriz, fileSelectorDirectoryPathInput,
				     NULL);

  fileSelectorUpButton = XtVaCreateManagedWidget(
				     "fileSelectorUpButton",
				     commandWidgetClass,
				     fileSelectorForm,
				     XtNlabel, "Up",
				     XtNwidth, FS_BUTTON_WIDTH,
				     XtNfromHoriz, fileSelectorScanButton,
				     NULL);

  fileSelectorFileInput = XtVaCreateManagedWidget(
				     "fileSelectorFileInput",
				     asciiTextWidgetClass,
				     fileSelectorForm,
				     XtNeditType, XawtextEdit,
				     XtNwidth, FS_WIDTH-2*FS_BUTTON_WIDTH-20,
				     XtNfromVert, fileSelectorUpButton,

				     NULL);

  fileSelectorOk = XtVaCreateManagedWidget(
				     "fileSelectorOk",
				     commandWidgetClass,
				     fileSelectorForm,
				     XtNlabel, "Ok",
				     XtNwidth, FS_BUTTON_WIDTH,
				     XtNfromVert, fileSelectorUpButton,
				     XtNfromHoriz, fileSelectorFileInput,
				     NULL);

  /* set up focus control between dir-input and file-input fields */
  saveFocusInfo(fileSelectorDirectoryPathInput, 
		fileSelectorFileInput,
		fileSelectorPane,
		fileSelectorScanButton, 1);
  saveFocusInfo(fileSelectorFileInput,
		fileSelectorDirectoryPathInput, 
		fileSelectorPane,
		fileSelectorOk, 0);

  fileSelectorCancel = XtVaCreateManagedWidget(
				     "fileSelectorCancel",
				     commandWidgetClass,
				     fileSelectorForm,
				     XtNlabel, "Cancel",
				     XtNwidth, FS_BUTTON_WIDTH,
				     XtNfromVert, fileSelectorUpButton,
				     XtNfromHoriz, fileSelectorOk,
				     NULL);

  fileSelectorViewp = XtVaCreateManagedWidget(
                                  "fileSelectorViewp",
                                  viewportWidgetClass,
                                  fileSelectorPane,
                                  XtNheight, 65,
                                  /* XtNwidth, 220, */
                                  XtNallowVert, TRUE,
                                  NULL);

  fileSelectorDirectoryList = XtVaCreateManagedWidget(
				       "fileSelectorDirectoryList",
				       listWidgetClass,
				       fileSelectorViewp,
				       /* XtNlist, directoryStr, */
				       /* XtNpasteBuffer, TRUE, */
				       /* XtNforceColumns, TRUE, */
				       XtNverticalList, True,
  				       NULL);

  XtAddCallback(fileSelectorScanButton, XtNcallback,
		FileSelectorScanDirectory, topLevel);
  XtAddCallback(fileSelectorUpButton, XtNcallback,
		FileSelectorUpDirectory, topLevel);
  XtAddCallback(fileSelectorOk, XtNcallback,
		FileSelectorOk, topLevel);
  XtAddCallback(fileSelectorCancel, XtNcallback,
		FileSelectorPopdown, topLevel);
  XtAddCallback(fileSelectorDirectoryList, XtNcallback,
		directoryListSelectionCallBack, topLevel);

  /* use the current working directory as initial value */
  p=getcwd(buf, FS_MAX_DIR_PATH_LENGTH);
  if(p)
    fileSelectorSetDirectory(p);
  else
    fileSelectorSetDirectory(".");

  /* set input focus to the directory field */
  jumpTo(fileSelectorDirectoryPathInput);
}
