#ifndef DATA_H
#define DATA_H
#include <stdlib.h>

#include "config.h"
#include "utils.h"

/* (x, y) data point where x is array of n length and y is double */
typedef struct point_data {
  size_t x_length;
  double x[CF_MAX_DIM];
  double y;
} Point;

/* Matrix data structure */
typedef struct matrix {
  size_t row;
  size_t col;
  double* val;
} Mat;

/* Print the value of a point */
void log_point(Point* point);

/* Create a matrix from an array of values.
 *
 * Parameters:
 * - val: Pointer to an array of double values to populate the matrix.
 * - row: Number of rows in the matrix.
 * - col: Number of columns in the matrix.
 *
 * Returns:
 * - A pointer to the created Mat if successful.
 * - NULL if unsuccessful.
 */
Mat* creat_mat_from_val(double* val, size_t row, size_t col);

/* Set matrix element. Users are responsible for checking the validity of this
 * operation.
 *
 * Parameters:
 * - mat: Pointer to the Mat structure.
 * - i: Row index.
 * - j: Column index.
 * - val: Value to set at the specified position.
 */
void set_mat_elem(Mat* mat, size_t i, size_t j, double val);

/* Get matrix element. Users are responsible for checking the validity of this
 * operation.
 *
 * Parameters:
 * - mat: Pointer to the Mat structure.
 * - i: Row index.
 * - j: Column index.
 *
 * Returns:
 * - The value of the element at the specified position.
 */
double get_mat_elem(Mat* mat, size_t i, size_t j);

/* Set a row in a matrix.
 *
 * Parameters:
 * - mat: Pointer to the Mat structure.
 * - row_idx: Index of the row to be set.
 * - value: Pointer to an array of double values to set in the specified row.
 */
void set_mat_row(Mat* mat, size_t row_idx, double* value);

/* Set a column in a matrix.
 *
 * Parameters:
 * - mat: Pointer to the Mat structure.
 * - col_idx: Index of the column to be set.
 * - value: Pointer to an array of double values to set in the specified column.
 */
void set_mat_col(Mat* mat, size_t col_idx, double* value);

/* Create a new matrix of size row x col */
Mat* creat_mat(size_t row, size_t col);

/* Destroy the matrix mat */
void destr_mat(Mat* mat);

#endif
