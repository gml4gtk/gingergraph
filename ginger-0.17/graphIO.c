/* $Id: graphIO.c,v 1.4 1996/02/26 08:51:43 lupus Exp $ */

/***************************************************************
 * This file is part of the Ginger project.                    *
 * Created 1-1-1996                                            *
 * Niels C. Larsen, Aalborg University, Denmark                *
 * (lupus@iesd.auc.dk)                                         *
 ***************************************************************/

/*  Functions to read/write the GGL representation of a graph
    from/to file  
*/

#include "graphIO.h"
#include "parser.h"

/* ---  external variables  --- */

extern FILE *yyin;     /* in the YACC/Bison generated parser */
extern int yyparse();  /* in the YACC/Bison generated parser */

/* ---  private declarations  --- */

static graph *the_graph;
static FILE *the_fd;
static void gprintNode(BYTE *n);
static void gprintEdge(BYTE *e);

void lexerRestart(FILE *f);  /* in the Flex generated scanner */

/* ---  public functions  --- */

/* output/print the GGL representation of the graph to fd */
void printGraph(graph *g, FILE *fd) {

  the_fd=fd;  /* used by the "iteration" functions */

  /* print the GGL representation of each node */
  iterate(gprintNode, g->nodes);
  /* print the GGL representation of each edge */
  iterate(gprintEdge, g->edges);
}

/* read graph from a GGL representation in the file fd.
   Return a pointer to the loaded graph object, or a NULL pointer,
   if the read failed (e.g. due to a parse error). */
graph *readGraph(FILE *fd) {
  int res;
  
  the_graph = 0;
  yyin = fd;
  
  lexerRestart(fd); /* reset the lexical scanner */
  parserSetMode(PARSER_MODE_NEW, 0);
  res=yyparse();  /* create new graph. yyparse() will return it
	             to us via graphIOReadResult(), unless a
		     parse error occurs. This is indicated by the 
		     return value assigned to "res" */
  /* printf("Status: %d\n", res); */

  if( !res ) /* if no parse error */
    return the_graph;
  else
    return 0;
}

/* read graph from a GGL representation in the file fd.
   Merge the graph into the graph g.
   Return g upon succes, 0 otherwise. 
   g is unchanged if 0 is returned.
*/
graph *readAndMergeGraph(graph *g, FILE *fd) {
  int res;
  
  the_graph = 0;
  yyin = fd;
  
  lexerRestart(fd); /* reset the lexical scanner */
  parserSetMode(PARSER_MODE_MERGE, g);
  res=yyparse(); 
  
  if( !res ) /* if no parse error */
    return g;
  else
    return 0;
}

/* receive the graph read by the parser. This function is used
   for communication between the two as an alternative to 
   a global variable. */
void graphIOReadResult(graph *res) {
  the_graph = res;
}

/* ---  private functions  --- */

/* called by printGraph() */
static void gprintNode(BYTE *n) {
  printNode((node *)n, the_fd);
}

/* called by printGraph() */
static void gprintEdge(BYTE *e) {
  printEdge((edge *)e, the_fd);
}

