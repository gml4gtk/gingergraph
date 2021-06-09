/* $Id: version.c,v 1.13 1996/04/19 07:43:40 lupus Exp $ */

/***************************************************************
 * This file is part of the Ginger project.                    *
 * Created 1-2-1996                                            *
 * Niels C. Larsen, Aalborg University, Denmark                *
 * (lupus@iesd.auc.dk)                                         *
 ***************************************************************/

#include <string.h>
#include <stdio.h>
#include "str.h"
#include "version.h"

/* ---  private declarations  --- */

#define VERSION_MAX 256

static char the_version[VERSION_MAX];

/* ---  public functions  --- */

char *versionGetAsString() {
  sprintf(the_version, "Ginger version 0.17, by Martin Molz & Niels C. Larsen, Aalborg University, Denmark.");
  return the_version;
}

/* ---  Ginger version history  --- */

/* Version#  Date         State of prototype
   ----------------------------------------------------------------------
   0.01      27-dec-1995  Beginning of project. The name "Ginger" 
                          was found for the project. The first crude
                          main window implemented.
   0.02      29-dec-1995  Node and edge drawing implemented. Nodes can
                          be moved; adjacent edges follow the node.
   0.03      31-dec-1995  The beginning of the GGL-parser.
   0.04      07-jan-1996  The file menu and file selector implemented.
                          The GGL-parser is now functional.
   0.05      20-jan-1996  First PostScript code generation.
   0.06      27-jan-1996  Options menu. Hypertext help system.
   0.07      29-jan-1996  Contents/editor mechanism on nodes.
   0.08      30-jan-1996  Node highlight mechanism.
   0.09      01-feb-1996  Scrollbars on the graph area. Message field.
             02-feb-1996  Individual node and edge colors.
   0.10      09-feb-1996  Node and edge selection.
             11-feb-1996  Edge creation, edition, and deletion.
   0.11      14-feb-1996  Layout menu and algorithms (array and circular)
             17-feb-1996  Hierarchical layout algorithm.
   0.12      19-feb-1996  Node creation, edition, and deletion. Hotspot.
   0.13      24-feb-1996  Insert graph. "Rubber-selection".
             25-feb-1996  Updated PostScript code generation. 
   0.14      02-mar-1996  First link to Elk Scheme.
   0.15      22-mar-1996  Improved performance of the layout algorithms.
                          New environment variable, GINGERLIBPATH.
			  First Meroon OO interface to the graph data type.
   0.16      27-mar-1996  Interactively move and select Meroon nodes.
   0.17      19-apr-1996  Better PostScript output.
*/
