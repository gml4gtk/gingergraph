# gingergraph
graph layout in C with sugiyama barycenter layout

Ginger -- an INteractive Graph EditoR for the X Window System.  

This is the original GNU GPL Free C source of the ginger graph visualization tool.  
This has a matrix based sugiyama barycenter layout implementation in C for automatic graph layout.  

The original source is in directory ginger-0.17  
The source to turn it into a graph library is in directory libginger  

In the file graphDisplay.c is the graphical interface  
In the file PostScript.c is the source to generate postscript output  

The ggl graph language implemented look like this and can have graphical nodes as xpm images:  

(node A  
  label "Center"  
  shape ellipse  
  x 200  y 100  
  width 60  height 60  
)  
(node B  
  label "A test node"  
  color "black"  
  bg_color "gray80"  
  x 150  y 25  
  width 75  height 20  
)  
(node C  
  label "A test node"  
  color "red"  
  bg_color "misty rose"  
  shape rhomb  
  x 343  y 100  
  width 100  height 50  
)  
(node D  
  label "Red rose"  
  color "red"  
  bg_color "misty rose"  
  pixmap "Icons/redrose.xpm"  
  x 269  y 199  
  width 66  height 58  
)  
(node E  
  label "A Gnu"  
  bg_color "lightblue"  
  bitmap "Icons/gnu.h"  
  x 125  y 200  
  width 51  height 68  
)  
(edge ab  
  from B to A  
)  
(edge ac  
  from C to A  
)  
(edge ad  
  from D to A  
)  
(edge ae  
  from E to A  
)  
(edge _edge0  
  from E to B  
)  
  

