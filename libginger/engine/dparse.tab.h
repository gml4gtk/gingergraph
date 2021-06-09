/* A Bison parser, made by GNU Bison 2.1a.  */

/* Skeleton parser for Yacc-like parsing with Bison,
   Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005 Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.  */

/* As a special exception, when this file is copied by Bison into a
   Bison output file, you may use that output file without restriction.
   This special exception was added by the Free Software Foundation
   in version 1.24 of Bison.  */

/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     DOTLEX_NODE = 258,
     DOTLEX_EDGE = 259,
     DOTLEX_GRAPH = 260,
     DOTLEX_DIGRAPH = 261,
     DOTLEX_SUBGRAPH = 262,
     DOTLEX_STRICT = 263,
     DOTLEX_DEDGE = 264,
     DOTLEX_UEDGE = 265,
     DOTLEX_UTF8LEADIN = 266,
     DOTLEX_IDENT = 267,
     DOTLEX_STRING = 268,
     DOTLEX_NUMBER = 269,
     DOTLEX_LLBRACE = 270
   };
#endif
/* Tokens.  */
#define DOTLEX_NODE 258
#define DOTLEX_EDGE 259
#define DOTLEX_GRAPH 260
#define DOTLEX_DIGRAPH 261
#define DOTLEX_SUBGRAPH 262
#define DOTLEX_STRICT 263
#define DOTLEX_DEDGE 264
#define DOTLEX_UEDGE 265
#define DOTLEX_UTF8LEADIN 266
#define DOTLEX_IDENT 267
#define DOTLEX_STRING 268
#define DOTLEX_NUMBER 269
#define DOTLEX_LLBRACE 270




#if ! defined (YYSTYPE) && ! defined (YYSTYPE_IS_DECLARED)
#line 227 "dparse.y"
typedef union YYSTYPE { char *dot_text; void *dot_ptr; } YYSTYPE;
/* Line 1543 of yacc.c.  */
#line 70 "dparse.tab.h"
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif

extern YYSTYPE yylval;



