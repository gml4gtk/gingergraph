/* $Id: PostScript.c,v 1.8 1996/04/09 07:11:31 lupus Exp $ */

/***************************************************************
 * This file is part of the Ginger project.                    *
 * Created 20-1-1996                                           *
 * Niels C. Larsen, Aalborg University, Denmark                *
 * (lupus@iesd.auc.dk)                                         *
 ***************************************************************/

/* Functions to generate a PostScript(TM) representation of 
   a Ginger graph */

#include <string.h>
#include "PostScript.h"
#include "version.h"
#include "graphBuffer.h"
#include "mathMacros.h"
#include "main.h"
#include "config.h"

/* ---  private declarations  --- */

#define CONTENTS_EXPERIMENT 1

#define PUT(x) fprintf(fd, "%s\n", x)
#define CM(x) ((x)*72.0/2.54)    /* convert centimetres to points */

#define FONT_NAME "Helvetica"    /* label font       */
#define POINT_SIZE 12            /* label font size  */
#define TEXT_Y_OFFSET 2          /* label adjustment */
#define ARROW_HEAD_WIDTH 7
#define ARROW_HEAD_LENGTH 10

#define PAGE_HEIGHT CM(29.0)
#define LEFT_MARGIN CM(1.0)
#define TOP_MARGIN  CM(1.0)
#define PS_CONV_X(x) ((int)((x)+LEFT_MARGIN))
#define PS_CONV_Y(y) ((int)(PAGE_HEIGHT-((y)+TOP_MARGIN)))

/* private function prototypes */
static void PostScriptInitialize(FILE *fd);
static void PostScriptEnd(FILE *fd);
static void PostScriptEllipse(int x, int y, int w, int h, FILE *fd);
static void PostScriptRhomb(int x, int y, int w, int h, FILE *fd);
static void PostScriptRectangle(int x1, int y1, int x2, int y2, FILE *fd);
static void PostScriptArrow(int x1, int y1, int x2, int y2, int dir, FILE *fd);
static void PostScriptText(char *text, FILE *fd);
static void PostScriptDetermineBoundingRectangle(graph *g);
static void PSNode(BYTE *n);
static void PSEdge(BYTE *e);
static void PSEdgeCalcEndpointsPS(BYTE *e);
static void PSEdgeCalcEndpoints(BYTE *e);
static void PostScriptLoadLib(FILE *fd);

static FILE *the_fd;
/* coordinates of bounding rectangle */
static int min_x, min_y, max_x, max_y;

/* ---  public functions  --- */

/* generate a PostScript representation of the graph g; 
   send output to the file descriptor fd */
void PostScriptGraph(graph *g, FILE *fd) {
  the_fd=fd;
  
  /* determine bounding rectangle */
  PostScriptDetermineBoundingRectangle(g);

  /* output PostScript header/initialization code */
  PostScriptInitialize(fd);

  /* re-calculate edge endpoints for better fit with elliptical nodes */
  iterate(PSEdgeCalcEndpointsPS, g->edges);

  /* output PostScript code for the edges */
  iterate(PSEdge, g->edges);

  /* re-calculate edge endpoints using the standard screen scheme */
  iterate(PSEdgeCalcEndpoints, g->edges);

  /* output PostScript code for the nodes */
  iterate(PSNode, g->nodes);

  /* output closing PostScript code */
  PostScriptEnd(fd);
}

/* generate PostScript representation of a node */
void PostScriptNode(node *n, FILE *fd) {
  int halfwid, halfhei;

#if CONTENTS_EXPERIMENT
  if(n->contents) {
    fprintf(fd, "%d %d %d %d %s\n", 
	    PS_CONV_X(n->x),
	    PS_CONV_Y(n->y),
	    n->width,
	    n->height, 
	    n->contents);
    return;
  }
#endif
  
  /* print label if node has one */
  if( strlen(n->label) > 0 ) {
    fprintf(fd, "%d ", PS_CONV_X(n->x) );
    PostScriptText(n->label, fd);
    fprintf(fd, " stringwidth pop 2 div sub %d moveto\n", 
	    PS_CONV_Y(n->y+POINT_SIZE/2-TEXT_Y_OFFSET));
    PostScriptText(n->label, fd);
    fprintf(fd, " show\n");
  }

  /* add border */
  switch(n->shape) {
  case SHAPE_RECTANGLE:
    halfwid=MAX(n->width/2,1);
    halfhei=MAX(n->height/2,1);
    PostScriptRectangle(PS_CONV_X(n->x-halfwid),
			PS_CONV_Y(n->y-halfhei),
			PS_CONV_X(n->x+n->width/2),
			PS_CONV_Y(n->y+n->height/2),
			fd);
    break;
  case SHAPE_ELLIPSE:
    PostScriptEllipse(PS_CONV_X(n->x), PS_CONV_Y(n->y), 
		      n->width, n->height, fd);
    break;
  case SHAPE_RHOMBE:
    PostScriptRhomb(PS_CONV_X(n->x), PS_CONV_Y(n->y), 
		    n->width, n->height, fd);
    break;
  }
}

/* generate PostScript representation of an edge */
void PostScriptEdge(edge *e, FILE *fd) {
  PostScriptArrow(PS_CONV_X(e->p1.x), PS_CONV_Y(e->p1.y), 
		  PS_CONV_X(e->p2.x), PS_CONV_Y(e->p2.y), 
		  e->direction, fd);
}

/* ---  private functions  --- */

/* min_x, min_y, max_x, and max_y should be initialized before this
   function is called */
static void PostScriptInitialize(FILE *fd) {

  /* output PostScript header */
  fprintf(fd, "%%!PS-Adobe-3.0\n");
  fprintf(fd, "%%%%Creator: %s\n", versionGetAsString() );
  fprintf(fd, "%%%%Title: %s\n", graphBufferGetCurrentFileName() );
  fprintf(fd, "%%%%BoundingBox: %d %d %d %d\n", min_x, min_y, max_x, max_y);
  fprintf(fd, "%%%%DocumentData: Clean8Bit\n");
  fprintf(fd, "%%%%Pages: 1\n");
  fprintf(fd, "%%%%EndComments\n");
  fprintf(fd, "%%%%BeginSetup\n");
  
  /* define centimetre (cm) unit */
  PUT("/cm {72 2.54 div mul} def\n");
  
  /* define font */
  fprintf(fd, "/%s findfont\n", FONT_NAME);
  PUT("  dup length dict begin ");
  PUT("  {1 index /FID ne {def}{pop pop} ifelse} forall ");
  PUT("  /Encoding ISOLatin1Encoding def ");
  PUT("  currentdict ");
  PUT("  end ");
  fprintf(fd, "/%s-ISO exch definefont pop ", FONT_NAME);

  fprintf(fd, "/%s-ISO findfont %d scalefont setfont\n\n", FONT_NAME, POINT_SIZE);

  /* define a procedure for drawing a rectangle */
  PUT("/rectangle {   % x1 y1 x2 y2 rectangle -> ");
  PUT("/y2 exch def");
  PUT("/x2 exch def");
  PUT("/y1 exch def");
  PUT("/x1 exch def");
  PUT("x1 y1 moveto");
  PUT("x2 y1 lineto");
  PUT("x2 y2 lineto");
  PUT("x1 y2 lineto");
  PUT("closepath");
  PUT("} def\n");
  
  /* define a procedure for drawing a rhomb */
  PUT("/rhomb {   % x1 y1 halfwidth halfheight rhomb -> ");
  PUT("/halfheight exch def");
  PUT("/halfwidth  exch def");
  PUT("/y1 exch def");
  PUT("/x1 exch def");
  PUT("x1 halfwidth sub y1 moveto");
  PUT("x1 y1 halfheight add lineto");
  PUT("x1 halfwidth add y1 lineto");
  PUT("x1 y1 halfheight sub  lineto");
  PUT("closepath");
  PUT("} def\n");
  
  /* define a procedure for drawing an ellipse */
  PUT("/ellipsedict 8 dict def");
  PUT("ellipsedict /mtrx matrix put");
  PUT("");
  PUT("/ellipse  % center_x center_y rad_x rad_y start_ang end_ang ellipse -> ");
  PUT(" { ellipsedict begin");
  PUT("    /endangle exch def");
  PUT("    /startangle exch def");
  PUT("    /yrad exch def");
  PUT("    /xrad exch def");
  PUT("    /y exch def");
  PUT("    /x exch def");
  PUT("    /savematrix mtrx currentmatrix def");
  PUT("    x y translate");
  PUT("    xrad yrad scale");
  PUT("    0 0 1 startangle endangle arc");
  PUT("    savematrix setmatrix");
  PUT("  end");
  PUT(" } def\n");

  /* define a procedure for drawing an arrow head */
  PUT("/arrowdict 14 dict def");
  PUT("arrowdict begin");
  PUT(" /mtrx matrix def");
  PUT("end");
  PUT("");
  PUT("/arrow  % tailx taily tipx tipy headwidth headlength arrow -> ");
  PUT(" { arrowdict begin");
  PUT("    /headlength exch def");
  PUT("    /halfheadthickness exch 2 div def");
  PUT("    /tipy exch def");
  PUT("    /tipx exch def");
  PUT("    /taily exch def");
  PUT("    /tailx exch def");
  PUT("   ");
  PUT("    /dx tipx tailx sub def");
  PUT("    /dy tipy taily sub def");
  PUT("");
  PUT("    /arrowlength dx dx mul dy dy mul add sqrt def");
  PUT("    /angle dy dx atan def");
  PUT("");
  PUT("    /base arrowlength headlength sub def");
  PUT("");
  PUT("    /savematrix mtrx currentmatrix def");
  PUT("");
  PUT("    tailx taily translate");
  PUT("    angle rotate");
  PUT("");
  PUT("    newpath");
  PUT("    0 0 moveto");
  PUT("    base 0 lineto");
  PUT("    stroke");
  PUT("");
  PUT("    newpath");
  PUT("    base halfheadthickness moveto");
  PUT("    arrowlength 0 lineto");
  PUT("    base halfheadthickness neg lineto");
  PUT("    closepath");
  PUT("    fill");
  PUT("");
  PUT("    savematrix setmatrix");
  PUT("   end");
  PUT(" } def\n");

  PostScriptLoadLib(fd);

  fprintf(fd, "%%%%EndSetup\n");
  fprintf(fd, "%%%%Page: 1 1\n\n");
}

static void PostScriptEnd(FILE *fd) {
  PUT("showpage");
  fprintf(fd, "%%%%EOF\n\n");
}

static void PostScriptLoadLib(FILE *fd) {
  FILE *lf;
  char buf[1024];
  char *p;

  expandPathOfSupportFile(PS_LIB_FILE, buf, 1024);
  
  if( (lf=fopen(buf, "r"))==NULL )
    return;
  
  while(!feof(lf)) {
    p=fgets(buf, 1024, lf);
    if(p)
      fprintf(fd, "%s", buf);
  }
  fprintf(fd, "\n\n");
  fclose(lf);
}

static void PostScriptEllipse(int x, int y, int w, int h, FILE *fd) {
  PUT("newpath");
  fprintf(fd, "%d %d %d %d 0 360 ellipse\n", x, y, w/2, h/2);
  PUT("stroke");
}

static void PostScriptRhomb(int x, int y, int w, int h, FILE *fd) {
  PUT("newpath");
  fprintf(fd, "%d %d %d %d rhomb\n", x, y, w/2, h/2);
  PUT("stroke");
}

static void PostScriptRectangle(int x1, int y1, int x2, int y2, FILE *fd) {
  PUT("newpath");
  fprintf(fd, "%d %d %d %d rectangle\n", x1, y1, x2, y2);
  PUT("stroke");
}

static void PostScriptText(char *text, FILE *fd) {
  int i;
  char ch;
  
  fprintf(fd, "(");
  for(i=0; ch=*(text+i); i++) {  /* escape special characters in the 
				    text to avoid confusing the
				    PostScript interpreter */
    switch(ch) {
    case '(':
      fprintf(fd, "\\050");
      break;
    case ')':
      fprintf(fd, "\\051");
      break;
    default:
      fprintf(fd, "%c", ch);
    }
  }
  fprintf(fd, ")");
}

static void PostScriptArrow(int x1, int y1, int x2, int y2, int dir, FILE *fd) {

  if( x1==x2 && y1==y2 )   /* avoid trouble with zero-length arrows */
    return;
  
  switch(dir) {
  case DIR_NONE:
    /* print arrow as a line */
    PUT("newpath");
    fprintf(fd, "%d %d moveto\n", x1, y1);
    fprintf(fd, "%d %d lineto\n", x2, y2);
    PUT("stroke");
    break;
  case DIR_NORMAL:
    fprintf(fd, "%d %d %d %d %d %d arrow\n", x1, y1, x2, y2, 
	    ARROW_HEAD_WIDTH, ARROW_HEAD_LENGTH );
    break;
  case DIR_INVERSE:
    fprintf(fd, "%d %d %d %d %d %d arrow\n", x2, y2, x1, y1, 
	    ARROW_HEAD_WIDTH, ARROW_HEAD_LENGTH );
    break;
  case DIR_BIDIRECTIONAL:
    fprintf(fd, "%d %d %d %d %d %d arrow\n", x1, y1, x2, y2, 
	    ARROW_HEAD_WIDTH, ARROW_HEAD_LENGTH );
    fprintf(fd, "%d %d %d %d %d %d arrow\n", x2, y2, x1, y1, 
	    ARROW_HEAD_WIDTH, ARROW_HEAD_LENGTH );
    break;
  }
}

static void PSNode(BYTE *n) {
  PostScriptNode((node *)n, the_fd);
}

static void PSEdge(BYTE *e) {
  PostScriptEdge((edge *)e, the_fd);
}

static void PSEdgeCalcEndpointsPS(BYTE *e) {
  calculateEdgeEndpointsElliptical((edge *)e);
}

static void PSEdgeCalcEndpoints(BYTE *e) {
  calculateEdgeEndpoints((edge *)e);
}

static void PSNodeBoundingRectangle(BYTE *n1) {
  node *n;
  n=(node *)n1;
  
  min_x=MIN(min_x, PS_CONV_X(n->x-n->width/2-NODE_MARGIN));
  max_x=MAX(max_x, PS_CONV_X(n->x+n->width/2+NODE_MARGIN));

  /* the y-calculation looks strange because the PS_CONV_Y
     inverts up and down! */
  min_y=MIN(min_y, PS_CONV_Y(n->y+n->height/2+NODE_MARGIN));
  max_y=MAX(max_y, PS_CONV_Y(n->y-n->height/2-NODE_MARGIN));
}

static void PostScriptDetermineBoundingRectangle(graph *g) {
  min_x=PAGE_HEIGHT;
  max_x=0;
  min_y=PAGE_HEIGHT;
  max_y=0;

  iterate(PSNodeBoundingRectangle, g->nodes);
}

