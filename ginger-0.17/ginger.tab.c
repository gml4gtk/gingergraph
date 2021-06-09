
/*  A Bison parser, made from ginger.y with Bison version GNU Bison version 1.21
  */

#define YYBISON 1  /* Identify Bison output.  */

#define	NODE	258
#define	EDGE	259
#define	TEMPLATE	260
#define	LABEL	261
#define	SHAPE	262
#define	CONTENTS	263
#define	EDITOR	264
#define	COLOR	265
#define	TO	266
#define	FROM	267
#define	ID	268
#define	STRING	269
#define	X	270
#define	Y	271
#define	WIDTH	272
#define	HEIGHT	273
#define	INT	274
#define	DIRECTION	275
#define	BACKGROUND	276
#define	NORMAL	277
#define	INVERSE	278
#define	BOTH	279
#define	NONE	280
#define	RECTANGLE	281
#define	ELLIPSE	282
#define	RHOMBE	283
#define	BITMAP	284
#define	PIXMAP	285

#line 14 "ginger.y"

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


#line 40 "ginger.y"
typedef union
{
  char	*a_string;
  int   an_int;
} YYSTYPE;

#ifndef YYLTYPE
typedef
  struct yyltype
    {
      int timestamp;
      int first_line;
      int first_column;
      int last_line;
      int last_column;
      char *text;
   }
  yyltype;

#define YYLTYPE yyltype
#endif

#include <stdio.h>

#ifndef __cplusplus
#ifndef __STDC__
#define const
#endif
#endif



#define	YYFINAL		66
#define	YYFLAG		-32768
#define	YYNTBASE	33

#define YYTRANSLATE(x) ((unsigned)(x) <= 285 ? yytranslate[x] : 47)

static const char yytranslate[] = {     0,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,    31,
    32,     2,     2,     2,     2,     2,     2,     2,     2,     2,
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
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     1,     2,     3,     4,     5,
     6,     7,     8,     9,    10,    11,    12,    13,    14,    15,
    16,    17,    18,    19,    20,    21,    22,    23,    24,    25,
    26,    27,    28,    29,    30
};

#if YYDEBUG != 0
static const short yyprhs[] = {     0,
     0,     1,     4,     7,     8,    10,    12,    14,    20,    26,
    32,    35,    36,    39,    42,    45,    48,    51,    54,    57,
    60,    63,    66,    69,    72,    75,    78,    81,    84,    86,
    88,    90,    92,    94,    96,    98,   100,   102,   104
};

static const short yyrhs[] = {    -1,
    34,    35,     0,    35,    36,     0,     0,    37,     0,    38,
     0,    39,     0,    31,     3,    42,    40,    32,     0,    31,
     4,    42,    40,    32,     0,    31,     5,    42,    40,    32,
     0,    40,    41,     0,     0,     6,    43,     0,     7,    45,
     0,     8,    43,     0,     9,    43,     0,    10,    43,     0,
    21,    43,     0,    15,    44,     0,    16,    44,     0,    17,
    44,     0,    18,    44,     0,    29,    43,     0,    30,    43,
     0,     5,    42,     0,    11,    42,     0,    12,    42,     0,
    20,    46,     0,    13,     0,    14,     0,    19,     0,    26,
     0,    28,     0,    27,     0,    25,     0,    22,     0,    23,
     0,    24,     0,    25,     0
};

#endif

#if YYDEBUG != 0
static const short yyrline[] = { 0,
    64,    71,    97,    98,   101,   102,   103,   106,   112,   118,
   124,   125,   128,   130,   132,   134,   136,   138,   140,   142,
   144,   146,   148,   150,   152,   154,   156,   158,   162,   165,
   168,   171,   172,   173,   174,   177,   178,   179,   180
};

static const char * const yytname[] = {   "$","error","$illegal.","NODE","EDGE",
"TEMPLATE","LABEL","SHAPE","CONTENTS","EDITOR","COLOR","TO","FROM","ID","STRING",
"X","Y","WIDTH","HEIGHT","INT","DIRECTION","BACKGROUND","NORMAL","INVERSE","BOTH",
"NONE","RECTANGLE","ELLIPSE","RHOMBE","BITMAP","PIXMAP","'('","')'","graph",
"@1","objs","obj","node","edge","template","attributes","attr","name","string",
"number","shape","direction",""
};
#endif

static const short yyr1[] = {     0,
    34,    33,    35,    35,    36,    36,    36,    37,    38,    39,
    40,    40,    41,    41,    41,    41,    41,    41,    41,    41,
    41,    41,    41,    41,    41,    41,    41,    41,    42,    43,
    44,    45,    45,    45,    45,    46,    46,    46,    46
};

static const short yyr2[] = {     0,
     0,     2,     2,     0,     1,     1,     1,     5,     5,     5,
     2,     0,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     1,     1,
     1,     1,     1,     1,     1,     1,     1,     1,     1
};

static const short yydefact[] = {     1,
     4,     2,     0,     3,     5,     6,     7,     0,     0,     0,
    29,    12,    12,    12,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     8,    11,     9,    10,    25,    30,    13,
    35,    32,    34,    33,    14,    15,    16,    17,    26,    27,
    31,    19,    20,    21,    22,    36,    37,    38,    39,    28,
    18,    23,    24,     0,     0,     0
};

static const short yydefgoto[] = {    64,
     1,     2,     4,     5,     6,     7,    15,    35,    12,    40,
    52,    45,    60
};

static const short yypact[] = {-32768,
-32768,   -23,    18,-32768,-32768,-32768,-32768,    -4,    -4,    -4,
-32768,-32768,-32768,-32768,    -5,    23,    51,    -4,     0,    -8,
     0,     0,     0,    -4,    -4,     7,     7,     7,     7,    24,
     0,     0,     0,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
-32768,-32768,-32768,    36,    42,-32768
};

static const short yypgoto[] = {-32768,
-32768,-32768,-32768,-32768,-32768,-32768,    37,-32768,    64,    54,
    63,-32768,-32768
};


#define	YYLAST		92


static const short yytable[] = {    18,
    19,    20,    21,    22,    23,    24,    25,     3,    11,    26,
    27,    28,    29,    39,    30,    31,    41,    42,    43,    44,
     8,     9,    10,    32,    33,    51,    34,    18,    19,    20,
    21,    22,    23,    24,    25,    65,     0,    26,    27,    28,
    29,    66,    30,    31,     0,    56,    57,    58,    59,    16,
    17,    32,    33,     0,    36,    18,    19,    20,    21,    22,
    23,    24,    25,     0,     0,    26,    27,    28,    29,     0,
    30,    31,    13,    14,    46,    47,    48,     0,     0,    32,
    33,    38,    37,     0,    61,    62,    63,    49,    50,    53,
    54,    55
};

static const short yycheck[] = {     5,
     6,     7,     8,     9,    10,    11,    12,    31,    13,    15,
    16,    17,    18,    14,    20,    21,    25,    26,    27,    28,
     3,     4,     5,    29,    30,    19,    32,     5,     6,     7,
     8,     9,    10,    11,    12,     0,    -1,    15,    16,    17,
    18,     0,    20,    21,    -1,    22,    23,    24,    25,    13,
    14,    29,    30,    -1,    32,     5,     6,     7,     8,     9,
    10,    11,    12,    -1,    -1,    15,    16,    17,    18,    -1,
    20,    21,     9,    10,    21,    22,    23,    -1,    -1,    29,
    30,    18,    32,    -1,    31,    32,    33,    24,    25,    27,
    28,    29
};
/* -*-C-*-  Note some compilers choke on comments on `#line' lines.  */
#line 3 "/pack/bison-1.21/lib/bison.simple"

/* Skeleton output parser for bison,
   Copyright (C) 1984, 1989, 1990 Bob Corbett and Richard Stallman

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 1, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.  */


#ifndef alloca
#ifdef __GNUC__
#define alloca __builtin_alloca
#else /* not GNU C.  */
#if (!defined (__STDC__) && defined (sparc)) || defined (__sparc__) || defined (__sparc) || defined (__sgi)
#include <alloca.h>
#else /* not sparc */
#if defined (MSDOS) && !defined (__TURBOC__)
#include <malloc.h>
#else /* not MSDOS, or __TURBOC__ */
#if defined(_AIX)
#include <malloc.h>
 #pragma alloca
#else /* not MSDOS, __TURBOC__, or _AIX */
#ifdef __hpux
#ifdef __cplusplus
extern "C" {
void *alloca (unsigned int);
};
#else /* not __cplusplus */
void *alloca (unsigned int);
#endif /* not __cplusplus */
#endif /* __hpux */
#endif /* not _AIX */
#endif /* not MSDOS, or __TURBOC__ */
#endif /* not sparc.  */
#endif /* not GNU C.  */
#endif /* alloca not defined.  */

/* This is the parser code that is written into each bison parser
  when the %semantic_parser declaration is not specified in the grammar.
  It was written by Richard Stallman by simplifying the hairy parser
  used when %semantic_parser is specified.  */

/* Note: there must be only one dollar sign in this file.
   It is replaced by the list of actions, each action
   as one case of the switch.  */

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		-2
#define YYEOF		0
#define YYACCEPT	return(0)
#define YYABORT 	return(1)
#define YYERROR		goto yyerrlab1
/* Like YYERROR except do call yyerror.
   This remains here temporarily to ease the
   transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  */
#define YYFAIL		goto yyerrlab
#define YYRECOVERING()  (!!yyerrstatus)
#define YYBACKUP(token, value) \
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    { yychar = (token), yylval = (value);			\
      yychar1 = YYTRANSLATE (yychar);				\
      YYPOPSTACK;						\
      goto yybackup;						\
    }								\
  else								\
    { yyerror ("syntax error: cannot back up"); YYERROR; }	\
while (0)

#define YYTERROR	1
#define YYERRCODE	256

#ifndef YYPURE
#define YYLEX		yylex()
#endif

#ifdef YYPURE
#ifdef YYLSP_NEEDED
#define YYLEX		yylex(&yylval, &yylloc)
#else
#define YYLEX		yylex(&yylval)
#endif
#endif

/* If nonreentrant, generate the variables here */

#ifndef YYPURE

int	yychar;			/*  the lookahead symbol		*/
YYSTYPE	yylval;			/*  the semantic value of the		*/
				/*  lookahead symbol			*/

#ifdef YYLSP_NEEDED
YYLTYPE yylloc;			/*  location data for the lookahead	*/
				/*  symbol				*/
#endif

int yynerrs;			/*  number of parse errors so far       */
#endif  /* not YYPURE */

#if YYDEBUG != 0
int yydebug;			/*  nonzero means print parse trace	*/
/* Since this is uninitialized, it does not stop multiple parsers
   from coexisting.  */
#endif

/*  YYINITDEPTH indicates the initial size of the parser's stacks	*/

#ifndef	YYINITDEPTH
#define YYINITDEPTH 200
#endif

/*  YYMAXDEPTH is the maximum size the stacks can grow to
    (effective only if the built-in stack extension method is used).  */

#if YYMAXDEPTH == 0
#undef YYMAXDEPTH
#endif

#ifndef YYMAXDEPTH
#define YYMAXDEPTH 10000
#endif

/* Prevent warning if -Wstrict-prototypes.  */
#ifdef __GNUC__
int yyparse (void);
#endif

#if __GNUC__ > 1		/* GNU C and GNU C++ define this.  */
#define __yy_bcopy(FROM,TO,COUNT)	__builtin_memcpy(TO,FROM,COUNT)
#else				/* not GNU C or C++ */
#ifndef __cplusplus

/* This is the most reliable way to avoid incompatibilities
   in available built-in functions on various systems.  */
static void
__yy_bcopy (from, to, count)
     char *from;
     char *to;
     int count;
{
  register char *f = from;
  register char *t = to;
  register int i = count;

  while (i-- > 0)
    *t++ = *f++;
}

#else /* __cplusplus */

/* This is the most reliable way to avoid incompatibilities
   in available built-in functions on various systems.  */
static void
__yy_bcopy (char *from, char *to, int count)
{
  register char *f = from;
  register char *t = to;
  register int i = count;

  while (i-- > 0)
    *t++ = *f++;
}

#endif
#endif

#line 184 "/pack/bison-1.21/lib/bison.simple"
int
yyparse()
{
  register int yystate;
  register int yyn;
  register short *yyssp;
  register YYSTYPE *yyvsp;
  int yyerrstatus;	/*  number of tokens to shift before error messages enabled */
  int yychar1;		/*  lookahead token as an internal (translated) token number */

  short	yyssa[YYINITDEPTH];	/*  the state stack			*/
  YYSTYPE yyvsa[YYINITDEPTH];	/*  the semantic value stack		*/

  short *yyss = yyssa;		/*  refer to the stacks thru separate pointers */
  YYSTYPE *yyvs = yyvsa;	/*  to allow yyoverflow to reallocate them elsewhere */

#ifdef YYLSP_NEEDED
  YYLTYPE yylsa[YYINITDEPTH];	/*  the location stack			*/
  YYLTYPE *yyls = yylsa;
  YYLTYPE *yylsp;

#define YYPOPSTACK   (yyvsp--, yyssp--, yylsp--)
#else
#define YYPOPSTACK   (yyvsp--, yyssp--)
#endif

  int yystacksize = YYINITDEPTH;

#ifdef YYPURE
  int yychar;
  YYSTYPE yylval;
  int yynerrs;
#ifdef YYLSP_NEEDED
  YYLTYPE yylloc;
#endif
#endif

  YYSTYPE yyval;		/*  the variable used to return		*/
				/*  semantic values from the action	*/
				/*  routines				*/

  int yylen;

#if YYDEBUG != 0
  if (yydebug)
    fprintf(stderr, "Starting parse\n");
#endif

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY;		/* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */

  yyssp = yyss - 1;
  yyvsp = yyvs;
#ifdef YYLSP_NEEDED
  yylsp = yyls;
#endif

/* Push a new state, which is found in  yystate  .  */
/* In all cases, when you get here, the value and location stacks
   have just been pushed. so pushing a state here evens the stacks.  */
yynewstate:

  *++yyssp = yystate;

  if (yyssp >= yyss + yystacksize - 1)
    {
      /* Give user a chance to reallocate the stack */
      /* Use copies of these so that the &'s don't force the real ones into memory. */
      YYSTYPE *yyvs1 = yyvs;
      short *yyss1 = yyss;
#ifdef YYLSP_NEEDED
      YYLTYPE *yyls1 = yyls;
#endif

      /* Get the current used size of the three stacks, in elements.  */
      int size = yyssp - yyss + 1;

#ifdef yyoverflow
      /* Each stack pointer address is followed by the size of
	 the data in use in that stack, in bytes.  */
      yyoverflow("parser stack overflow",
		 &yyss1, size * sizeof (*yyssp),
		 &yyvs1, size * sizeof (*yyvsp),
#ifdef YYLSP_NEEDED
		 &yyls1, size * sizeof (*yylsp),
#endif
		 &yystacksize);

      yyss = yyss1; yyvs = yyvs1;
#ifdef YYLSP_NEEDED
      yyls = yyls1;
#endif
#else /* no yyoverflow */
      /* Extend the stack our own way.  */
      if (yystacksize >= YYMAXDEPTH)
	{
	  yyerror("parser stack overflow");
	  return 2;
	}
      yystacksize *= 2;
      if (yystacksize > YYMAXDEPTH)
	yystacksize = YYMAXDEPTH;
      yyss = (short *) alloca (yystacksize * sizeof (*yyssp));
      __yy_bcopy ((char *)yyss1, (char *)yyss, size * sizeof (*yyssp));
      yyvs = (YYSTYPE *) alloca (yystacksize * sizeof (*yyvsp));
      __yy_bcopy ((char *)yyvs1, (char *)yyvs, size * sizeof (*yyvsp));
#ifdef YYLSP_NEEDED
      yyls = (YYLTYPE *) alloca (yystacksize * sizeof (*yylsp));
      __yy_bcopy ((char *)yyls1, (char *)yyls, size * sizeof (*yylsp));
#endif
#endif /* no yyoverflow */

      yyssp = yyss + size - 1;
      yyvsp = yyvs + size - 1;
#ifdef YYLSP_NEEDED
      yylsp = yyls + size - 1;
#endif

#if YYDEBUG != 0
      if (yydebug)
	fprintf(stderr, "Stack size increased to %d\n", yystacksize);
#endif

      if (yyssp >= yyss + yystacksize - 1)
	YYABORT;
    }

#if YYDEBUG != 0
  if (yydebug)
    fprintf(stderr, "Entering state %d\n", yystate);
#endif

  goto yybackup;
 yybackup:

/* Do appropriate processing given the current state.  */
/* Read a lookahead token if we need one and don't already have one.  */
/* yyresume: */

  /* First try to decide what to do without reference to lookahead token.  */

  yyn = yypact[yystate];
  if (yyn == YYFLAG)
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* yychar is either YYEMPTY or YYEOF
     or a valid token in external form.  */

  if (yychar == YYEMPTY)
    {
#if YYDEBUG != 0
      if (yydebug)
	fprintf(stderr, "Reading a token: ");
#endif
      yychar = YYLEX;
    }

  /* Convert token to internal form (in yychar1) for indexing tables with */

  if (yychar <= 0)		/* This means end of input. */
    {
      yychar1 = 0;
      yychar = YYEOF;		/* Don't call YYLEX any more */

#if YYDEBUG != 0
      if (yydebug)
	fprintf(stderr, "Now at end of input.\n");
#endif
    }
  else
    {
      yychar1 = YYTRANSLATE(yychar);

#if YYDEBUG != 0
      if (yydebug)
	{
	  fprintf (stderr, "Next token is %d (%s", yychar, yytname[yychar1]);
	  /* Give the individual parser a way to print the precise meaning
	     of a token, for further debugging info.  */
#ifdef YYPRINT
	  YYPRINT (stderr, yychar, yylval);
#endif
	  fprintf (stderr, ")\n");
	}
#endif
    }

  yyn += yychar1;
  if (yyn < 0 || yyn > YYLAST || yycheck[yyn] != yychar1)
    goto yydefault;

  yyn = yytable[yyn];

  /* yyn is what to do for this token type in this state.
     Negative => reduce, -yyn is rule number.
     Positive => shift, yyn is new state.
       New state is final state => don't bother to shift,
       just return success.
     0, or most negative number => error.  */

  if (yyn < 0)
    {
      if (yyn == YYFLAG)
	goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }
  else if (yyn == 0)
    goto yyerrlab;

  if (yyn == YYFINAL)
    YYACCEPT;

  /* Shift the lookahead token.  */

#if YYDEBUG != 0
  if (yydebug)
    fprintf(stderr, "Shifting token %d (%s), ", yychar, yytname[yychar1]);
#endif

  /* Discard the token being shifted unless it is eof.  */
  if (yychar != YYEOF)
    yychar = YYEMPTY;

  *++yyvsp = yylval;
#ifdef YYLSP_NEEDED
  *++yylsp = yylloc;
#endif

  /* count tokens shifted since error; after three, turn off error status.  */
  if (yyerrstatus) yyerrstatus--;

  yystate = yyn;
  goto yynewstate;

/* Do the default action for the current state.  */
yydefault:

  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;

/* Do a reduction.  yyn is the number of a rule to reduce with.  */
yyreduce:
  yylen = yyr2[yyn];
  yyval = yyvsp[1-yylen]; /* implement default value of the action */

#if YYDEBUG != 0
  if (yydebug)
    {
      int i;

      fprintf (stderr, "Reducing via rule %d (line %d), ",
	       yyn, yyrline[yyn]);

      /* Print the symbols being reduced, and their result.  */
      for (i = yyprhs[yyn]; yyrhs[i] > 0; i++)
	fprintf (stderr, "%s ", yytname[yyrhs[i]]);
      fprintf (stderr, " -> %s\n", yytname[yyr1[yyn]]);
    }
#endif


  switch (yyn) {

case 1:
#line 64 "ginger.y"
{ /* initial actions */
	    initTable();            /* initialize symbol table */
	    resetObj(&the_obj);     /* clear the attr. object  */
            line=1;                 /* reset line number count */
	    error_output = stdout;  /* send err.messg. to stdout */
	  ;
    break;}
case 2:
#line 71 "ginger.y"
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
	  ;
    break;}
case 8:
#line 106 "ginger.y"
{ the_obj.name=yyvsp[-2].a_string;
					  the_obj.type=T_NODE;
					  newObj();
					;
    break;}
case 9:
#line 112 "ginger.y"
{ the_obj.name=yyvsp[-2].a_string;
					  the_obj.type=T_EDGE;
					  newObj();
					;
    break;}
case 10:
#line 118 "ginger.y"
{ the_obj.name=yyvsp[-2].a_string;
					     the_obj.type=T_TEMPLATE;
					     newObj();
					   ;
    break;}
case 13:
#line 128 "ginger.y"
{ the_obj.label		= yyvsp[0].a_string; 
				  the_obj.vlabel	= 1; ;
    break;}
case 14:
#line 130 "ginger.y"
{ the_obj.shape		= yyvsp[0].an_int; 
				  the_obj.vshape	= 1; ;
    break;}
case 15:
#line 132 "ginger.y"
{ the_obj.contents	= yyvsp[0].a_string; 
				  the_obj.vcontents	= 1; ;
    break;}
case 16:
#line 134 "ginger.y"
{ the_obj.editor	= yyvsp[0].a_string; 
				  the_obj.veditor	= 1; ;
    break;}
case 17:
#line 136 "ginger.y"
{ the_obj.color		= yyvsp[0].a_string; 
				  the_obj.vcolor	= 1; ;
    break;}
case 18:
#line 138 "ginger.y"
{ the_obj.bg_color	= yyvsp[0].a_string; 
				  the_obj.vbg_color	= 1; ;
    break;}
case 19:
#line 140 "ginger.y"
{ the_obj.x		= yyvsp[0].an_int; 
				  the_obj.vx		= 1; ;
    break;}
case 20:
#line 142 "ginger.y"
{ the_obj.y		= yyvsp[0].an_int; 
				  the_obj.vy		= 1; ;
    break;}
case 21:
#line 144 "ginger.y"
{ the_obj.width		= yyvsp[0].an_int; 
				  the_obj.vwidth	= 1; ;
    break;}
case 22:
#line 146 "ginger.y"
{ the_obj.height	= yyvsp[0].an_int; 
				  the_obj.vheight	= 1; ;
    break;}
case 23:
#line 148 "ginger.y"
{ the_obj.bitmap	= yyvsp[0].a_string;
				  the_obj.vbitmap	= 1; ;
    break;}
case 24:
#line 150 "ginger.y"
{ the_obj.pixmap	= yyvsp[0].a_string;
				  the_obj.vpixmap	= 1; ;
    break;}
case 25:
#line 152 "ginger.y"
{ the_obj.template	= yyvsp[0].a_string; 
				  the_obj.vtemplate	= 1; ;
    break;}
case 26:
#line 154 "ginger.y"
{ the_obj.to		= yyvsp[0].a_string; 
				  the_obj.vto		= 1; ;
    break;}
case 27:
#line 156 "ginger.y"
{ the_obj.from		= yyvsp[0].a_string; 
				  the_obj.vfrom		= 1; ;
    break;}
case 28:
#line 158 "ginger.y"
{ the_obj.direction	= yyvsp[0].an_int; 
				  the_obj.vdirection	= 1; ;
    break;}
case 29:
#line 162 "ginger.y"
{ yyval.a_string = GingerNewString(yytext); ;
    break;}
case 30:
#line 165 "ginger.y"
{ yyval.a_string = makeCleanString(yytext); ;
    break;}
case 31:
#line 168 "ginger.y"
{ yyval.an_int = atoi(yytext); ;
    break;}
case 32:
#line 171 "ginger.y"
{ yyval.an_int = SHAPE_RECTANGLE; ;
    break;}
case 33:
#line 172 "ginger.y"
{ yyval.an_int = SHAPE_RHOMBE;    ;
    break;}
case 34:
#line 173 "ginger.y"
{ yyval.an_int = SHAPE_ELLIPSE;   ;
    break;}
case 35:
#line 174 "ginger.y"
{ yyval.an_int = SHAPE_NONE;	;
    break;}
case 36:
#line 177 "ginger.y"
{ yyval.an_int = DIR_NORMAL;        ;
    break;}
case 37:
#line 178 "ginger.y"
{ yyval.an_int = DIR_INVERSE;       ;
    break;}
case 38:
#line 179 "ginger.y"
{ yyval.an_int = DIR_BIDIRECTIONAL; ;
    break;}
case 39:
#line 180 "ginger.y"
{ yyval.an_int = DIR_NONE;          ;
    break;}
}
   /* the action file gets copied in in place of this dollarsign */
#line 457 "/pack/bison-1.21/lib/bison.simple"

  yyvsp -= yylen;
  yyssp -= yylen;
#ifdef YYLSP_NEEDED
  yylsp -= yylen;
#endif

#if YYDEBUG != 0
  if (yydebug)
    {
      short *ssp1 = yyss - 1;
      fprintf (stderr, "state stack now");
      while (ssp1 != yyssp)
	fprintf (stderr, " %d", *++ssp1);
      fprintf (stderr, "\n");
    }
#endif

  *++yyvsp = yyval;

#ifdef YYLSP_NEEDED
  yylsp++;
  if (yylen == 0)
    {
      yylsp->first_line = yylloc.first_line;
      yylsp->first_column = yylloc.first_column;
      yylsp->last_line = (yylsp-1)->last_line;
      yylsp->last_column = (yylsp-1)->last_column;
      yylsp->text = 0;
    }
  else
    {
      yylsp->last_line = (yylsp+yylen-1)->last_line;
      yylsp->last_column = (yylsp+yylen-1)->last_column;
    }
#endif

  /* Now "shift" the result of the reduction.
     Determine what state that goes to,
     based on the state we popped back to
     and the rule number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTBASE] + *yyssp;
  if (yystate >= 0 && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTBASE];

  goto yynewstate;

yyerrlab:   /* here on detecting error */

  if (! yyerrstatus)
    /* If not already recovering from an error, report this error.  */
    {
      ++yynerrs;

#ifdef YYERROR_VERBOSE
      yyn = yypact[yystate];

      if (yyn > YYFLAG && yyn < YYLAST)
	{
	  int size = 0;
	  char *msg;
	  int x, count;

	  count = 0;
	  /* Start X at -yyn if nec to avoid negative indexes in yycheck.  */
	  for (x = (yyn < 0 ? -yyn : 0);
	       x < (sizeof(yytname) / sizeof(char *)); x++)
	    if (yycheck[x + yyn] == x)
	      size += strlen(yytname[x]) + 15, count++;
	  msg = (char *) malloc(size + 15);
	  if (msg != 0)
	    {
	      strcpy(msg, "parse error");

	      if (count < 5)
		{
		  count = 0;
		  for (x = (yyn < 0 ? -yyn : 0);
		       x < (sizeof(yytname) / sizeof(char *)); x++)
		    if (yycheck[x + yyn] == x)
		      {
			strcat(msg, count == 0 ? ", expecting `" : " or `");
			strcat(msg, yytname[x]);
			strcat(msg, "'");
			count++;
		      }
		}
	      yyerror(msg);
	      free(msg);
	    }
	  else
	    yyerror ("parse error; also virtual memory exceeded");
	}
      else
#endif /* YYERROR_VERBOSE */
	yyerror("parse error");
    }

  goto yyerrlab1;
yyerrlab1:   /* here on error raised explicitly by an action */

  if (yyerrstatus == 3)
    {
      /* if just tried and failed to reuse lookahead token after an error, discard it.  */

      /* return failure if at end of input */
      if (yychar == YYEOF)
	YYABORT;

#if YYDEBUG != 0
      if (yydebug)
	fprintf(stderr, "Discarding token %d (%s).\n", yychar, yytname[yychar1]);
#endif

      yychar = YYEMPTY;
    }

  /* Else will try to reuse lookahead token
     after shifting the error token.  */

  yyerrstatus = 3;		/* Each real token shifted decrements this */

  goto yyerrhandle;

yyerrdefault:  /* current state does not do anything special for the error token. */

#if 0
  /* This is wrong; only states that explicitly want error tokens
     should shift them.  */
  yyn = yydefact[yystate];  /* If its default is to accept any token, ok.  Otherwise pop it.*/
  if (yyn) goto yydefault;
#endif

yyerrpop:   /* pop the current state because it cannot handle the error token */

  if (yyssp == yyss) YYABORT;
  yyvsp--;
  yystate = *--yyssp;
#ifdef YYLSP_NEEDED
  yylsp--;
#endif

#if YYDEBUG != 0
  if (yydebug)
    {
      short *ssp1 = yyss - 1;
      fprintf (stderr, "Error: state stack now");
      while (ssp1 != yyssp)
	fprintf (stderr, " %d", *++ssp1);
      fprintf (stderr, "\n");
    }
#endif

yyerrhandle:

  yyn = yypact[yystate];
  if (yyn == YYFLAG)
    goto yyerrdefault;

  yyn += YYTERROR;
  if (yyn < 0 || yyn > YYLAST || yycheck[yyn] != YYTERROR)
    goto yyerrdefault;

  yyn = yytable[yyn];
  if (yyn < 0)
    {
      if (yyn == YYFLAG)
	goto yyerrpop;
      yyn = -yyn;
      goto yyreduce;
    }
  else if (yyn == 0)
    goto yyerrpop;

  if (yyn == YYFINAL)
    YYACCEPT;

#if YYDEBUG != 0
  if (yydebug)
    fprintf(stderr, "Shifting error token, ");
#endif

  *++yyvsp = yylval;
#ifdef YYLSP_NEEDED
  *++yylsp = yylloc;
#endif

  yystate = yyn;
  goto yynewstate;
}
#line 183 "ginger.y"


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
