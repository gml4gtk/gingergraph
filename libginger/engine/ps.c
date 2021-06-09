/*
Checksum: 2686974521      (ps.c)
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

#include <stdio.h>
#include <string.h>

#include "gingerprotos.h"

#define CM(x) ((x)*72.0/2.54)
#define FONT_NAME "Helvetica"
#define POINT_SIZE 12
#define TEXT_Y_OFFSET 2
#define ARROW_HEAD_WIDTH 7
#define ARROW_HEAD_LENGTH 10
#define PAGE_HEIGHT CM(29.0)
#define LEFT_MARGIN CM(1.0)
#define TOP_MARGIN  CM(1.0)
#define PS_CONV_X(x) ((int)((x)+LEFT_MARGIN))
#define PS_CONV_Y(y) ((int)(PAGE_HEIGHT-((y)+TOP_MARGIN)))

#define NODE_MARGIN 3

/* */
#undef MIN
#define MIN(x,y)    ( ((x)<(y)) ? (x):(y) )

/* */
#undef MAX
#define MAX(x,y)    ( ((x)>(y)) ? (x):(y) )

/* */
FILE *the_fd = (FILE *) 0;

/* */
int min_x = 0;
int min_y = 0;
int max_x = 0;
int max_y = 0;

/* */
void
postscriptgraph (graph * g, FILE * fd)
{

  /* */
  the_fd = fd;

  /* determine bounding rectangle */
  postscriptdetermineboundingrectangle (g);

  /* output PostScript header/initialization code */
  postscriptinitialize (fd);

  /* re-calculate edge endpoints for better fit with elliptical nodes */
  iterate (psedgecalcendpointsps, g->edges);

  /* output PostScript code for the edges */
  iterate (psedge, g->edges);

  /* re-calculate edge endpoints using the standard screen scheme */
  iterate (psedgecalcendpoints, g->edges);

  /* output PostScript code for the nodes */
  iterate (psnode, g->nodes);

  /* output closing PostScript code */
  postscriptend (fd);

  return;
}

/* */
void
postscriptedge (edge * e, FILE * fd)
{

  fprintf (fd, "%%%%%%%% %s: %s(): edge from node `%s' to node `%s' \n",
	   __FILE__, __FUNCTION__, e->from->name, e->to->name);

  postscriptarrow (PS_CONV_X (e->p1.x), PS_CONV_Y (e->p1.y),
		   PS_CONV_X (e->p2.x), PS_CONV_Y (e->p2.y), e->direction,
		   fd);

  return;
}

/* */
void
postscriptinitialize (FILE * fd)
{

  fprintf (fd, "%%%%%%%% %s: %s(): \n", __FILE__, __FUNCTION__);

  /* */
  fprintf (fd, "%%!PS-Adobe-3.0\n");
  fprintf (fd, "%%%%Creator: %s\n", "Linux");
  fprintf (fd, "%%%%Title: %s\n", "WorldDomination");
  fprintf (fd, "%%%%BoundingBox: %d %d %d %d\n", min_x, min_y, max_x, max_y);
  fprintf (fd, "%%%%DocumentData: Clean8Bit\n");
  fprintf (fd, "%%%%Pages: 1\n");
  fprintf (fd, "%%%%EndComments\n");
  fprintf (fd, "%%%%BeginSetup\n");

  /* */
  fprintf (fd, "%s\n", "/cm {72 2.54 div mul} def\n");

  /* */
  fprintf (fd, "/%s findfont\n", FONT_NAME);
  fprintf (fd, "%s\n", "  dup length dict begin ");
  fprintf (fd, "%s\n", "  {1 index /FID ne {def}{pop pop} ifelse} forall ");
  fprintf (fd, "%s\n", "  /Encoding ISOLatin1Encoding def ");
  fprintf (fd, "%s\n", "  currentdict ");
  fprintf (fd, "%s\n", "  end ");
  fprintf (fd, "/%s-ISO exch definefont pop ", FONT_NAME);

  fprintf (fd, "/%s-ISO findfont %d scalefont setfont\n\n", FONT_NAME,
	   POINT_SIZE);

  /* define a procedure for drawing a rectangle */
  fprintf (fd, "%s\n", "/rectangle {   % x1 y1 x2 y2 rectangle -> ");
  fprintf (fd, "%s\n", "/y2 exch def");
  fprintf (fd, "%s\n", "/x2 exch def");
  fprintf (fd, "%s\n", "/y1 exch def");
  fprintf (fd, "%s\n", "/x1 exch def");
  fprintf (fd, "%s\n", "x1 y1 moveto");
  fprintf (fd, "%s\n", "x2 y1 lineto");
  fprintf (fd, "%s\n", "x2 y2 lineto");
  fprintf (fd, "%s\n", "x1 y2 lineto");
  fprintf (fd, "%s\n", "closepath");
  fprintf (fd, "%s\n", "} def\n");

  /*
   *
   *    ---- 
   *  /      \
   * |        |
   *  \      /
   *    ----
   */

  /* define a procedure for drawing a rhomb */
  fprintf (fd, "%s\n", "/rhomb {   % x1 y1 halfwidth halfheight rhomb -> ");
  fprintf (fd, "%s\n", "/halfheight exch def");
  fprintf (fd, "%s\n", "/halfwidth  exch def");
  fprintf (fd, "%s\n", "/y1 exch def");
  fprintf (fd, "%s\n", "/x1 exch def");
  fprintf (fd, "%s\n", "x1 halfwidth sub y1 moveto");
  fprintf (fd, "%s\n", "x1 y1 halfheight add lineto");
  fprintf (fd, "%s\n", "x1 halfwidth add y1 lineto");
  fprintf (fd, "%s\n", "x1 y1 halfheight sub  lineto");
  fprintf (fd, "%s\n", "closepath");
  fprintf (fd, "%s\n", "} def\n");

  /* define a procedure for drawing an ellipse */
  fprintf (fd, "%s\n", "/ellipsedict 8 dict def");
  fprintf (fd, "%s\n", "ellipsedict /mtrx matrix put");
  fprintf (fd, "%s\n", "");
  fprintf (fd, "%s\n",
	   "/ellipse  % center_x center_y rad_x rad_y start_ang end_ang ellipse -> ");
  fprintf (fd, "%s\n", " { ellipsedict begin");
  fprintf (fd, "%s\n", "    /endangle exch def");
  fprintf (fd, "%s\n", "    /startangle exch def");
  fprintf (fd, "%s\n", "    /yrad exch def");
  fprintf (fd, "%s\n", "    /xrad exch def");
  fprintf (fd, "%s\n", "    /y exch def");
  fprintf (fd, "%s\n", "    /x exch def");
  fprintf (fd, "%s\n", "    /savematrix mtrx currentmatrix def");
  fprintf (fd, "%s\n", "    x y translate");
  fprintf (fd, "%s\n", "    xrad yrad scale");
  fprintf (fd, "%s\n", "    0 0 1 startangle endangle arc");
  fprintf (fd, "%s\n", "    savematrix setmatrix");
  fprintf (fd, "%s\n", "  end");
  fprintf (fd, "%s\n", " } def\n");

  /* define a procedure for drawing an arrow head */
  fprintf (fd, "%s\n", "/arrowdict 14 dict def");
  fprintf (fd, "%s\n", "arrowdict begin");
  fprintf (fd, "%s\n", " /mtrx matrix def");
  fprintf (fd, "%s\n", "end");
  fprintf (fd, "%s\n", "");
  fprintf (fd, "%s\n",
	   "/arrow  % tailx taily tipx tipy headwidth headlength arrow -> ");
  fprintf (fd, "%s\n", " { arrowdict begin");
  fprintf (fd, "%s\n", "    /headlength exch def");
  fprintf (fd, "%s\n", "    /halfheadthickness exch 2 div def");
  fprintf (fd, "%s\n", "    /tipy exch def");
  fprintf (fd, "%s\n", "    /tipx exch def");
  fprintf (fd, "%s\n", "    /taily exch def");
  fprintf (fd, "%s\n", "    /tailx exch def");
  fprintf (fd, "%s\n", "   ");
  fprintf (fd, "%s\n", "    /dx tipx tailx sub def");
  fprintf (fd, "%s\n", "    /dy tipy taily sub def");
  fprintf (fd, "%s\n", "");
  fprintf (fd, "%s\n", "    /arrowlength dx dx mul dy dy mul add sqrt def");
  fprintf (fd, "%s\n", "    /angle dy dx atan def");
  fprintf (fd, "%s\n", "");
  fprintf (fd, "%s\n", "    /base arrowlength headlength sub def");
  fprintf (fd, "%s\n", "");
  fprintf (fd, "%s\n", "    /savematrix mtrx currentmatrix def");
  fprintf (fd, "%s\n", "");
  fprintf (fd, "%s\n", "    tailx taily translate");
  fprintf (fd, "%s\n", "    angle rotate");
  fprintf (fd, "%s\n", "");
  fprintf (fd, "%s\n", "    newpath");
  fprintf (fd, "%s\n", "    0 0 moveto");
  fprintf (fd, "%s\n", "    base 0 lineto");
  fprintf (fd, "%s\n", "    stroke");
  fprintf (fd, "%s\n", "");
  fprintf (fd, "%s\n", "    newpath");
  fprintf (fd, "%s\n", "    base halfheadthickness moveto");
  fprintf (fd, "%s\n", "    arrowlength 0 lineto");
  fprintf (fd, "%s\n", "    base halfheadthickness neg lineto");
  fprintf (fd, "%s\n", "    closepath");
  fprintf (fd, "%s\n", "    fill");
  fprintf (fd, "%s\n", "");
  fprintf (fd, "%s\n", "    savematrix setmatrix");
  fprintf (fd, "%s\n", "   end");
  fprintf (fd, "%s\n", " } def\n");

  fprintf (fd, "%%%%EndSetup\n");
  fprintf (fd, "%%%%Page: 1 1\n\n");

  fprintf (fd, "%%%%%%%% %s: %s(end)\n", __FILE__, __FUNCTION__);

  return;
}

/* */
void
postscriptend (FILE * fd)
{
  fprintf (fd, "%%%%%%%% %s: %s(): \n", __FILE__, __FUNCTION__);
  fprintf (fd, "%s\n", "showpage");
  fprintf (fd, "%%%%EOF\n\n");
  fprintf (fd, "%%%%%%%% %s: %s(): \n", __FILE__, __FUNCTION__);
  return;
}

/* */
void
postscriptnode (node * n, FILE * fd)
{
  int halfwid = 0;
  int halfhei = 0;

  fprintf (fd, "%%%%%%%% %s: %s(): node `%s' \n", __FILE__, __FUNCTION__,
	   n->name);

  /* print label if node has one
   * if (strlen (n->label) > 0)
   *     {
   *        fprintf (fd, "%d ", PS_CONV_X (n->x));
   *       postscripttext (n->label, fd);
   *       fprintf (fd, " stringwidth pop 2 div sub %d moveto\n",
   *           PS_CONV_Y (n->y + POINT_SIZE / 2 - TEXT_Y_OFFSET));
   *       postscripttext (n->label, fd);
   *       fprintf (fd, " show\n");
   *     }
   */

  /* add border 
   * switch (n->shape)
   * {
   * case SHAPE_RECTANGLE:
   */
  halfwid = MAX (n->width / 2, 1);
  halfhei = MAX (n->height / 2, 1);
  postscriptrectangle (PS_CONV_X (n->x - halfwid),
		       PS_CONV_Y (n->y - halfhei),
		       PS_CONV_X (n->x + n->width / 2),
		       PS_CONV_Y (n->y + n->height / 2), fd);
  /*
   *  break;
   *    case SHAPE_ELLIPSE:
   *      postscriptellipse (PS_CONV_X (n->x), PS_CONV_Y (n->y), n->width,
   *                   n->height, fd);
   *      break;
   *    case SHAPE_RHOMBE:
   *      postscriptrhomb (PS_CONV_X (n->x), PS_CONV_Y (n->y), n->width,
   *                 n->height, fd);
   *      break;
   *    }
   */

  return;
}

/* */
void
psnode (void *n)
{
  postscriptnode ((node *) n, the_fd);
  return;
}

/* */
void
psedge (void *e)
{
  postscriptedge ((edge *) e, the_fd);
  return;
}

/* */
void
psedgecalcendpointsps (void *e)
{
//  calculateedgeendpointselliptical ((edge *) e);
  return;
}

/* */
void
psedgecalcendpoints (void *e)
{
//  calculateedgeendpoints ((edge *) e);
  return;
}


/* */
void
psnodeboundingrectangle (void *n1)
{
  node *n = (node *) 0;

  n = (node *) n1;

  min_x = MIN (min_x, PS_CONV_X (n->x - n->width / 2 - NODE_MARGIN));
  max_x = MAX (max_x, PS_CONV_X (n->x + n->width / 2 + NODE_MARGIN));

  min_y = MIN (min_y, PS_CONV_Y (n->y + n->height / 2 + NODE_MARGIN));
  max_y = MAX (max_y, PS_CONV_Y (n->y - n->height / 2 - NODE_MARGIN));

  return;
}

/* */
void
postscriptdetermineboundingrectangle (graph * g)
{

  /* */
  min_x = PAGE_HEIGHT;
  min_y = PAGE_HEIGHT;

  /* */
  max_x = 0;
  max_y = 0;

  iterate (psnodeboundingrectangle, g->nodes);

  return;
}

/* */
void
postscriptellipse (int x, int y, int w, int h, FILE * fd)
{
  fprintf (fd, "%s\n", "newpath");
  fprintf (fd, "%d %d %d %d 0 360 ellipse\n", x, y, w / 2, h / 2);
  fprintf (fd, "%s\n", "stroke");
  return;
}

/* */
void
postscriptrhomb (int x, int y, int w, int h, FILE * fd)
{
  fprintf (fd, "%s\n", "newpath");
  fprintf (fd, "%d %d %d %d rhomb\n", x, y, w / 2, h / 2);
  fprintf (fd, "%s\n", "stroke");
  return;
}

/* */
void
postscriptrectangle (int x1, int y1, int x2, int y2, FILE * fd)
{
  fprintf (fd, "%s\n", "newpath");
  fprintf (fd, "%d %d %d %d rectangle\n", x1, y1, x2, y2);
  fprintf (fd, "%s\n", "stroke");
  return;
}

/* */
void
postscripttext (char *text, FILE * fd)
{
  int i = 0;
  char ch = 0;

  fprintf (fd, "(");

  for (i = 0; (ch = *(text + i)); i++)
    {

      /* escape special characters in
       * the text to avoid confusing the
       * postScript interpreter 
       */

      switch (ch)
	{
	case '(':
	  fprintf (fd, "\\050");
	  break;
	case ')':
	  fprintf (fd, "\\051");
	  break;
	default:
	  fprintf (fd, "%c", ch);
	  break;
	}
    }

  fprintf (fd, ")");

  return;
}

/* */
void
postscriptarrow (int x1, int y1, int x2, int y2, int dir, FILE * fd)
{

  fprintf (fd,
	   "%%%%%%%% %s: %s(): edge from (%d,%d) to (%d,%d) direction %d \n",
	   __FILE__, __FUNCTION__, x1, y1, x2, y2, dir);

  /* */
  if ((x1 == x2) && (y1 == y2))
    {
      fprintf (fd,
	       "%%%%%%%% %s: %s(): skipped because ((x1==x2) && (y1==y2)) \n",
	       __FILE__, __FUNCTION__);
      return;
    }
  /*
   * switch (dir)
   *    {
   *    case DIR_NONE:
   */
  /* print arrow as a line */
  fprintf (fd, "%s\n", "newpath");
  fprintf (fd, "%d %d moveto\n", x1, y1);
  fprintf (fd, "%d %d lineto\n", x2, y2);
  fprintf (fd, "%s\n", "stroke");
  /*
   *      break;
   *     case DIR_NORMAL:
   *       fprintf (fd, "%d %d %d %d %d %d arrow\n", x1, y1, x2, y2,
   *            ARROW_HEAD_WIDTH, ARROW_HEAD_LENGTH);
   *       break;
   *     case DIR_INVERSE:
   *       fprintf (fd, "%d %d %d %d %d %d arrow\n", x2, y2, x1, y1,
   *            ARROW_HEAD_WIDTH, ARROW_HEAD_LENGTH);
   *       break;
   *     case DIR_BIDIRECTIONAL:
   *       fprintf (fd, "%d %d %d %d %d %d arrow\n", x1, y1, x2, y2,
   *            ARROW_HEAD_WIDTH, ARROW_HEAD_LENGTH);
   *       fprintf (fd, "%d %d %d %d %d %d arrow\n", x2, y2, x1, y1,
   *            ARROW_HEAD_WIDTH, ARROW_HEAD_LENGTH);
   *       break;
   *     default:
   *       break;
   *     }
   */

  return;
}

/* End. */
