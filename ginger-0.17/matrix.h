/* $Id: matrix.h,v 1.1 1996/02/19 10:34:46 lupus Exp $ */
 
/***************************************************************
 * This file is part of the Ginger project.                    *
 * Created 15-02-1996                                          *
 * Niels C. Larsen, Aalborg University, Denmark                *
 * (lupus@iesd.auc.dk)                                         *
 * Martin K. Molz, Aalborg University, Denmark                 *
 * (molz@iesd.auc.dk)                                          *
 ***************************************************************/

#ifndef MATRIX_H
#define MATRIX_H

#include "node.h"

typedef struct {
  int   redirection;
  float barycenter;
  node  *n;
} redirection_element;

typedef struct {
  int rows;
  int columns;
  redirection_element *logical_to_physical_row_number;
  redirection_element *logical_to_physical_col_number;
  int *elements;
} matrix;

matrix *matrixCreate(int r, int c);
void    matrixDestroy(matrix *m);
matrix *matrixCopy(matrix *m);
int     matrixAt(matrix *m, int r, int c);
void    matrixAtPut(matrix *m, int r, int c, int element);
void    matrixAtRowPutNode(matrix *m, int r, node *n1);
void    matrixAtColumnPutNode(matrix *m, int c, node *n1);
node   *matrixNodeAtRow(matrix *m, int r);
node   *matrixNodeAtColumn(matrix *m, int c);

void    matrixPrint(matrix *m);
void    matrixSwapRows(matrix *m, int r1, int r2);
void    matrixSwapColumns(matrix *m, int c1, int c2);

int matrixCrossingsInRows(matrix *m, int j, int k);
int matrixCrossings(matrix *m);
int matrixArrayCrossings(matrix *array[], int size);

void matrixArrayDownUpIterate(matrix *m[], int size, int max_iterations);
void matrixPriorityLayoutDown(matrix *m);
void matrixPriorityLayoutUp(matrix *m);

#endif
