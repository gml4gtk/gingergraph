/* $Id: elkInterface.c,v 1.11 1996/04/01 07:46:17 lupus Exp $ */
  
/***************************************************************
 * This file is part of the Ginger project.                    *
 * Created 01-03-1996                                          *
 * Niels C. Larsen, Aalborg University, Denmark                *
 * (lupus@iesd.auc.dk)                                         *
 * Martin K. Molz, Aalborg University, Denmark                 *
 * (molz@iesd.auc.dk)                                          *
 ***************************************************************/

#include "config.h"

#if USE_SCHEME  /* if not, skip the rest of this file */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "scheme.h"
#include "elkGraph.h"
#include "str.h"
#include "main.h"

#define DEFAULT_DIR "."

extern char *optarg;
extern int optind, opterr;

/* The function
 * 
 *     char *Elk_Eval(char *expr);
 * 
 * is similar to Tcl_Eval() in Tcl.  It is called with a Scheme expression
 * encoded as a C string and returns the result of evaluating the expression
 * (as another C string), or a null pointer if an error has occured
 * during evaluation.
 * 
 * Elk_Eval() stores its result in a static buffer of fixed size; this
 * can be improved easily by passing a buffer and a length as additional
 * arguments.
 */

static Object in, out;

static char *String_Eval(expr) char *expr; {
    Object str, res;
    char *p;
    GC_Node;
    static char buf[1024];

    str = Make_String(expr, strlen(expr));
    PORT(in)->name = str;
    PORT(in)->ptr = 0;
    res = General_Read(in, 0);
    GC_Link(res);
    res = Eval(res);
    (void)General_Print_Object(res, out, 1);
    str = P_Get_Output_String(out);
    p = Get_String(str);
    if (strlen(p) > sizeof buf - 1)
	p = "[result too long for display]";
    strcpy(buf, p);
    GC_Unlink;
    return buf;
}

static char *Elk_Eval(expr) char *expr; {
    char *s;

    s = String_Eval("\
	(call-with-current-continuation (lambda (c)\
	  (set! error-handler (lambda a (gingerError a) (c #f))) #t))\
    ");
    if (strcmp(s, "#f") == 0)
	return 0;
    return String_Eval(expr);
}

static void elk_init_eval() {
    in = P_Open_Input_String(Make_String("", 0));
    Global_GC_Link(in);
    out = P_Open_Output_String();
    Global_GC_Link(out);
}


void elkInterfaceInitialize(int ac, char **av) {
  char **eav;
  int eac = 1, c;
  
  char *directory;
  char buf[4096];

  Set_App_Name(av[0]);
  eav = (char**)Safe_Malloc((ac+2+1+2) * sizeof(char *));    /* ac + -p xxx + -h + heapsize + 0 */
  eav[0] = av[0];
  eav[eac++] = "-p";

  directory = supportFilePath();

  sprintf(buf, "%s/Scheme/Ginger:%s/Scheme/ElkRuntime:%s/Scheme/Meroon",
	  directory,
	  directory,
	  directory);
  
  eav[eac++] = strdup(buf);

#ifdef GINGER_HEAP_K_BYTES
  sprintf(buf, "%d", GINGER_HEAP_K_BYTES);
  eav[eac++] = "-h";          /* heap size */
  eav[eac++] = strdup(buf);;
#endif

  eav[eac] = 0;
  
  Elk_Init(eac, eav, 0, 0);
  elk_init_eval();
  
  installGraphPrimitives();
}

char *elkInterfaceEvalString(char* expr) {
  return Elk_Eval(expr);
}

#endif /* USE_SCHEME */
