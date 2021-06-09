/*
Checksum: 3380374404      (dlex.l.c)
*/

/*
 *    This file is part of:
 *
 *     Ginger -- an INteractive Graph EditoR for the X Window System.
 *                          COPYRIGHT (C) 1996 
 *
 *    Martin K. Molz and Niels C. Larse, Aalborg University, Denmark.
 *
 *    This program is free software; you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation; either version 2 of the License, or
 *    (at your option) any later version.
 *
 *    This program is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with Bison; see the file COPYING.  If not, write to
 *    the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 *    Boston, MA 02110-1301, USA.
 */

%{

/* */
static char *version_dot_l_c ="$Id: dlex.l.c,v 0.000 0000/00/00 00:00:00 top Exp $";

/* */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "gingerprotos.h"

/* */
#if defined YY_FLEX_MINOR_VERSION
	/* flex is used */
#else
	/* flex not used */
#endif

/* flex beta test version */
#ifdef FLEX_BETA
	/* flex-beta test version */
#endif

/* malloc/free/realloc wrap */
#undef malloc
#undef free
#undef realloc

#define malloc dot_lexer_malloc
#define free dot_lexer_free
#define realloc dot_lexer_realloc

/* */
#ifndef YY_USE_PROTOS
#define YY_USE_PROTOS 1
#endif /* YY_USE_PROTOS */

/* */
#ifndef YY_USE_CONST
#define YY_USE_CONST 1
#endif /* YY_USE_CONST */

/* */
#undef YY_NEED_STRLEN

/* */
#define YY_NO_UNPUT 1

/* */
#define YY_NEVER_INTERACTIVE 1
/* #define YY_AlWAYS_INTERACTIVE 1 */

/* increase flex hardcoded preset */
#undef YY_READ_BUF_SIZE
#undef YY_BUF_SIZE
#define YY_READ_BUF_SIZE 1024*64
#define YY_BUF_SIZE 1024*64

/* redefining YY_INPUT to own routine() */
#undef YY_INPUT

/* */
#define YY_INPUT(buf, result, max_size) { \
		result = dot_input ( buf, 1, max_size ); \
	}

/*
 * #define YY_INPUT(buf, result, maxsize) { \
 *     int c=fgetc(yyin);\
 *     if (c==EOF) { result=YY_NULL; } else { buf[0]=c; result=1; } } \
 */

/* renamed macros in the parser */
#define yylval dot_lval

/* token numbers */
#include "dparse.tab.h"

/* token return */
#define DLEX_RET(x) { return (x); }

%}

%s COMMENT
%s COMMENT2

DDELIM			[ \t]
DWHITESPACE		{DDELIM}+
DUTF8LEADIN		\xef\xbb\xbf
DNEWLINEMAC		\r\n
DNEWLINEDOS		\n\r
DNEWLINE         	\n
DBLANK			[ ]
DDIGIT			[0-9]
DLETTER			[A-Za-z_\.]
DLETTER2		[A-Za-z_<>.//]
DQUOTE			[`']
DSPECIAL         	[\x80-\xff=;:\|\(\)\{\}\+\-\/\*\[\]\<\>\,\?\!\@\#\$\%\^\&\\]
DSTRING			"\""({DLETTER2}|{DDIGIT}|{DBLANK}|{DSPECIAL}|{DQUOTE})*"\""
DINT      		{DDIGIT}+
DIDENTIFIER		{DLETTER2}({DDIGIT}|{DLETTER2})*
DBEGINCOMMENT		"/*"
DCOMMENT		({DLETTER}|{DDIGIT}|{DDELIM}|{DSPECIAL}|{DQUOTE}|\")*
DENDCOMMENT		"*/"
DBEGINCOMMENT2		"\#"
DCPLUSPLUS		\/\/.*

/*
 * zero or more of backslash-newline char's
 * with gcc'ism to allow spaces between the
 * backslash and the newline:
 * gccbackslash (\\[ \f\t\v]*\n)*
 */

%%

	/* comment line as c-comment */
{DBEGINCOMMENT}		{ BEGIN COMMENT; 		}
<COMMENT>{DCOMMENT}{DENDCOMMENT} { BEGIN 0; 		}
<COMMENT>{DCOMMENT} 	{ 				}
<COMMENT>{DNEWLINE} 	{ 				}
<COMMENT>{DNEWLINEMAC} 	{ 				}
<COMMENT>{DNEWLINEDOS} 	{ 				}

{DBEGINCOMMENT2}	{ BEGIN COMMENT2; 		}
<COMMENT2>{DCOMMENT}	{ 				}

	/* comment line starting with '#' */
<COMMENT2>{DNEWLINE}	{ BEGIN 0; 			}

	/* c++ comment */
{DCPLUSPLUS}		{				}

"node"			{ DLEX_RET(DOTLEX_NODE);	}
"edge"			{ DLEX_RET(DOTLEX_EDGE);	}
"graph"			{ DLEX_RET(DOTLEX_GRAPH);	}
"digraph"		{ DLEX_RET(DOTLEX_DIGRAPH);	}
"subgraph"		{ DLEX_RET(DOTLEX_SUBGRAPH);	}
"strict"		{ DLEX_RET(DOTLEX_STRICT);	}
"->"			{ DLEX_RET(DOTLEX_DEDGE);	}
"--"			{ DLEX_RET(DOTLEX_UEDGE);	}
"@"			{ DLEX_RET('@');		}
"="			{ DLEX_RET('=');		}
"+"			{ DLEX_RET('+');		}
","			{ DLEX_RET(',');		}
"["			{ DLEX_RET('[');		}
"]"			{ DLEX_RET(']');		}
"("			{ DLEX_RET('(');		}
")"			{ DLEX_RET(')');		}
"{"			{ DLEX_RET('{');		}
"}"			{ DLEX_RET('}');		}
":"			{ DLEX_RET(':');		}
";"			{ DLEX_RET(';');		}
{DUTF8LEADIN}		{ DLEX_RET(DOTLEX_UTF8LEADIN);	}
{DWHITESPACE}		{				}
{DNEWLINEMAC}		{				}
{DNEWLINEDOS}		{				}
{DNEWLINE}		{				}
{DIDENTIFIER}		{ dot_lval.dot_text=strdup(yytext); DLEX_RET(DOTLEX_IDENT); }
{DSTRING}		{ dot_lval.dot_text=strdup(yytext); DLEX_RET(DOTLEX_STRING); }
{DINT}			{ dot_lval.dot_text=strdup(yytext); DLEX_RET(DOTLEX_NUMBER); }
.			{ DLEX_RET(' '); /* other */	}

%%

/* */
#undef malloc
#undef free
#undef realloc

/* */
int dot_lexer_init (void)
{
	yy_init = 1;
	return (0);
}

/* */
int dot_lexer_deinit (void)
{
	yy_init = 0;
	return (0);
}

/* */
void *dot_lexer_malloc (size_t n) 
{ 
	return (malloc(n)); 
}

/* */
void dot_lexer_free (void *p) 
{ 
	if (p) {
		free (p);
	}
	return;
}

/* */
void *dot_lexer_realloc (void *p, size_t n) 
{ 
	return ((void *) realloc (p, n)); 
}

/* */
char *dot_lexer_version (void) 
{ 
	return ((char *) version_dot_l_c); 
}

/* */
int dot_input (void *ptr, size_t size, size_t nmemb)
{
	size_t result = 0;
	result = fread (ptr, size, nmemb, yyin);
	return ((int) result);
}

/* */
void dot_set_stream (FILE *stream, int debugflag)
{
	yyin = stream;
	yy_flex_debug = debugflag;
	return;
}

/* yy_delete_buffer() because of memory leak in some flex versions */
int dot_wrap (void) 
{ 
	yy_flush_buffer(YY_CURRENT_BUFFER);
	yy_delete_buffer(YY_CURRENT_BUFFER);
	yy_init = 1;
	return (1);
}

/* End. */
