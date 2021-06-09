/* $Id: matrix.c,v 1.2 1996/02/26 08:51:43 lupus Exp $ */
 
/***************************************************************
 * This file is part of the Ginger project.                    *
 * Created 15-02-1996                                          *
 * Niels C. Larsen, Aalborg University, Denmark                *
 * (lupus@iesd.auc.dk)                                         *
 * Martin K. Molz, Aalborg University, Denmark                 *
 * (molz@iesd.auc.dk)                                          *
 ***************************************************************/

/* special purpose matrix datatype used with the tree/hierarchy
   layout algorithm.
*/

#include <stdio.h>
#include "allocmem.h"
#include "matrix.h"

/* ---  private declarations  --- */

#define MAX_LEVELS 100

static int current_edge_crossings;

/* private function prototypes */

static void matrixAtRowPutBarycenter(matrix *m, int r, float b);
static void matrixAtColumnPutBarycenter(matrix *m, int c, float b);
static float matrixRowBarycenter(matrix *m, int r);
static float matrixColumnBarycenter(matrix *m, int c);
static void matrixComputeRowBarycenters(matrix *m);
static void matrixComputeColumnBarycenters(matrix *m);
static void matrixComputeBarycenters(matrix *m);
static void swapElements(redirection_element array[], int i, int j);
static void sortRedirectionVectorOnBarycenter(redirection_element array[], int array_size);
static void sortRedirectionVectorOnBarycenterFollow(redirection_element array[], redirection_element follow_array[], int array_size);
static void matrixSortRowsOnBarycenter(matrix *m);
static void matrixSortColumnsOnBarycenter(matrix *m);
static void matrixSortRowsOnBarycenterFollow(matrix *m, matrix *follow);
static void matrixSortColumnsOnBarycenterFollow(matrix *m, matrix *follow);
static void matrixArrayDown(matrix *m[], int size);
static void matrixArrayUp(matrix *m[], int size);
static void matrixArrayDownUp(matrix *m[], int size);
static int  matrixNodeUpperBarycenter(matrix *m, node *n, int c);
static int  matrixNodeLowerBarycenter(matrix *m, node *n, int r);
static void moveNodeInColumn(matrix *m, int c, int delta_position);
static void moveNodeInRow(matrix *m, int r, int delta_position);
static int  tryMoveNodeInColumn(matrix *m, int column, int old_position,
				int delta_position, int priority);
static int  tryMoveNodeInRow(matrix *m, int row, int old_position,
			     int delta_position, int priority);


/* ---  public functions  --- */

/* crate a matrix with the given dimension (# of rows and columns).
   elements are initialized with integer zero */
matrix *matrixCreate(int r, int c) {
  matrix *res;
  int i;

  res=AllocateMemory(sizeof(matrix));
  res->rows=r;
  res->columns=c;
  res->logical_to_physical_row_number=AllocateMemory(r*sizeof(redirection_element));
  res->logical_to_physical_col_number=AllocateMemory(c*sizeof(redirection_element));
  res->elements=AllocateMemory(r*c*sizeof(int));
  for(i=0; i<r; i++) {
    res->logical_to_physical_row_number[i].redirection=i;
    res->logical_to_physical_row_number[i].n=0;
    res->logical_to_physical_row_number[i].barycenter=0.0;
  }
  for(i=0; i<c; i++) {
    res->logical_to_physical_col_number[i].redirection=i;
    res->logical_to_physical_col_number[i].n=0;
    res->logical_to_physical_col_number[i].barycenter=0.0;
  }
  for(i=0; i<r*c; i++)
    res->elements[i]=0;
  return res;
}

/* deallocate matrix */
void matrixDestroy(matrix *m) {
  free(m->logical_to_physical_row_number);
  free(m->logical_to_physical_col_number);
  free(m->elements);
  free(m);
}

/* create a new matrix identical to m. */
matrix *matrixCopy(matrix *m) {
  matrix *res;
  int i,r,c;
  
  r=m->rows;
  c=m->columns;
  
  res=matrixCreate(r,c);
  
  for(i=0; i<r; i++) {
    res->logical_to_physical_row_number[i]=
      m->logical_to_physical_row_number[i];
  }
  for(i=0; i<c; i++) {
    res->logical_to_physical_col_number[i]=
      m->logical_to_physical_col_number[i];
  }
  for(i=0; i<r*c; i++)
    res->elements[i]=m->elements[i];
  return res;
}

/* return the element at (r,c) */
int matrixAt(matrix *m, int r, int c) {
  int i;
  
  i=m->logical_to_physical_row_number[r].redirection*m->columns
    +m->logical_to_physical_col_number[c].redirection;
  return m->elements[i];
}

/* assign 'element' to (r,c) */
void matrixAtPut(matrix *m, int r, int c, int element) {
  int i;
  
  i=m->logical_to_physical_row_number[r].redirection*m->columns
    +m->logical_to_physical_col_number[c].redirection;
  m->elements[i]=element;
}

/* associate the given node with the matrix row r */
void matrixAtRowPutNode(matrix *m, int r, node *n1) {
  m->logical_to_physical_row_number[r].n=n1;
}

/* associate the given node with the matrix column c */
void matrixAtColumnPutNode(matrix *m, int c, node *n1) {
  m->logical_to_physical_col_number[c].n=n1;
}

/* return the node associated with row r */
node *matrixNodeAtRow(matrix *m, int r) {
  return m->logical_to_physical_row_number[r].n;
}

/* return the node associated with column c */
node *matrixNodeAtColumn(matrix *m, int c) {
  return m->logical_to_physical_col_number[c].n;
}

/* print a representation of the matrix to stdout */
void matrixPrint(matrix *m) {
  int i,j;
  
  for(i=0; i<m->rows; i++) {
    for(j=0; j<m->columns; j++) {
      printf("%3d ", matrixAt(m,i,j));
    }
    printf(" | %3.1f\n", m->logical_to_physical_row_number[i].barycenter);
  }
  for(j=0; j<m->columns; j++)
    printf("----");
  printf("\n");
  for(j=0; j<m->columns; j++)
    printf("%3.1f ", m->logical_to_physical_col_number[j].barycenter);
  printf("\n");
}

/* swap the rows r1 and r2 */
void matrixSwapRows(matrix *m, int r1, int r2) {
  redirection_element tmp;
  tmp=m->logical_to_physical_row_number[r1];
  m->logical_to_physical_row_number[r1]=m->logical_to_physical_row_number[r2];
  m->logical_to_physical_row_number[r2]=tmp;
}

/* swap the columns c1 and c2 */
void matrixSwapColumns(matrix *m, int c1, int c2) {
  redirection_element tmp;
  tmp=m->logical_to_physical_col_number[c1];
  m->logical_to_physical_col_number[c1]=m->logical_to_physical_col_number[c2];
  m->logical_to_physical_col_number[c2]=tmp;
}

/* return the number of crossing edges between the nodes represented in
   rows j and k [Sug81, formula (3)] */
int matrixCrossingsInRows(matrix *m, int j, int k) {
  int a,b,q,sum;
  
  sum=0;
  q=m->columns;
  for(a=0; a<q-1; a++)
    for(b=a+1; b<q; b++) {
      sum+=matrixAt(m,j,b)*matrixAt(m,k,a);
    }
  return sum;
}

/* return the number of crossing edges between the nodes represented in
   the matrix m [Sug81, formula (4)] */
int matrixCrossings(matrix *m) {
  int j,k,p,sum;
  
  sum=0;
  p=m->rows;
  for(j=0; j<p-1; j++)
    for(k=j+1; k<p; k++)
      sum+=matrixCrossingsInRows(m,j,k);
  return sum;
}

/* return the total number of edge crossings in an array of matrices
   [Sug81, formula (5)] */
int matrixArrayCrossings(matrix *array[], int size) {
  int i,sum;
  
  for(i=0, sum=0; i<size; i++)
    sum+=matrixCrossings(array[i]);
  return sum;
}

/* iterate "down" and "up" procedures to reduce the number of edge crossings */
void matrixArrayDownUpIterate(matrix *m[], int size, int max_iterations) {
  int i;
  
  current_edge_crossings=matrixArrayCrossings(m,size);
  
  for(i=0; i<max_iterations; i++) {
    if( current_edge_crossings==0 )
      return;  
    matrixArrayDownUp(m,size);
    /* printf("After iteration %d, crossings: %d\n", i, current_edge_crossings); */
  }
}

/* "down" procedure to adjust the position of nodes in the level given by m */
void matrixPriorityLayoutDown(matrix *m) {
  int i;
  int desired_position;
  node *n;
  
  /* the priority of nodes is given by their upper connectivity */
  for(i=0; i<m->columns; i++) {
    n=matrixNodeAtColumn(m,i);
    if(n) 
      n->tree_layout.priority=n->tree_layout.upper_connectivity;
  }
  
  for(i=0; i<m->columns; i++) {
    n=matrixNodeAtColumn(m,i);
    if(n) {
      /* determine the node's desired position */
      desired_position=matrixNodeUpperBarycenter(m,n,i);

      /* try to move the node to its desired position.
	 This may not be possible because nodes with higher priority
	 occupy the position. In any case, move as close to the
	 desired position as possible */
      tryMoveNodeInColumn(m, i, n->tree_layout.position,
			  desired_position-n->tree_layout.position,
			  n->tree_layout.priority);
    }
  }
}

/* "up" procedure to adjust the position of nodes in the level given by m */
void matrixPriorityLayoutUp(matrix *m) {
  int i;
  int desired_position;
  node *n;
  
  /* the priority of nodes is given by their lower connectivity */
  for(i=0; i<m->rows; i++) {
    n=matrixNodeAtRow(m,i);
    if(n) 
      n->tree_layout.priority=n->tree_layout.lower_connectivity;
  }
  
  for(i=0; i<m->rows; i++) {
    n=matrixNodeAtRow(m,i);
    if(n) {
      /* determine the node's desired position */
      desired_position=matrixNodeLowerBarycenter(m,n,i);
      
      /* try to move the node to its desired position.
	 This may not be possible because nodes with higher priority
	 occupy the position. In any case, move as close to the
	 desired position as possible */
      tryMoveNodeInRow(m, i, n->tree_layout.position,
		       desired_position-n->tree_layout.position,
		       n->tree_layout.priority);
    }
  }
}

/* ---  private functions  --- */

/* assign barycenter to row */
static void matrixAtRowPutBarycenter(matrix *m, int r, float b) {
  int i;
  i=m->logical_to_physical_row_number[r].redirection;
  m->logical_to_physical_row_number[i].barycenter=b;
}

/* assign barycenter to column */
static void matrixAtColumnPutBarycenter(matrix *m, int c, float b) {
  int i;
  i=m->logical_to_physical_col_number[c].redirection;
  m->logical_to_physical_col_number[i].barycenter=b;
}

/* calculate the row barycenter of r [Sug81, formula (9)] */
static float matrixRowBarycenter(matrix *m, int r) {
  int l,q,sum1,sum2;
  
  q=m->columns;
  sum1=sum2=0;
  for(l=0; l<q; l++) {
    sum1+=(l+1)*matrixAt(m,r,l);
    sum2+=matrixAt(m,r,l);
  }
  if( sum1==0 || sum2==0 )
    return 0.0;
  return (float)sum1/(float)sum2;
}

/* calculate the column barycenter of c [Sug81, formula (10)] */
static float matrixColumnBarycenter(matrix *m, int c) {
  int k,p,sum1,sum2;
  
  p=m->rows;
  sum1=sum2=0;
  for(k=0; k<p; k++) {
    sum1+=(k+1)*matrixAt(m,k,c);
    sum2+=matrixAt(m,k,c);
  }
  if( sum1==0 || sum2==0 )
    return 0.0;
  return (float)sum1/(float)sum2;
}

/* calculate row barycenters of all rows in m */
static void matrixComputeRowBarycenters(matrix *m) {
  int r;
  for(r=0; r<m->rows; r++) 
    matrixAtRowPutBarycenter(m, r, matrixRowBarycenter(m,r));
}

/* calculate column barycenters of all columns in m */
static void matrixComputeColumnBarycenters(matrix *m) {
  int c;
  for(c=0; c<m->columns; c++) 
    matrixAtColumnPutBarycenter(m, c, matrixColumnBarycenter(m,c));
}

/* calculate row and column barycenters of m */
static void matrixComputeBarycenters(matrix *m) {
  matrixComputeRowBarycenters(m);
  matrixComputeColumnBarycenters(m);
}

/* swap elements i and j in the redirection vector 'array[]' */
static void swapElements(redirection_element array[], int i, int j) {
  redirection_element tmp;
  
  tmp=array[i];
  array[i]=array[j];
  array[j]=tmp;
}

/* bubble sort the redirection vector in order of ascending barycenters */
static void sortRedirectionVectorOnBarycenter(redirection_element array[], int array_size) {
  int i,j;
  
  for(i=1; i<array_size; i++) 
    for(j=i; j>0 && array[j-1].barycenter>array[j].barycenter; j--)
      swapElements(array,j-1,j);
}

/* bubble sort the redirection vector in order of ascending barycenters.
   Keep the position of elements in the vector 'follow_array[]' relative to
   the vector being sorted ('array[]') constant */
static void sortRedirectionVectorOnBarycenterFollow(redirection_element array[], redirection_element follow_array[], int array_size) {
  int i,j;
  
  for(i=1; i<array_size; i++) 
    for(j=i; j>0 && array[j-1].barycenter>array[j].barycenter; j--) {
      swapElements(array,j-1,j);
      swapElements(follow_array,j-1,j);
    }
}

/* sort the rows of m in order of ascending barycenters. */
static void matrixSortRowsOnBarycenter(matrix *m) {
  sortRedirectionVectorOnBarycenter(m->logical_to_physical_row_number, 
				    m->rows);
}

/* sort the columns of m in order of ascending barycenters. */
static void matrixSortColumnsOnBarycenter(matrix *m) {
  sortRedirectionVectorOnBarycenter(m->logical_to_physical_col_number, 
				    m->columns);
}

/* sort the rows of m in order of ascending barycenters. 
   Keep the order of the columns in the matrix 'follow' relative to
   the order of the rows in 'm' constant */
static void matrixSortRowsOnBarycenterFollow(matrix *m, matrix *follow) {
  if(m->rows!=follow->columns) {
    printf("Dimension mismatch in matrixSortRowsOnBarycenterFollow()\n");
    return;
  }
  sortRedirectionVectorOnBarycenterFollow(m->logical_to_physical_row_number, 
					  follow->logical_to_physical_col_number, 
					  m->rows);
}

/* sort the columns of m in order of ascending barycenters. 
   Keep the order of the rows in the matrix 'follow' relative to
   the order of the columns in 'm' constant */
static void matrixSortColumnsOnBarycenterFollow(matrix *m, matrix *follow) {
  if(m->columns!=follow->rows) {
    printf("Dimension mismatch in matrixSortColumnsOnBarycenterFollow()\n");
    return;
  }
  sortRedirectionVectorOnBarycenterFollow(m->logical_to_physical_col_number, 
					  follow->logical_to_physical_row_number, 
					  m->columns);
}

/* "down" procedure for reducing the number of edge crossings */
static void matrixArrayDown(matrix *m[], int size) {
  int i,c1,c2;
  matrix *m1[MAX_LEVELS];
  
  if(size>=MAX_LEVELS)
    return;
  
  /* save a "backup" of the matrices, in case the "down" procedure 
     makes the result _worse_ (more edge crossings) rather than better */
  for(i=0; i<size; i++) {
    m1[i]=matrixCopy(m[i]);
  }
  
  c1=current_edge_crossings;
  /* printf("In down, before: %d\n", c1); */
  
  for(i=0; i<size-1; i++) {
    matrixComputeColumnBarycenters(m[i]);
    matrixSortColumnsOnBarycenterFollow(m[i],m[i+1]);
  }
  matrixComputeColumnBarycenters(m[i]);
  matrixSortColumnsOnBarycenter(m[i]);
  
  c2=matrixArrayCrossings(m,size);
  /* printf("In down, after: %d\n", c2); */

  if( c2<=c1 ) {              /* if the result has been improved */
    current_edge_crossings=c2;
    for(i=0; i<size; i++)     /* destroy backup */
      matrixDestroy(m1[i]);
  } else                   
    for(i=0; i<size; i++) {   /* otherwise destroy the original */
      matrixDestroy(m[i]);    /*   and use the backup           */
      m[i]=m1[i];
    }
}

/* "up" procedure for reducing the number of edge crossings */
static void matrixArrayUp(matrix *m[], int size) {
  int i,c1,c2;
  matrix *m1[MAX_LEVELS];
  
  if(size>=MAX_LEVELS)
    return;
  
  /* save a "backup" of the matrices, in case the "up" procedure 
     makes the result _worse_ (more edge crossings) rather than better */
  for(i=0; i<size; i++) {
    m1[i]=matrixCopy(m[i]);
  }
  
  c1=current_edge_crossings;
  /* printf("In up, before: %d\n", c1); */
  
  for(i=size-1; i>0; i--) {
    matrixComputeRowBarycenters(m[i]);
    matrixSortRowsOnBarycenterFollow(m[i],m[i-1]);
  }
  matrixComputeRowBarycenters(m[0]);
  matrixSortRowsOnBarycenter(m[0]);
  
  c2=matrixArrayCrossings(m,size);
  /* printf("In up, after: %d\n", c2); */
  
  if( c2<=c1 ) {               /* if the result has been improved */
    current_edge_crossings=c2;
    for(i=0; i<size; i++)      /* destroy the backup */
      matrixDestroy(m1[i]);
  } else
    for(i=0; i<size; i++) {    /* otherwise destroy the original */
      matrixDestroy(m[i]);     /*   and use the backup           */
      m[i]=m1[i];
    }
}

/* call the "down" and "up" procedures */
static void matrixArrayDownUp(matrix *m[], int size) {
  matrixArrayDown(m,size);
  /* printf("After down, crossings: %d\n", current_edge_crossings); */
  if( current_edge_crossings==0 )
    return;  
  matrixArrayUp(m,size);
  /* printf("After up, crossings: %d\n", current_edge_crossings); */
}

/* calculate the upper barycenter of node 'n'. [Sug81, formula (11)]  */
static int matrixNodeUpperBarycenter(matrix *m, node *n, int c) {
  int i, sum, pos;
  node *n1;
  
  if( n->tree_layout.upper_connectivity==0 )
    return -1;
  
  sum=0;
  for(i=0; i<m->rows; i++) {
    n1=matrixNodeAtRow(m,i);
    if(n1)
      sum+=n1->tree_layout.position*matrixAt(m,i,c);
  }
  return sum/n->tree_layout.upper_connectivity;
}

/* calculate the lower barycenter of node 'n'. [Sug81, formula (12)]  */
static int matrixNodeLowerBarycenter(matrix *m, node *n, int r) {
  int i, sum, pos;
  node *n1;
  
  if( n->tree_layout.lower_connectivity==0 )
    return -1;
  
  sum=0;
  for(i=0; i<m->columns; i++) {
    n1=matrixNodeAtColumn(m,i);
    if(n1)
      sum+=n1->tree_layout.position*matrixAt(m,r,i);
  }
  return sum/n->tree_layout.lower_connectivity;
}

/* update the position of node at column 'c' */
static void moveNodeInColumn(matrix *m, int c, int delta_position) {
  node *n;
  
  n=matrixNodeAtColumn(m,c);
  if(n) {
    n->tree_layout.position+=delta_position;
  }
}

/* update the position of node at row 'r' */
static void moveNodeInRow(matrix *m, int r, int delta_position) {
  node *n;
  
  n=matrixNodeAtRow(m,r);
  if(n) {
    n->tree_layout.position+=delta_position;
  }
}

/* move the node at the given 'column' from 'old_position' to
   'old_position+delta_position', if possible. Otherwise move the node 
   as close to the desired position as possible. 
   If other nodes occupy the desired position, then try to (recursively)
   move them, if they have lower priority than the given 'priority'.
   Return the delta_position that the node is actually moved.
*/
static int tryMoveNodeInColumn(matrix *m, int column, int old_position,
			       int delta_position, int priority) {
  int i,dist;
  node *n;
  
  /* if delta_position is zero, we're done */
  if(delta_position==0)
    return 0;

  if(delta_position<0) {  /******** moving to the left ********/
    if(column==0) {
      /* the leftmost node may be moved further left */
      moveNodeInColumn(m,column,delta_position);
      return delta_position;
    }
    i=column-1;
    n=matrixNodeAtColumn(m,i);  /* get hold of the left neighbor */
    if(n) {
      if( n->tree_layout.position<old_position+delta_position ) {
	/* if the neighbor is far enough away */
	moveNodeInColumn(m,column,delta_position);
	return delta_position;
      }
      else if( n->tree_layout.priority<priority ) {
	/* if the neighbor has lower priority, we can try to move it */

	dist=n->tree_layout.position-old_position+1;
	  /* dist is what we can move without moving the neighbor */

	dist+=tryMoveNodeInColumn(m, i, n->tree_layout.position, 
				 delta_position-dist, priority);
          /* now dist is what we can move after the neighbor is moved */

	if(dist!=0) 
	  moveNodeInColumn(m,column,dist);
	return dist;
      }
      else { /* else go as far as we can get without moving the neighbor */
	dist=n->tree_layout.position-old_position+1;
	if(dist!=0) 
	  moveNodeInColumn(m,column,dist);
	return dist;
      }
    }
    else /* this should not happen, but just in case */
      return 0;
  }
  
  if(delta_position>0) {         /******** moving to the right ********/
    if(column==m->columns-1) {
      /* the rightmost node may be moved further right */
      moveNodeInColumn(m,column,delta_position);
      return delta_position;
    }
    i=column+1;
    n=matrixNodeAtColumn(m,i);  /* get hold of the right neighbor */
    if(n) {
      if( n->tree_layout.position>old_position+delta_position ) {
	/* if the neighbor is far enough away */
	moveNodeInColumn(m,column,delta_position);
	return delta_position;
      }
      else {
	/* move the neighbors to the right. We can do this
	   without regard to the priority because the right neighbors will
	   simply undo the move if they have higher priority:
	   Right neighbors will be processed after this node. */
	dist=n->tree_layout.position-old_position-1;
	dist+=tryMoveNodeInColumn(m, i, n->tree_layout.position, 
				 delta_position-dist, priority);
	if(dist!=0) 
	  moveNodeInColumn(m,column,dist);
	return dist;
      }
    }
    else { /* this should not happen */
      printf("node not found\n");
      return 0;
    }
  }
  
  return 0;
}

/* move the node at the given 'row' from 'old_position' to
   'old_position+delta_position', if possible. Otherwise move the node 
   as close to the desired position as possible. 
   If other nodes occupy the desired position, then try to (recursively)
   move them, if they have lower priority than the given 'priority'.
   Return the delta_position that the node is actually moved.

   This procedure is identical to the previous except that it
   operates on the row elements in the matrix instead of column elements.
   Some day we might join the two, giving them another parameter to 
   indicate row/column.
*/
static int tryMoveNodeInRow(matrix *m, int row, int old_position,
			    int delta_position, int priority) {
  int i,dist;
  node *n;
  
  if(delta_position==0)
    return 0;

  if(delta_position<0) {  /******** moving to the left ********/
    if(row==0) {
      /* the leftmost node may be moved further left */
      moveNodeInRow(m,row,delta_position);
      return delta_position;
    }
    i=row-1;
    n=matrixNodeAtRow(m,i);
    if(n) {
      if( n->tree_layout.position<old_position+delta_position ) {
	/* if the neighbor is far enough away */
	moveNodeInRow(m,row,delta_position);
	return delta_position;
      }
      else if( n->tree_layout.priority<priority ) {
	/* if the neighbor has lower priority, we can try to move it */
	dist=n->tree_layout.position-old_position+1;
	  /* dist is what we can move without moving the neighbor */
	dist+=tryMoveNodeInRow(m, i, n->tree_layout.position, 
				 delta_position-dist, priority);
	if(dist!=0) 
	  moveNodeInRow(m,row,dist);
	return dist;
      }
      else { /* else go as far as we can get */
	dist=n->tree_layout.position-old_position+1;
	if(dist!=0) 
	  moveNodeInRow(m,row,dist);
	return dist;
      }
    }
    else
      return 0;
  }
  
  if(delta_position>0) {  /******** moving to the right ********/
    if(row==m->rows-1) {
      /* the rightmost node may be moved further right */
      moveNodeInRow(m,row,delta_position);
      return delta_position;
    }
    i=row+1;
    n=matrixNodeAtRow(m,i);
    if(n) {
      if( n->tree_layout.position>old_position+delta_position ) {
	/* if the neighbor is far enough away */
	moveNodeInRow(m,row,delta_position);
	return delta_position;
      }
      else {
	/* move the neighbors to the right. We can do this
	   without regard to the priority because the neighbors will
	   simply undo the move if they have higher priority:
	   Right neighbors will be processed after this node. */
	dist=n->tree_layout.position-old_position-1;
	dist+=tryMoveNodeInRow(m, i, n->tree_layout.position, 
			       delta_position-dist, priority);
	if(dist!=0) 
	  moveNodeInRow(m,row,dist);
	return dist;
      }
    }
    else {
      printf("node not found\n");
      return 0;
    }
  }
  
  return 0;
}

