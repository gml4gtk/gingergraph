/* $Id: gingerNew.h,v 1.1 1996/03/04 08:50:05 lupus Exp $ */
  
/***************************************************************
 * This file is part of the Ginger project.                    *
 * Created 02-03-1996                                          *
 * Niels C. Larsen, Aalborg University, Denmark                *
 * (lupus@iesd.auc.dk)                                         *
 * Martin K. Molz, Aalborg University, Denmark                 *
 * (molz@iesd.auc.dk)                                          *
 ***************************************************************/

/* This file is a hack to "fix" the problem that both the Elk library 
   and the X Toolkit Intrinsics define a type called "Object".

   What we do is include this file instead of <X11/Intrinsic.h> in
   our "top-level" include files "node.h" and "edge.h". This file then 
   includes <X11/Xlib.h> instead, and explicitly defines the intrinsic
   macros "XtNew" and "XtNewString" used extensively in the code.
*/

/* This file contains definitions borrowed from the <X11/Intrinsic.h> 
   file. The following copyright notice states the original copyright and
   copying policy from <X11/Intrinsic.h>.
*/

/*
Copyright (c) 1987, 1988, 1994  X Consortium

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
X CONSORTIUM BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN
AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

Except as contained in this notice, the name of the X Consortium shall not be
used in advertising or otherwise to promote the sale, use or other dealings
in this Software without prior written authorization from the X Consortium.

*/

#ifndef GINGER_NEW_H
#define GINGER_NEW_H

#include "config.h"

#if USE_SCHEME

#include <X11/Xlib.h>
#include <string.h>
#include "allocmem.h"
#define GingerNew(type) ((type *) AllocateMemory((unsigned) sizeof(type)))
#define GingerMalloc(x) AllocateMemory(x)
#define GingerNewString(str) ((str) != NULL ? (strcpy(GingerMalloc((unsigned)strlen(str) + 1), str)) : NULL)
extern void XtFree(
#if NeedFunctionPrototypes
    char*		/* ptr */
#endif
);
#define GingerFree(x)   XtFree(x)

#else

#include <X11/Intrinsic.h>
#define GingerNew(type) XtNew(type)
#define GingerMalloc(x) XtMalloc(x)
#define GingerNewString(str) XtNewString(str)
#define GingerFree(x)   XtFree(x)

#endif

#endif
