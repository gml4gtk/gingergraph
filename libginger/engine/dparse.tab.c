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

/* As a special exception, when this parser skeleton is copied by
   Bison into a Bison output file, you may use that output file
   without restriction.  This special exception was added by the Free
   Software Foundation for C LALR(1) parsers in version 1.24 of
   Bison.  */

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "2.1a"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Using locations.  */
#define YYLSP_NEEDED 0



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




/* Copy the first part of user declarations.  */
#line 29 "dparse.y"


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

#line 214 "dparse.y"


/* */
void dot_error (const char *s) 
{
    (void) printf("dot_error(): `%s' \n",s); 
    (void) fflush(stdout); 
    return;
}



/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 1
#endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* Enabling the token table.  */
#ifndef YYTOKEN_TABLE
# define YYTOKEN_TABLE 0
#endif

#if ! defined (YYSTYPE) && ! defined (YYSTYPE_IS_DECLARED)
#line 227 "dparse.y"
typedef union YYSTYPE { char *dot_text; void *dot_ptr; } YYSTYPE;
/* Line 197 of yacc.c.  */
#line 263 "dparse.tab.c"
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif



/* Copy the second part of user declarations.  */


/* Line 220 of yacc.c.  */
#line 275 "dparse.tab.c"

/* Define YYMODERN_C if this compiler supports C89 or better.  If
   __STDC__ is defined, the compiler is modern.  IBM xlc 7.0 when run
   as 'cc' doesn't define __STDC__ (or __STDC_VERSION__) for pedantic
   reasons, but it defines __C99__FUNC__ so check that as well.
   Consider a C++ compiler to be modern if it defines __cplusplus.  */
#ifndef YYMODERN_C
# if defined (__STDC__) || defined (__C99__FUNC__) || defined (__cplusplus)
#  define YYMODERN_C 1
# else
#  define YYMODERN_C 0
# endif
#endif

#ifndef YYSIZE_T
# if defined (__SIZE_TYPE__)
#  define YYSIZE_T __SIZE_TYPE__
# elif defined (size_t)
#  define YYSIZE_T size_t
# elif ! defined (YYSIZE_T) && YYMODERN_C
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#ifndef lint
# define YYUSE(e) ((void) (e))
#else
# define YYUSE(e) /* empty */
#endif

/* Identity function, used to suppress warnings about constant conditions.  */
#ifndef lint
# define YYID(n) (n)
#else
#if defined (__STDC__) || defined (__C99__FUNC__) || defined (__cplusplus)
static int
YYID (int i)
#else
static int
YYID (i)
    int i;
#endif
{
  return i;
}
#endif

#if ! defined (yyoverflow) || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined (__BUILTIN_VA_ARG_INCR)
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined (_AIX)
#    define YYSTACK_ALLOC __alloca
#   elif defined (_MSC_VER)
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined (_ALLOCA_H) && ! defined (_STDLIB_H) && YYMODERN_C
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#     ifndef _STDLIB_H
#      define _STDLIB_H 1
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's `empty if-body' warning. */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (YYID (0))
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2005 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  ifdef __cplusplus
extern "C" {
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined (malloc) && ! defined (_STDLIB_H) && YYMODERN_C
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined (free) && ! defined (_STDLIB_H)  && YYMODERN_C
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifdef __cplusplus
}
#  endif
# endif
#endif /* ! defined (yyoverflow) || YYERROR_VERBOSE */


#if (! defined (yyoverflow) \
     && (! defined (__cplusplus) \
	 || (defined (YYSTYPE_IS_TRIVIAL) && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  short int yyss;
  YYSTYPE yyvs;
  };

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (short int) + sizeof (YYSTYPE))			\
      + YYSTACK_GAP_MAXIMUM)

/* Copy COUNT objects from FROM to TO.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined (__GNUC__) && 1 < __GNUC__
#   define YYCOPY(To, From, Count) \
      __builtin_memcpy (To, From, (Count) * sizeof (*(From)))
#  else
#   define YYCOPY(To, From, Count)		\
      do					\
	{					\
	  YYSIZE_T yyi;				\
	  for (yyi = 0; yyi < (Count); yyi++)	\
	    (To)[yyi] = (From)[yyi];		\
	}					\
      while (YYID (0))
#  endif
# endif

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack)					\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack, Stack, yysize);				\
	Stack = &yyptr->Stack;						\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (YYID (0))

#endif

#if YYMODERN_C
   typedef signed char yysigned_char;
#else
   typedef short int yysigned_char;
#endif

/* YYFINAL -- State number of the termination state. */
#define YYFINAL  3
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   139

/* YYNTOKENS -- Number of terminals. */
#define YYNTOKENS  28
/* YYNNTS -- Number of nonterminals. */
#define YYNNTS  33
/* YYNRULES -- Number of rules. */
#define YYNRULES  66
/* YYNRULES -- Number of states. */
#define YYNSTATES  98

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   270

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const unsigned char yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
      22,    24,     2,    20,    23,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    21,    18,
       2,    19,     2,     2,    25,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    26,     2,    27,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    16,     2,    17,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const unsigned char yyprhs[] =
{
       0,     0,     3,     4,     7,    15,    17,    18,    20,    21,
      23,    25,    27,    28,    30,    33,    35,    38,    40,    42,
      44,    46,    48,    51,    55,    57,    60,    63,    66,    69,
      72,    75,    79,    85,    90,    94,    97,    99,   101,   103,
     105,   107,   109,   111,   115,   117,   119,   122,   124,   125,
     127,   130,   133,   135,   138,   145,   148,   150,   151,   155,
     160,   162,   165,   170,   174,   177,   179
};

/* YYRHS -- A `-1'-separated list of the rules' RHS. */
static const yysigned_char yyrhs[] =
{
      29,     0,    -1,    -1,    30,    31,    -1,    32,    33,    34,
      35,    16,    36,    17,    -1,    11,    -1,    -1,     8,    -1,
      -1,     5,    -1,     6,    -1,    47,    -1,    -1,    37,    -1,
      36,    37,    -1,    38,    -1,    38,    18,    -1,    39,    -1,
      40,    -1,    43,    -1,    44,    -1,    45,    -1,    52,    57,
      -1,    46,    41,    57,    -1,    42,    -1,    41,    42,    -1,
       9,    46,    -1,    10,    46,    -1,     5,    58,    -1,     3,
      58,    -1,     4,    58,    -1,    47,    19,    47,    -1,     7,
      47,    16,    36,    17,    -1,     7,    16,    36,    17,    -1,
      16,    36,    17,    -1,     7,    47,    -1,    52,    -1,    45,
      -1,    48,    -1,    49,    -1,    51,    -1,    12,    -1,    50,
      -1,    49,    20,    50,    -1,    13,    -1,    14,    -1,    47,
      53,    -1,    54,    -1,    -1,    55,    -1,    55,    56,    -1,
      56,    55,    -1,    56,    -1,    21,    47,    -1,    21,    22,
      47,    23,    47,    24,    -1,    25,    47,    -1,    58,    -1,
      -1,    26,    59,    27,    -1,    58,    26,    59,    27,    -1,
      60,    -1,    59,    60,    -1,    47,    19,    47,    23,    -1,
      47,    19,    47,    -1,    47,    23,    -1,    47,    -1,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const unsigned short int yyrline[] =
{
       0,   236,   236,   236,   240,   245,   246,   251,   252,   257,
     258,   263,   264,   269,   270,   275,   276,   281,   282,   283,
     284,   285,   290,   295,   300,   301,   306,   307,   312,   313,
     314,   319,   324,   325,   326,   327,   332,   333,   338,   339,
     340,   345,   350,   351,   356,   361,   366,   371,   372,   377,
     378,   379,   380,   385,   386,   391,   396,   397,   402,   403,
     408,   409,   414,   415,   416,   417,   418
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals. */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "DOTLEX_NODE", "DOTLEX_EDGE",
  "DOTLEX_GRAPH", "DOTLEX_DIGRAPH", "DOTLEX_SUBGRAPH", "DOTLEX_STRICT",
  "DOTLEX_DEDGE", "DOTLEX_UEDGE", "DOTLEX_UTF8LEADIN", "DOTLEX_IDENT",
  "DOTLEX_STRING", "DOTLEX_NUMBER", "DOTLEX_LLBRACE", "'{'", "'}'", "';'",
  "'='", "'+'", "':'", "'('", "','", "')'", "'@'", "'['", "']'", "$accept",
  "dot_graphstart", "@1", "dot_graph", "dot_utf8leadin",
  "dot_strictleadin", "dot_graphleadin", "dot_graphidleadin",
  "dot_stmt_list", "dot_stmt_semi", "dot_stmt", "dot_node_stmt",
  "dot_edge_stmt", "dot_edge_rhs_list", "dot_edge_rhs", "dot_attr_stmt",
  "dot_id_equal", "dot_subgraph", "dot_node_or_subgraph", "dot_id",
  "dot_ident", "dot_string", "dot_a_string", "dot_number_id",
  "dot_node_id", "dot_opt_port", "dot_port", "dot_port_location",
  "dot_port_angle", "dot_opt_attr_list", "dot_attr_list", "dot_a_list",
  "dot_a_list_item", 0
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const unsigned short int yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   123,   125,    59,    61,
      43,    58,    40,    44,    41,    64,    91,    93
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const unsigned char yyr1[] =
{
       0,    28,    30,    29,    31,    32,    32,    33,    33,    34,
      34,    35,    35,    36,    36,    37,    37,    38,    38,    38,
      38,    38,    39,    40,    41,    41,    42,    42,    43,    43,
      43,    44,    45,    45,    45,    45,    46,    46,    47,    47,
      47,    48,    49,    49,    50,    51,    52,    53,    53,    54,
      54,    54,    54,    55,    55,    56,    57,    57,    58,    58,
      59,    59,    60,    60,    60,    60,    60
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const unsigned char yyr2[] =
{
       0,     2,     0,     2,     7,     1,     0,     1,     0,     1,
       1,     1,     0,     1,     2,     1,     2,     1,     1,     1,
       1,     1,     2,     3,     1,     2,     2,     2,     2,     2,
       2,     3,     5,     4,     3,     2,     1,     1,     1,     1,
       1,     1,     1,     3,     1,     1,     2,     1,     0,     1,
       2,     2,     1,     2,     6,     2,     1,     0,     3,     4,
       1,     2,     4,     3,     2,     1,     0
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const unsigned char yydefact[] =
{
       2,     0,     6,     1,     5,     3,     8,     7,     0,     9,
      10,    12,    41,    44,    45,     0,    11,    38,    39,    42,
      40,     0,     0,     0,     0,     0,     0,     0,     0,    13,
      15,    17,    18,    19,    20,    21,     0,    48,    57,    43,
      66,    29,    30,    28,     0,    35,     0,     4,    14,    16,
       0,     0,    57,    24,     0,     0,     0,    46,    47,    49,
      52,    22,    56,    65,     0,    60,    66,     0,     0,    34,
      37,    26,    48,    36,    27,    25,    23,    31,     0,    53,
      55,    50,    51,     0,    64,    58,    61,     0,    33,     0,
       0,    63,    59,    32,     0,    62,     0,    54
};

/* YYDEFGOTO[NTERM-NUM]. */
static const yysigned_char yydefgoto[] =
{
      -1,     1,     2,     5,     6,     8,    11,    15,    28,    29,
      30,    31,    32,    52,    53,    33,    34,    35,    36,    37,
      17,    18,    19,    20,    38,    57,    58,    59,    60,    61,
      62,    64,    65
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -64
static const yysigned_char yypact[] =
{
     -64,    19,    10,   -64,   -64,   -64,    30,   -64,    -2,   -64,
     -64,    23,   -64,   -64,   -64,    12,   -64,   -64,    32,   -64,
     -64,   107,    49,    33,    33,    33,   120,   107,    44,   -64,
      69,   -64,   -64,   -64,   -64,     1,    91,    67,    -3,   -64,
      23,    53,    53,    53,   107,    82,    61,   -64,   -64,   -64,
     115,   115,    -1,   -64,    23,    20,    23,   -64,   -64,    60,
      86,   -64,    53,     7,     0,   -64,    23,    77,   107,   -64,
     -64,   -64,    29,   -64,   -64,   -64,   -64,   -64,    23,   -64,
     -64,   -64,   -64,    23,   -64,   -64,   -64,     4,   -64,    92,
      90,    94,   -64,   -64,    23,   -64,   102,   -64
};

/* YYPGOTO[NTERM-NUM].  */
static const yysigned_char yypgoto[] =
{
     -64,   -64,   -64,   -64,   -64,   -64,   -64,   -64,   -22,   -26,
     -64,   -64,   -64,   -64,    66,   -64,   -64,    52,    65,   -11,
     -64,   -64,   108,   -64,    74,   -64,   -64,    75,    78,    87,
      46,    72,   -63
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -38
static const yysigned_char yytable[] =
{
      16,    86,    48,     9,    10,    46,   -36,   -36,    50,    51,
     -37,   -37,    12,    13,    14,    45,    12,    13,    14,     3,
      48,     4,    67,    40,    86,    40,    83,    85,    21,    63,
      84,    92,    12,    13,    14,    12,    13,    14,     7,    72,
      72,    48,    78,    77,    79,    80,    89,    23,    24,    25,
      55,    26,    22,    63,    56,    63,    12,    13,    14,    40,
      27,    47,    13,    48,    23,    24,    25,    90,    26,    41,
      42,    43,    91,    12,    13,    14,    63,    27,    69,    66,
      23,    24,    25,    96,    26,    56,    54,    49,    55,    12,
      13,    14,    56,    27,    88,    23,    24,    25,    68,    26,
      50,    51,    70,    70,    12,    13,    14,    55,    27,    93,
      23,    24,    25,    94,    26,    71,    74,    95,    75,    12,
      13,    14,    26,    27,    73,    73,    97,    12,    13,    14,
      39,    27,    12,    13,    14,    82,    44,    81,    87,    76
};

static const unsigned char yycheck[] =
{
      11,    64,    28,     5,     6,    27,     9,    10,     9,    10,
       9,    10,    12,    13,    14,    26,    12,    13,    14,     0,
      46,    11,    44,    26,    87,    26,    19,    27,    16,    40,
      23,    27,    12,    13,    14,    12,    13,    14,     8,    50,
      51,    67,    22,    54,    55,    56,    68,     3,     4,     5,
      21,     7,    20,    64,    25,    66,    12,    13,    14,    26,
      16,    17,    13,    89,     3,     4,     5,    78,     7,    23,
      24,    25,    83,    12,    13,    14,    87,    16,    17,    26,
       3,     4,     5,    94,     7,    25,    19,    18,    21,    12,
      13,    14,    25,    16,    17,     3,     4,     5,    16,     7,
       9,    10,    50,    51,    12,    13,    14,    21,    16,    17,
       3,     4,     5,    23,     7,    50,    51,    23,    52,    12,
      13,    14,     7,    16,    50,    51,    24,    12,    13,    14,
      22,    16,    12,    13,    14,    60,    16,    59,    66,    52
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const unsigned char yystos[] =
{
       0,    29,    30,     0,    11,    31,    32,     8,    33,     5,
       6,    34,    12,    13,    14,    35,    47,    48,    49,    50,
      51,    16,    20,     3,     4,     5,     7,    16,    36,    37,
      38,    39,    40,    43,    44,    45,    46,    47,    52,    50,
      26,    58,    58,    58,    16,    47,    36,    17,    37,    18,
       9,    10,    41,    42,    19,    21,    25,    53,    54,    55,
      56,    57,    58,    47,    59,    60,    26,    36,    16,    17,
      45,    46,    47,    52,    46,    42,    57,    47,    22,    47,
      47,    56,    55,    19,    23,    27,    60,    59,    17,    36,
      47,    47,    27,    17,    23,    23,    47,    24
};

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		(-2)
#define YYEOF		0

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrorlab


/* Like YYERROR except do call yyerror.  This remains here temporarily
   to ease the transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  */

#define YYFAIL		goto yyerrlab

#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)					\
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    {								\
      yychar = (Token);						\
      yylval = (Value);						\
      yytoken = YYTRANSLATE (yychar);				\
      YYPOPSTACK;						\
      goto yybackup;						\
    }								\
  else								\
    {								\
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;							\
    }								\
while (YYID (0))


#define YYTERROR	1
#define YYERRCODE	256


/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#define YYRHSLOC(Rhs, K) ((Rhs)[K])
#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)				\
    do									\
      if (N)								\
	{								\
	  (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;	\
	  (Current).first_column = YYRHSLOC (Rhs, 1).first_column;	\
	  (Current).last_line    = YYRHSLOC (Rhs, N).last_line;		\
	  (Current).last_column  = YYRHSLOC (Rhs, N).last_column;	\
	}								\
      else								\
	{								\
	  (Current).first_line   = (Current).last_line   =		\
	    YYRHSLOC (Rhs, 0).last_line;				\
	  (Current).first_column = (Current).last_column =		\
	    YYRHSLOC (Rhs, 0).last_column;				\
	}								\
    while (YYID (0))
#endif


/* YY_LOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

#ifndef YY_LOCATION_PRINT
# if YYLTYPE_IS_TRIVIAL
#  define YY_LOCATION_PRINT(File, Loc)			\
     fprintf (File, "%d.%d-%d.%d",			\
              (Loc).first_line, (Loc).first_column,	\
              (Loc).last_line,  (Loc).last_column)
# else
#  define YY_LOCATION_PRINT(File, Loc) ((void) 0)
# endif
#endif


/* YYLEX -- calling `yylex' with the right arguments.  */

#ifdef YYLEX_PARAM
# define YYLEX yylex (YYLEX_PARAM)
#else
# define YYLEX yylex ()
#endif

/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)			\
do {						\
  if (yydebug)					\
    YYFPRINTF Args;				\
} while (YYID (0))

# define YY_SYMBOL_PRINT(Title, Type, Value, Location)			  \
do {									  \
  if (yydebug)								  \
    {									  \
      YYFPRINTF (stderr, "%s ", Title);					  \
      yysymprint (stderr,						  \
                  Type, Value); \
      YYFPRINTF (stderr, "\n");						  \
    }									  \
} while (YYID (0))

/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

/*ARGSUSED*/
#if defined (__STDC__) || defined (__C99__FUNC__) || defined (__cplusplus)
static void
yysymprint (FILE *yyoutput, int yytype, const YYSTYPE * const yyvaluep)
#else
static void
yysymprint (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    const YYSTYPE * const yyvaluep;
#endif
{
  YYUSE (yyvaluep);
  if (yytype < YYNTOKENS)
    YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);


# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# endif
  switch (yytype)
    {
      default:
        break;
    }
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

#if defined (__STDC__) || defined (__C99__FUNC__) || defined (__cplusplus)
static void
yy_stack_print (short int *bottom, short int *top)
#else
static void
yy_stack_print (bottom, top)
    short int *bottom;
    short int *top;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (/* Nothing. */; bottom <= top; ++bottom)
    YYFPRINTF (stderr, " %d", *bottom);
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)				\
do {								\
  if (yydebug)							\
    yy_stack_print ((Bottom), (Top));				\
} while (YYID (0))


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

#if defined (__STDC__) || defined (__C99__FUNC__) || defined (__cplusplus)
static void
yy_reduce_print (YYSTYPE *yyvsp, 
		   int yyrule)
#else
static void
yy_reduce_print (yyvsp, yyrule
                   )
    YYSTYPE *yyvsp;
    
		   int yyrule;
#endif
{
  int yynrhs = yyr2[yyrule];
  int yyi;
  unsigned long int yylno = yyrline[yyrule];
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      fprintf (stderr, "   $%d = ", yyi + 1);
      yysymprint (stderr, yyrhs[yyprhs[yyrule] + yyi],
                  &(yyvsp[(yyi + 1) - (yynrhs)])
                  	          );
      fprintf (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (yyvsp, Rule); \
} while (YYID (0))

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef	YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif



#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined (__GLIBC__) && defined (_STRING_H)
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
static YYSIZE_T
#   if YYMODERN_C
yystrlen (const char *yystr)
#   else
yystrlen (yystr)
     const char *yystr;
#   endif
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined (__GLIBC__) && defined (_STRING_H) && defined (_GNU_SOURCE)
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *
#   if YYMODERN_C
yystpcpy (char *yydest, const char *yysrc)
#   else
yystpcpy (yydest, yysrc)
     char *yydest;
     const char *yysrc;
#   endif
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      size_t yyn = 0;
      char const *yyp = yystr;

      for (;;)
	switch (*++yyp)
	  {
	  case '\'':
	  case ',':
	    goto do_not_strip_quotes;

	  case '\\':
	    if (*++yyp != '\\')
	      goto do_not_strip_quotes;
	    /* Fall through.  */
	  default:
	    if (yyres)
	      yyres[yyn] = *yyp;
	    yyn++;
	    break;

	  case '"':
	    if (yyres)
	      yyres[yyn] = '\0';
	    return yyn;
	  }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

/* Copy into YYRESULT an error message about the unexpected token
   YYCHAR while in state YYSTATE.  Return the number of bytes copied,
   including the terminating null byte.  If YYRESULT is null, do not
   copy anything; just return the number of bytes that would be
   copied.  As a special case, return 0 if an ordinary "syntax error"
   message will do.  Return YYSIZE_MAXIMUM if overflow occurs during
   size calculation.  */
static YYSIZE_T
yysyntax_error (char *yyresult, int yystate, int yychar)
{
  int yyn = yypact[yystate];

  if (! (YYPACT_NINF < yyn && yyn < YYLAST))
    return 0;
  else
    {
      int yytype = YYTRANSLATE (yychar);
      YYSIZE_T yysize0 = yytnamerr (0, yytname[yytype]);
      YYSIZE_T yysize = yysize0;
      YYSIZE_T yysize1;
      int yysize_overflow = 0;
      enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
      char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
      int yyx;

# if 0
      /* This is so xgettext sees the translatable formats that are
         constructed on the fly.  */
      YY_("syntax error, unexpected %s");
      YY_("syntax error, unexpected %s, expecting %s");
      YY_("syntax error, unexpected %s, expecting %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s");
# endif
      char *yyfmt;
      char const *yyf;
      static char const yyunexpected[] = "syntax error, unexpected %s";
      static char const yyexpecting[] = ", expecting %s";
      static char const yyor[] = " or %s";
      char yyformat[sizeof yyunexpected
                    + sizeof yyexpecting - 1
                    + ((YYERROR_VERBOSE_ARGS_MAXIMUM - 2)
                       * (sizeof yyor - 1))];
      char const *yyprefix = yyexpecting;

      /* Start YYX at -YYN if negative to avoid negative indexes in
         YYCHECK.  */
      int yyxbegin = yyn < 0 ? -yyn : 0;

      /* Stay within bounds of both yycheck and yytname.  */
      int yychecklim = YYLAST - yyn;
      int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
      int yycount = 1;

      yyarg[0] = yytname[yytype];
      yyfmt = yystpcpy (yyformat, yyunexpected);

      for (yyx = yyxbegin; yyx < yyxend; ++yyx)
        if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR)
          {
            if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
              {
                yycount = 1;
                yysize = yysize0;
                yyformat[sizeof yyunexpected - 1] = '\0';
                break;
              }
            yyarg[yycount++] = yytname[yyx];
            yysize1 = yysize + yytnamerr (0, yytname[yyx]);
            yysize_overflow |= (yysize1 < yysize);
            yysize = yysize1;
            yyfmt = yystpcpy (yyfmt, yyprefix);
            yyprefix = yyor;
          }

      yyf = YY_(yyformat);
      yysize1 = yysize + yystrlen (yyf);
      yysize_overflow |= (yysize1 < yysize);
      yysize = yysize1;

      if (yysize_overflow)
	return YYSIZE_MAXIMUM;

      if (yyresult)
	{
          /* Avoid sprintf, as that infringes on the user's name space.
             Don't have undefined behavior even if the translation
             produced a string with the wrong number of "%s"s.  */
          char *yyp = yyresult;
          int yyi = 0;
          while ((*yyp = *yyf) != '\0')
            {
              if (*yyp == '%' && yyf[1] == 's' && yyi < yycount)
                {
                  yyp += yytnamerr (yyp, yyarg[yyi++]);
                  yyf += 2;
                }
              else
                {
                  yyp++;
                  yyf++;
                }
            }
        }
      return yysize;
    }
}
#endif /* YYERROR_VERBOSE */


/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

/*ARGSUSED*/
#if defined (__STDC__) || defined (__C99__FUNC__) || defined (__cplusplus)
static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
#else
static void
yydestruct (yymsg, yytype, yyvaluep)
    const char *yymsg;
    int yytype;
    YYSTYPE *yyvaluep;
#endif
{
  YYUSE (yyvaluep);

  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  switch (yytype)
    {

      default:
        break;
    }
}


/* Prevent warnings from -Wmissing-prototypes.  */

#ifdef YYPARSE_PARAM
# if YYMODERN_C
int yyparse (void *YYPARSE_PARAM);
# else
int yyparse ();
# endif
#else /* ! YYPARSE_PARAM */
#if defined (__STDC__) || defined (__cplusplus)
int yyparse (void);
#else
int yyparse ();
#endif
#endif /* ! YYPARSE_PARAM */



/* The look-ahead symbol.  */
int yychar;

/* The semantic value of the look-ahead symbol.  */
YYSTYPE yylval;

/* Number of syntax errors so far.  */
int yynerrs;



/*----------.
| yyparse.  |
`----------*/

#ifdef YYPARSE_PARAM
# if YYMODERN_C
int yyparse (void *YYPARSE_PARAM)
# else
int yyparse (YYPARSE_PARAM)
  void *YYPARSE_PARAM;
# endif
#else /* ! YYPARSE_PARAM */
#if defined (__STDC__) || defined (__C99__FUNC__) || defined (__cplusplus)
int
yyparse (void)
#else
int
yyparse ()

#endif
#endif
{
  
  int yystate;
  int yyn;
  int yyresult;
  /* Number of tokens to shift before error messages enabled.  */
  int yyerrstatus;
  /* Look-ahead token as an internal (translated) token number.  */
  int yytoken = 0;
#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

  /* Three stacks and their tools:
     `yyss': related to states,
     `yyvs': related to semantic values,
     `yyls': related to locations.

     Refer to the stacks thru separate pointers, to allow yyoverflow
     to reallocate them elsewhere.  */

  /* The state stack.  */
  short int yyssa[YYINITDEPTH];
  short int *yyss = yyssa;
  short int *yyssp;

  /* The semantic value stack.  */
  YYSTYPE yyvsa[YYINITDEPTH];
  YYSTYPE *yyvs = yyvsa;
  YYSTYPE *yyvsp;



#define YYPOPSTACK   (yyvsp--, yyssp--)

  YYSIZE_T yystacksize = YYINITDEPTH;

  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;


  /* When reducing, the number of symbols on the RHS of the reduced
     rule.  */
  int yylen;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY;		/* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */

  yyssp = yyss;
  yyvsp = yyvs;

  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed. so pushing a state here evens the stacks.
     */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
	/* Give user a chance to reallocate the stack. Use copies of
	   these so that the &'s don't force the real ones into
	   memory.  */
	YYSTYPE *yyvs1 = yyvs;
	short int *yyss1 = yyss;


	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  This used to be a
	   conditional around just the two extra args, but that might
	   be undefined if yyoverflow is a macro.  */
	yyoverflow (YY_("memory exhausted"),
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),

		    &yystacksize);

	yyss = yyss1;
	yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
	goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
	yystacksize = YYMAXDEPTH;

      {
	short int *yyss1 = yyss;
	union yyalloc *yyptr =
	  (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
	if (! yyptr)
	  goto yyexhaustedlab;
	YYSTACK_RELOCATE (yyss);
	YYSTACK_RELOCATE (yyvs);

#  undef YYSTACK_RELOCATE
	if (yyss1 != yyssa)
	  YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;


      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
		  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
	YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

/* Do appropriate processing given the current state.  */
/* Read a look-ahead token if we need one and don't already have one.  */
/* yyresume: */

  /* First try to decide what to do without reference to look-ahead token.  */

  yyn = yypact[yystate];
  if (yyn == YYPACT_NINF)
    goto yydefault;

  /* Not known => get a look-ahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid look-ahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = YYLEX;
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yyn == 0 || yyn == YYTABLE_NINF)
	goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  if (yyn == YYFINAL)
    YYACCEPT;

  /* Shift the look-ahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the token being shifted unless it is eof.  */
  if (yychar != YYEOF)
    yychar = YYEMPTY;

  *++yyvsp = yylval;


  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  yystate = yyn;
  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     `$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 2:
#line 236 "dparse.y"
    { }
    break;

  case 3:
#line 236 "dparse.y"
    { (yyval.dot_ptr)=0; }
    break;

  case 4:
#line 240 "dparse.y"
    { (yyval.dot_ptr)=0; }
    break;

  case 5:
#line 245 "dparse.y"
    { }
    break;

  case 6:
#line 246 "dparse.y"
    { }
    break;

  case 7:
#line 251 "dparse.y"
    { }
    break;

  case 8:
#line 252 "dparse.y"
    { }
    break;

  case 9:
#line 257 "dparse.y"
    { (yyval.dot_ptr)=0; }
    break;

  case 10:
#line 258 "dparse.y"
    { (yyval.dot_ptr)=0; }
    break;

  case 11:
#line 263 "dparse.y"
    { (yyval.dot_ptr)=0; }
    break;

  case 12:
#line 264 "dparse.y"
    { (yyval.dot_ptr)=0; }
    break;

  case 13:
#line 269 "dparse.y"
    { (yyval.dot_ptr)=0; }
    break;

  case 14:
#line 270 "dparse.y"
    { (yyval.dot_ptr)=0; }
    break;

  case 15:
#line 275 "dparse.y"
    { (yyval.dot_ptr)=0; }
    break;

  case 16:
#line 276 "dparse.y"
    { (yyval.dot_ptr)=0; }
    break;

  case 17:
#line 281 "dparse.y"
    { (yyval.dot_ptr)=0; }
    break;

  case 18:
#line 282 "dparse.y"
    { (yyval.dot_ptr)=0; }
    break;

  case 19:
#line 283 "dparse.y"
    { (yyval.dot_ptr)=0; }
    break;

  case 20:
#line 284 "dparse.y"
    { (yyval.dot_ptr)=0; }
    break;

  case 21:
#line 285 "dparse.y"
    { (yyval.dot_ptr)=0; }
    break;

  case 22:
#line 290 "dparse.y"
    { (yyval.dot_ptr)=0; }
    break;

  case 23:
#line 295 "dparse.y"
    { (yyval.dot_ptr)=0; }
    break;

  case 24:
#line 300 "dparse.y"
    { (yyval.dot_ptr)=0; }
    break;

  case 25:
#line 301 "dparse.y"
    { (yyval.dot_ptr)=0; }
    break;

  case 26:
#line 306 "dparse.y"
    { (yyval.dot_ptr)=0; }
    break;

  case 27:
#line 307 "dparse.y"
    { (yyval.dot_ptr)=0; }
    break;

  case 28:
#line 312 "dparse.y"
    { (yyval.dot_ptr)=0; }
    break;

  case 29:
#line 313 "dparse.y"
    { (yyval.dot_ptr)=0; }
    break;

  case 30:
#line 314 "dparse.y"
    { (yyval.dot_ptr)=0; }
    break;

  case 31:
#line 319 "dparse.y"
    { (yyval.dot_ptr)=0; }
    break;

  case 32:
#line 324 "dparse.y"
    { (yyval.dot_ptr)=0; }
    break;

  case 33:
#line 325 "dparse.y"
    { (yyval.dot_ptr)=0; }
    break;

  case 34:
#line 326 "dparse.y"
    { (yyval.dot_ptr)=0; }
    break;

  case 35:
#line 327 "dparse.y"
    { (yyval.dot_ptr)=0; }
    break;

  case 36:
#line 332 "dparse.y"
    { (yyval.dot_ptr)=0; }
    break;

  case 37:
#line 333 "dparse.y"
    { (yyval.dot_ptr)=0; }
    break;

  case 38:
#line 338 "dparse.y"
    { (yyval.dot_ptr)=0; }
    break;

  case 39:
#line 339 "dparse.y"
    { (yyval.dot_ptr)=0; }
    break;

  case 40:
#line 340 "dparse.y"
    { (yyval.dot_ptr)=0; }
    break;

  case 41:
#line 345 "dparse.y"
    { (yyval.dot_ptr)=0; }
    break;

  case 42:
#line 350 "dparse.y"
    { (yyval.dot_ptr)=0; }
    break;

  case 43:
#line 351 "dparse.y"
    { (yyval.dot_ptr)=0; }
    break;

  case 44:
#line 356 "dparse.y"
    { (yyval.dot_ptr)=0; }
    break;

  case 45:
#line 361 "dparse.y"
    { (yyval.dot_ptr)=0; }
    break;

  case 46:
#line 366 "dparse.y"
    { (yyval.dot_ptr)=0; }
    break;

  case 47:
#line 371 "dparse.y"
    { (yyval.dot_ptr)=0; }
    break;

  case 48:
#line 372 "dparse.y"
    { (yyval.dot_ptr)=0; }
    break;

  case 49:
#line 377 "dparse.y"
    { (yyval.dot_ptr)=0; }
    break;

  case 50:
#line 378 "dparse.y"
    { (yyval.dot_ptr)=0; }
    break;

  case 51:
#line 379 "dparse.y"
    { (yyval.dot_ptr)=0; }
    break;

  case 52:
#line 380 "dparse.y"
    { (yyval.dot_ptr)=0; }
    break;

  case 53:
#line 385 "dparse.y"
    { (yyval.dot_ptr)=0; }
    break;

  case 54:
#line 386 "dparse.y"
    { (yyval.dot_ptr)=0; }
    break;

  case 55:
#line 391 "dparse.y"
    { (yyval.dot_ptr)=0; }
    break;

  case 56:
#line 396 "dparse.y"
    { (yyval.dot_ptr)=0; }
    break;

  case 57:
#line 397 "dparse.y"
    { (yyval.dot_ptr)=0; }
    break;

  case 58:
#line 402 "dparse.y"
    { (yyval.dot_ptr)=0; }
    break;

  case 59:
#line 403 "dparse.y"
    { (yyval.dot_ptr)=0; }
    break;

  case 60:
#line 408 "dparse.y"
    { (yyval.dot_ptr)=0; }
    break;

  case 61:
#line 409 "dparse.y"
    { (yyval.dot_ptr)=0; }
    break;

  case 62:
#line 414 "dparse.y"
    { (yyval.dot_ptr)=0; }
    break;

  case 63:
#line 415 "dparse.y"
    { (yyval.dot_ptr)=0; }
    break;

  case 64:
#line 416 "dparse.y"
    { (yyval.dot_ptr)=0; }
    break;

  case 65:
#line 417 "dparse.y"
    { (yyval.dot_ptr)=0; }
    break;

  case 66:
#line 418 "dparse.y"
    { (yyval.dot_ptr)=0; }
    break;


      default: break;
    }
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

/* Line __line__ of yacc.c.  */
#line 1852 "dparse.tab.c"

  yyvsp -= yylen;
  yyssp -= yylen;


  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;


  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*------------------------------------.
| yyerrlab -- here on detecting error |
`------------------------------------*/
yyerrlab:
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
      {
	YYSIZE_T yysize = yysyntax_error (0, yystate, yychar);
	if (yymsg_alloc < yysize && yymsg_alloc < YYSTACK_ALLOC_MAXIMUM)
	  {
	    YYSIZE_T yyalloc = 2 * yysize;
	    if (! (yysize <= yyalloc && yyalloc <= YYSTACK_ALLOC_MAXIMUM))
	      yyalloc = YYSTACK_ALLOC_MAXIMUM;
	    if (yymsg != yymsgbuf)
	      YYSTACK_FREE (yymsg);
	    yymsg = (char *) YYSTACK_ALLOC (yyalloc);
	    if (yymsg)
	      yymsg_alloc = yyalloc;
	    else
	      {
		yymsg = yymsgbuf;
		yymsg_alloc = sizeof yymsgbuf;
	      }
	  }

	if (0 < yysize && yysize <= yymsg_alloc)
	  {
	    (void) yysyntax_error (yymsg, yystate, yychar);
	    yyerror (yymsg);
	  }
	else
	  {
	    yyerror (YY_("syntax error"));
	    if (yysize != 0)
	      goto yyexhaustedlab;
	  }
      }
#endif
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse look-ahead token after an
	 error, discard it.  */

      if (yychar <= YYEOF)
        {
	  /* Return failure if at end of input.  */
	  if (yychar == YYEOF)
	    YYABORT;
        }
      else
	{
	  yydestruct ("Error: discarding",
                      yytoken, &yylval);
	  yychar = YYEMPTY;
	}
    }

  /* Else will try to reuse look-ahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

yyvsp -= yylen;
  yyssp -= yylen;
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;	/* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (yyn != YYPACT_NINF)
	{
	  yyn += YYTERROR;
	  if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
	    {
	      yyn = yytable[yyn];
	      if (0 < yyn)
		break;
	    }
	}

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
	YYABORT;


      yydestruct ("Error: popping",
                  yystos[yystate], yyvsp);
      YYPOPSTACK;
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  if (yyn == YYFINAL)
    YYACCEPT;

  *++yyvsp = yylval;


  /* Shift the error token. */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#ifndef yyoverflow
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEOF && yychar != YYEMPTY)
     yydestruct ("Cleanup: discarding lookahead",
		 yytoken, &yylval);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
		  yystos[*yyssp], yyvsp);
      YYPOPSTACK;
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  return yyresult;
}


#line 421 "dparse.y"


/* End. */

