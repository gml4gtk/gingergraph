/* $Id: fileSelector.h,v 1.4 1996/01/30 16:12:18 lupus Exp $ */

/***************************************************************
 * This file is part of the Ginger project.                    *
 * Created 6-1-1996                                            *
 * Niels C. Larsen, Aalborg University, Denmark                *
 * (lupus@iesd.auc.dk)                                         *
 ***************************************************************/

#ifndef FILE_SELECTOR_H
#define FILE_SELECTOR_H

#include <X11/Intrinsic.h>
#include <X11/StringDefs.h>
#include <X11/Shell.h>

/* ---  internal limits  --- */

#define FS_MAX_DIR_ENTRIES 1000
#define FS_MAX_DIR_PATH_LENGTH 512

/* ---  public function prototypes  --- */

void FileSelectorOk(Widget w, XtPointer client_data, XtPointer call_data);
void FileSelectorPopup(Widget w, XtPointer client_data, XtPointer call_data);
void FileSelectorPopdown(Widget w, XtPointer client_data, XtPointer call_data);
void FileSelectorUpDirectory(Widget w, XtPointer client_data, XtPointer call_data);
void FileSelectorScanDirectory(Widget w, XtPointer client_data, XtPointer call_data);

void fileSelectorSetDirectory(char *dir_path);
void fileSelectorGetDirectory(char *buf, int bufsiz);
void fileSelectorGetFile(char *buf, int bufsiz);
void fileSelectorSetTitle(char *title);

#endif
