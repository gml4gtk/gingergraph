/* $Id: rectangles.c,v 1.3 1996/02/01 15:50:42 lupus Exp $ */

/***************************************************************
 * This file is part of the Ginger project.                    *
 * Created 29-12-1995                                          *
 * Niels C. Larsen, Aalborg University, Denmark                *
 * (lupus@iesd.auc.dk)                                         *
 ***************************************************************/

#include "rectangles.h"
#include "mathMacros.h"

/* is point inside the rectangle with opposite corners rect1
   and rect2 ? */
int pointInRectangle(XPoint point, XPoint rect1, XPoint rect2)
{
  int res; 
  
  /* is point inside rectangle in horizontal direction? */
  res = (point.x>=rect1.x && point.x<=rect2.x) ||
        (point.x>=rect2.x && point.x<=rect1.x);
  
  /* if not, return false */
  if ( !res )
    return res;
  
  /* is point also inside rectangle in vertical direction? */
  res = (point.y>=rect1.y && point.y<=rect2.y) ||
        (point.y>=rect2.y && point.y<=rect1.y);
  
  return res;
}

/* is there an overlap between the rectangle r1
   with opposite corners r11 and r12 
   and the rectangle r2
   with opposite corners r21 and r22  ?
*/
int rectangleOverlap(XPoint r11, XPoint r12, XPoint r21, XPoint r22) {
  XPoint c, p1, p2;
  int res;
  
  /* make sure r11 and r22 are upper left corners, r12 and r22 
     are lower right corners */
  p1.x=MIN(r11.x, r12.x);
  p1.y=MIN(r11.y, r12.y);
  p2.x=MAX(r11.x, r12.x);
  p2.y=MAX(r11.y, r12.y);
  r11=p1;
  r12=p2;

  p1.x=MIN(r21.x, r22.x);
  p1.y=MIN(r21.y, r22.y);
  p2.x=MAX(r21.x, r22.x);
  p2.y=MAX(r21.y, r22.y);
  r21=p1;
  r22=p2;

  /* first check if any of the corners of r1 lies inside r2 */

  c.x = r11.x;
  c.y = r11.y;
  res=pointInRectangle(c, r21, r22);
  if(res)
    return res;
  
  c.x = r12.x;
  res=pointInRectangle(c, r21, r22);
  if(res)
    return res;
  
  c.y = r12.y;
  res=pointInRectangle(c, r21, r22);
  if(res)
    return res;
  
  c.x = r11.x;
  c.y = r12.y;
  res=pointInRectangle(c, r21, r22);
  if(res)
    return res;

  /* check if the rectangles overlap without having points
     "inside each other" */
  
  res = ( (r11.x >= r21.x) && (r11.x <= r22.x) ) &&
        ( (r21.y >= r11.y) && (r21.y <= r12.y) );
  
  if(res)
    return res;

  res = ( (r11.y >= r21.y) && (r11.y <= r22.y) ) &&
        ( (r21.x >= r11.x) && (r21.x <= r12.x) );
  
  if(res)
    return res;
  
  /* check if any of the corners of r2 lies inside r1 */

  c.x = r21.x;
  c.y = r21.y;
  res=pointInRectangle(c, r11, r12);
  if(res)
    return res;
  
  c.x = r22.x;
  res=pointInRectangle(c, r11, r12);
  if(res)
    return res;
  
  c.y = r22.y;
  res=pointInRectangle(c, r11, r12);
  if(res)
    return res;
  
  c.x = r21.x;
  c.y = r22.y;
  res=pointInRectangle(c, r11, r12);

  return res;

}

