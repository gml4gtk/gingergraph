/*
Checksum: 1623000553      (dparse.y.c)
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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#include "gingerprotos.h"

/* debug */
#define YYDEBUG 1 
/* #undef YYDEBUG */

/* debug */
#define YYERROR_VERBOSE 1

/* */
#define YYVALGLOBAL 1

/* set larger */
#undef YYINITDEPTH
#define YYINITDEPTH 50000

/*
 * routing malloc()/free()
 *
 * # if YYSTACK_USE_ALLOCA
 * #  define YYSTACK_ALLOC alloca
 * # else
 * #  ifndef YYSTACK_USE_ALLOCA
 * #   if defined (alloca) || defined (_ALLOCA_H)
 * #    define YYSTACK_ALLOC alloca
 * #   else
 * #    ifdef __GNUC__
 * #     define YYSTACK_ALLOC __builtin_alloca
 * #    endif
 * #   endif
 * #  endif
 * # endif
 * 
 * # ifdef YYSTACK_ALLOC
 * #  define YYSTACK_FREE(Ptr) do { ; } while (0)
 * # else
 * #  if defined (__STDC__) || defined (__cplusplus)
 * #   include <stdlib.h> 
 * #   define YYSIZE_T size_t
 * #  endif
 * #  define YYSTACK_ALLOC malloc
 * #  define YYSTACK_FREE free
 * # endif
 */

/*
 * Remap normal yacc parser interface names (yyparse, yylex, yyerror, etc),
 * as well as gratuitiously global symbol names, so we can have multiple
 * yacc generated parsers in the same program.  Note that these are only
 * the variables produced by yacc.  If other parser generators (bison,
 * byacc, etc) produce additional global names that conflict at link time,
 * then those parser generators need to be fixed instead of adding those
 * names to this list. 
 */

#define yymaxdepth	dot_maxdepth
#define yyparse		dot_parse
#define yylex		dot_lex
#define yyerror		dot_error
#define yylval		dot_lval
#define yychar		dot_char
#define yydebug		dot_debug
#define yypact		dot_pact
#define yyr1		dot_r1
#define yyr2		dot_r2
#define yydef		dot_def
#define yychk		dot_chk
#define yypgo		dot_pgo
#define yyact		dot_act
#define yyexca		dot_exca
#define yyerrflag	dot_errflag
#define yynerrs		dot_nerrs
#define yyps		dot_ps
#define yypv		dot_pv
#define yys		dot_s
#define yy_yys		dot_yys
#define yystate		dot_state
#define yytmp		dot_tmp
#define yyv		dot_v
#define yy_yyv		dot_yyv
#define yyval		dot_val
#define yylloc		dot_lloc
#define yyreds		dot_reds          /* With YYDEBUG defined */
#define yytoks		dot_toks          /* With YYDEBUG defined */
#define yylhs		dot_yylhs
#define yylen		dot_yylen
#define yydefred	dot_yydefred
#define yydgoto		dot_yydgoto
#define yysindex	dot_yysindex
#define yyrindex	dot_yyrindex
#define yygindex	dot_yygindex
#define yytable		dot_yytable
#define yycheck		dot_yycheck

/* */
#ifdef YYDEBUG
/* empty */
#else
int yydebug = 0;
#endif

/* */
#ifdef YYDEBUG

/* Bison debug output to stdout redefining YYFPRINTF */
#define YYFPRINTF dot_parser_fprintf
void dot_parser_fprintf (FILE *f, const char *fmt, ...)
{
    char buffer[1024];
    int n = 0;
    va_list args;
    (void) memset (buffer, 0, 1024);
    va_start (args, fmt);
    n = vsnprintf (buffer, 1024, fmt, args);
    va_end(args);
    if (n > 0) {
	(void) fprintf (f,"%s",buffer);
	(void) fflush (f);
    }
    return;
}

#endif

%}

/* %glr-parser */

%token DOTLEX_NODE /* `node' */
%token DOTLEX_EDGE /* `edge'*/
%token DOTLEX_GRAPH /* `graph' */
%token DOTLEX_DIGRAPH /* `digraph' */
%token DOTLEX_SUBGRAPH /* `subgraph' */
%token DOTLEX_STRICT /* `strict' */
%token DOTLEX_DEDGE /* `->' */
%token DOTLEX_UEDGE /* `--' */
%token DOTLEX_UTF8LEADIN /* `utf-8 codes' */
%token <dot_text> DOTLEX_IDENT /* `identifier' */
%token <dot_text> DOTLEX_STRING /* `string' */
%token <dot_text> DOTLEX_NUMBER /* `number' */

/* */
%nonassoc DOTLEX_LLBRACE

%type <dot_ptr> dot_graphstart
%type <dot_ptr> dot_graph
%type <dot_ptr> dot_utf8leadin
%type <dot_ptr> dot_strictleadin
%type <dot_ptr> dot_graphleadin
%type <dot_ptr> dot_graphidleadin
%type <dot_ptr> dot_stmt_list
%type <dot_ptr> dot_stmt_semi
%type <dot_ptr> dot_stmt
%type <dot_ptr> dot_node_stmt
%type <dot_ptr> dot_edge_stmt
%type <dot_ptr> dot_edge_rhs_list
%type <dot_ptr> dot_edge_rhs
%type <dot_ptr> dot_attr_stmt
%type <dot_ptr> dot_id_equal
%type <dot_ptr> dot_subgraph
%type <dot_ptr> dot_node_or_subgraph
%type <dot_ptr> dot_id
%type <dot_ptr> dot_ident
%type <dot_ptr> dot_string
%type <dot_ptr> dot_a_string
%type <dot_ptr> dot_number_id
%type <dot_ptr> dot_node_id
%type <dot_ptr> dot_opt_port
%type <dot_ptr> dot_port
%type <dot_ptr> dot_port_location
%type <dot_ptr> dot_port_angle
%type <dot_ptr> dot_opt_attr_list
%type <dot_ptr> dot_attr_list
%type <dot_ptr> dot_a_list
%type <dot_ptr> dot_a_list_item

/* */
%{

/* */
void dot_error (const char *s) 
{
    (void) printf("dot_error(): `%s' \n",s); 
    (void) fflush(stdout); 
    return;
}

%}

/* */
%union { char *dot_text; void *dot_ptr; }

/* */
%start dot_graphstart

%%

/* start here */
dot_graphstart: 
	  { } dot_graph { $$=0; }

/* leadin codes */
dot_graph:
	  dot_utf8leadin dot_strictleadin dot_graphleadin dot_graphidleadin '{' dot_stmt_list '}' { $$=0; }
	;

/* optional utf-8 leadin */
dot_utf8leadin:
	  DOTLEX_UTF8LEADIN { }
	| /* empty */ { }
	;

/* optional strict mode */
dot_strictleadin:
	  DOTLEX_STRICT { }
	| /* empty */ { }
	;

/* mandatory graph or digraph */
dot_graphleadin:
	  DOTLEX_GRAPH { $$=0; }
	| DOTLEX_DIGRAPH { $$=0; }
	;

/* optional graph name */
dot_graphidleadin:
	  dot_id { $$=0; }
	| /* empty */ { $$=0; }
	;

/* graph statement list */
dot_stmt_list:
	  dot_stmt_semi { $$=0; }
	| dot_stmt_list dot_stmt_semi { $$=0; }
	;

/* optional semicolon at statement */
dot_stmt_semi:
	  dot_stmt { $$=0; }
	| dot_stmt ';' { $$=0; }
	;

/* graph statements */
dot_stmt:
	  dot_node_stmt { $$=0; }
	| dot_edge_stmt { $$=0; }
	| dot_attr_stmt { $$=0; }
	| dot_id_equal { $$=0; }
	| dot_subgraph { $$=0; }
	;

/* node statements */
dot_node_stmt:
	  dot_node_id dot_opt_attr_list { $$=0; }
	;

/* edeg statements */
dot_edge_stmt:
	  dot_node_or_subgraph dot_edge_rhs_list dot_opt_attr_list { $$=0; }
	;

/* righthandside */
dot_edge_rhs_list:
	  dot_edge_rhs { $$=0; }
	| dot_edge_rhs_list dot_edge_rhs { $$=0; }
	;

/* righthandside */
dot_edge_rhs:
	  DOTLEX_DEDGE dot_node_or_subgraph { $$=0; }
	| DOTLEX_UEDGE dot_node_or_subgraph { $$=0; }
	;

/* attribute statement */
dot_attr_stmt:
	  DOTLEX_GRAPH dot_attr_list { $$=0; }
	| DOTLEX_NODE dot_attr_list { $$=0; }
	| DOTLEX_EDGE dot_attr_list { $$=0; }
	;

/* id=id */
dot_id_equal:
	  dot_id '=' dot_id { $$=0; }
	;

/* subgraph */
dot_subgraph:
	  DOTLEX_SUBGRAPH dot_id '{' dot_stmt_list '}' { $$=0; }
 	| DOTLEX_SUBGRAPH '{' dot_stmt_list '}' { $$=0; }
	| '{' dot_stmt_list '}' { $$=0; }
	| DOTLEX_SUBGRAPH dot_id %prec DOTLEX_LLBRACE { $$=0; }
	;

/* node or subgraph */
dot_node_or_subgraph:
	  dot_node_id { $$=0; }
	| dot_subgraph { $$=0; }
	;

/* identifier */
dot_id:
	  dot_ident { $$=0; }
	| dot_string { $$=0; }
	| dot_number_id { $$=0; }
	;

/* identifier */
dot_ident:
	  DOTLEX_IDENT { $$=0; }
	;

/* optional cat string */
dot_string:
	  dot_a_string { $$=0; }
	| dot_string '+' dot_a_string { $$=0; }
	;

/* string */
dot_a_string:
	  DOTLEX_STRING { $$=0; }
	;

/* number */
dot_number_id:
	  DOTLEX_NUMBER { $$=0; }
	;

/* node with optional port */
dot_node_id:
	  dot_id dot_opt_port { $$=0; }
	;

/* optional nodeport */
dot_opt_port:
	  dot_port { $$=0; }
	| /* empty */ { $$=0; }
	;

/* nodeport angle */
dot_port:
	  dot_port_location { $$=0; }
	| dot_port_location dot_port_angle { $$=0; }
	| dot_port_angle dot_port_location { $$=0; }
	| dot_port_angle { $$=0; }
	;

/* nodeport location */
dot_port_location:
	  ':' dot_id { $$=0; }
	| ':' '(' dot_id ',' dot_id ')' { $$=0; }
	;

/* nodeport angle */
dot_port_angle:
	  '@' dot_id { $$=0; }
	;

/* optional attribute list */
dot_opt_attr_list:
	  dot_attr_list { $$=0; }
	| /* empty */ { $$=0; }
	;

/* attribute list */
dot_attr_list:
	  '[' dot_a_list ']' { $$=0; }
	| dot_attr_list '[' dot_a_list ']' { $$=0; }
	;

/* list of items */
dot_a_list:
	  dot_a_list_item { $$=0; }
	| dot_a_list dot_a_list_item { $$=0; }
	; 

/* one item */
dot_a_list_item:
	  dot_id '=' dot_id ',' { $$=0; }
	| dot_id '=' dot_id { $$=0; }
	| dot_id ',' { $$=0; }
	| dot_id { $$=0; }
	| /* empty */ { $$=0; }
	;

%%

/* End. */
