/* $Id: ginger.y,v 1.11 1996/03/04 08:50:05 lupus Exp $ */

/***************************************************************
 * This file is part of the Ginger project.                    *
 * Created 31-12-1995                                          *
 * Niels C. Larsen, Aalborg University, Denmark                *
 * (lupus@iesd.auc.dk)                                         *
 ***************************************************************/

/* ---  Grammar-specification for the GGL 
	(Ginger Graph Language)  --- */

/* ---  C-declarations  --- */
%{
#include <stdlib.h>
#include <string.h>
#include "options.h"
#include "gingerNew.h"
#include "symtable.h"
#include "str.h"
#include "graph.h"
#include "graphIO.h"
#include "parser.h"

extern char *yytext;      /* from lex.yy.c */
extern int  line;         /* from lex.yy.c */

static int mode=PARSER_MODE_NEW;  /* PARSER_MODE_NEW or PARSER_MODE_MERGE */
static graph *current_graph=0;
static attribute_obj the_obj;

FILE *error_output;

static void newObj();

%}

/* ---  Types  --- */

%union
{
  char	*a_string;
  int   an_int;
}

/* ---  Token declarations  --- */

%token NODE EDGE TEMPLATE
%token LABEL SHAPE CONTENTS EDITOR COLOR TO FROM ID STRING
%token X Y WIDTH HEIGHT INT DIRECTION
%token BACKGROUND
%token NORMAL INVERSE BOTH NONE
%token RECTANGLE ELLIPSE RHOMBE BITMAP PIXMAP

/* ---  type information on non-terminals  --- */

%type <a_string> name string
%type <an_int> number direction shape

%%

/* ---  Grammar rules  --- */

graph	: { /* initial actions */
	    initTable();            /* initialize symbol table */
	    resetObj(&the_obj);     /* clear the attr. object  */
            line=1;                 /* reset line number count */
	    error_output = stdout;  /* send err.messg. to stdout */
	  } 
	objs 
	  { /* actions upon recognizing a complete graph production */
	    switch(mode) {
 	    case PARSER_MODE_NEW:
              current_graph=buildGraphFromSymtable();
              break;
            case PARSER_MODE_MERGE:
	      if(current_graph) {
                if( getCurrentRenameAtInsertMode() )
		  mergeGraphFromSymtableWithGraph(current_graph, 
				     		CLASH_MODE_RENAME,
						CLASH_MODE_RENAME);
                else
		  mergeGraphFromSymtableWithGraph(current_graph, 
				     		CLASH_MODE_MERGE,
						CLASH_MODE_MERGE);
              }
	      break;
	    }
	    cleanUpTable(); 
            graphIOReadResult(current_graph); /* give result to graphIO */
	    current_graph=0;
	    mode = PARSER_MODE_NEW; /* just in case someone forget to
	                               reset the mode with parserSetMode() */
	  }
	;

objs 	: objs obj
	|	
	;

obj	: node
	| edge
	| template
	;

node	: '(' NODE name attributes ')'	{ the_obj.name=$3;
					  the_obj.type=T_NODE;
					  newObj();
					}
	;	

edge	: '(' EDGE name attributes ')'	{ the_obj.name=$3;
					  the_obj.type=T_EDGE;
					  newObj();
					}	
	;	

template: '(' TEMPLATE name attributes ')' { the_obj.name=$3;
					     the_obj.type=T_TEMPLATE;
					     newObj();
					   }
	;	

attributes : attributes attr
	|
	;

attr	: LABEL string  	{ the_obj.label		= $2; 
				  the_obj.vlabel	= 1; }
	| SHAPE shape  		{ the_obj.shape		= $2; 
				  the_obj.vshape	= 1; }
	| CONTENTS string	{ the_obj.contents	= $2; 
				  the_obj.vcontents	= 1; }
	| EDITOR string		{ the_obj.editor	= $2; 
				  the_obj.veditor	= 1; }
	| COLOR string		{ the_obj.color		= $2; 
				  the_obj.vcolor	= 1; }
	| BACKGROUND string	{ the_obj.bg_color	= $2; 
				  the_obj.vbg_color	= 1; }
	| X number		{ the_obj.x		= $2; 
				  the_obj.vx		= 1; }
	| Y number		{ the_obj.y		= $2; 
				  the_obj.vy		= 1; }
	| WIDTH number		{ the_obj.width		= $2; 
				  the_obj.vwidth	= 1; }
	| HEIGHT number		{ the_obj.height	= $2; 
				  the_obj.vheight	= 1; }
	| BITMAP string		{ the_obj.bitmap	= $2;
				  the_obj.vbitmap	= 1; }
	| PIXMAP string		{ the_obj.pixmap	= $2;
				  the_obj.vpixmap	= 1; }
	| TEMPLATE name		{ the_obj.template	= $2; 
				  the_obj.vtemplate	= 1; }
	| TO name		{ the_obj.to		= $2; 
				  the_obj.vto		= 1; }
	| FROM name		{ the_obj.from		= $2; 
				  the_obj.vfrom		= 1; }
	| DIRECTION direction	{ the_obj.direction	= $2; 
				  the_obj.vdirection	= 1; }
	;

name	: ID	{ $$ = GingerNewString(yytext); }
	;

string	: STRING { $$ = makeCleanString(yytext); }
	;

number 	: INT	{ $$ = atoi(yytext); }
	;

shape	: RECTANGLE	{ $$ = SHAPE_RECTANGLE; }
	| RHOMBE        { $$ = SHAPE_RHOMBE;    }
	| ELLIPSE	{ $$ = SHAPE_ELLIPSE;   }
	| NONE		{ $$ = SHAPE_NONE;	}
	;

direction : NORMAL	{ $$ = DIR_NORMAL;        }
	| INVERSE	{ $$ = DIR_INVERSE;       }
	| BOTH		{ $$ = DIR_BIDIRECTIONAL; }
	| NONE		{ $$ = DIR_NONE;          }
	;

%%

/* ---  Supporting C-functions  --- */

/* add the current object (in the_obj) to the symbol table */
static void newObj() {
  char *obj;

  switch(the_obj.type) {
  case T_NODE:
    obj="Node";
    break;
  case T_EDGE:
    obj="Edge";
    break;
  case T_TEMPLATE:
    obj="Template";
    break;
  }

  /* check whether an object of same type with same name exists */
  if( findObjNamed(the_obj.name, the_obj.type) ) {
    fprintf(error_output, "%s %s multiply defined\n", 
			  obj, the_obj.name);
  } else { /* otherwise, add the object to the symbol table */
    addObj(&the_obj);
  }

  /* clear contents of the_obj to prepare for the next object */
  resetObj(&the_obj);
}

void parserSetMode(int the_mode, graph *g) {
  mode = the_mode;
  current_graph = g;
}
