%{
/* $Id: ginger.l,v 1.11 1996/03/11 11:19:37 lupus Exp $ */

/***************************************************************
 * This file is part of the Ginger project.                    *
 * Created 31-12-1995                                          *
 * Niels C. Larsen, Aalborg University, Denmark                *
 * (lupus@iesd.auc.dk)                                         *
 ***************************************************************/

/* ---  Lex-specification for the GGL (Ginger Graph Language)  --- */

#include <stdio.h>
#include <string.h>
#include "str.h"
#include "notify.h"
#include "ginger.tab.h"

int     line = 1;

/* ---  regular definitions  --- */
%}

%option noyywrap

%s COMMENT
%s COMMENT2

delim			[ \t]
newLine         	[\n\r]
blank			[ ]
ws			{delim}+
digit       		[0-9]
letter      		[A-Za-z_\.]
quote			[`']
special         	[\x80-\xff=;:\|\(\)\{\}\+\-\/\*\[\]\<\>\,\?\!\@\#\$\%\^\&\\]
string        		"\""({letter}|{digit}|{blank}|{special}|{quote})*"\""
int      		{digit}+
id			{letter}({digit}|{letter}|{quote})*
begincomment		"/*"
comment			({letter}|{digit}|{delim}|{special}|{quote}|\")*
endcomment		"*/"
begincomment2		"\#"

%%
{begincomment}		{ BEGIN COMMENT;	}
<COMMENT>{comment}{endcomment}	{ BEGIN 0;	}
<COMMENT>{comment}	{ /* no action */;	}
<COMMENT>{newLine}	{ line++;		}
{begincomment2}		{ BEGIN COMMENT2;	}
<COMMENT2>{comment}	{ /* no action */;	}
<COMMENT2>{newLine}	{ BEGIN 0; line++;	}
node			{ return(NODE);		}
edge			{ return(EDGE);		}
template		{ return(TEMPLATE);	}
label			{ return(LABEL);	}
shape			{ return(SHAPE);	}
contents		{ return(CONTENTS);	}
editor			{ return(EDITOR);	}
color			{ return(COLOR);	}
fg_color		{ return(COLOR);	}
foreground		{ return(COLOR);	}
bg_color		{ return(BACKGROUND);	}
background		{ return(BACKGROUND);	}
x			{ return(X);		}
y			{ return(Y);		}
to			{ return(TO);		}
from			{ return(FROM);		}
width			{ return(WIDTH);	}
height			{ return(HEIGHT);	}
direction		{ return(DIRECTION);	}
normal			{ return(NORMAL);	}
inverse			{ return(INVERSE);	}
both			{ return(BOTH);		}
none			{ return(NONE);		}
rectangle		{ return(RECTANGLE);	}
ellipse			{ return(ELLIPSE);	}
rhomb			{ return(RHOMBE);	}
rhombe			{ return(RHOMBE);	}
rhombus			{ return(RHOMBE);	}
bitmap			{ return(BITMAP);	}
pixmap			{ return(PIXMAP);	}
"("			{ return('(');		}
")"			{ return(')');		}
{ws}			{ /* No Action */	}
{newLine}       	{ line++;		}
{id}			{ return(ID);		}
{string}		{ return(STRING);	}
{int}			{ return(INT);		}

%%
/* ---  Supporting C routines  --- */

void yyerror(char* s) {
	char buf[256];
	sprintf(buf, "Error reading input, line %d: %s, <%s>\n",line,s,yytext);
	notifyAndOfferHelp(buf, "GGL grammar"); 
}

void lexerRestart(FILE *f) {
	yyrestart(f);	
}
