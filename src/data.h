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
  size_t capacity;
  double* val;
} Mat;

/* Print the value of a point */
void log_point(Point* point);

/* Create a matrix from an array of values.
 *
 * The newly created mat will not take ownership of the val pointer but only
 * copy values.
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
Mat* mat_creat_from_val(double* val, size_t row, size_t col);

/* Set matrix element. Users are responsible for checking the validity of this
 * operation.
 *
 * Parameters:
 * - mat: Pointer to the Mat structure.
 * - i: Row index.
 * - j: Column index.
 * - val: Value to set at the specified position.
 */
void mat_set(Mat* mat, size_t i, size_t j, double val);

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
double mat_get(Mat* mat, size_t i, size_t j);

/* Set a row in a matrix.
 *
 * Parameters:
 * - mat: Pointer to the Mat structure.
 * - row_idx: Index of the row to be set.
 * - value: Pointer to an array of double values to set in the specified row.
 */
void mat_set_row(Mat* mat, size_t row_idx, double* value);

/* Set a column in a matrix.
 *
 * Parameters:
 * - mat: Pointer to the Mat structure.
 * - col_idx: Index of the column to be set.
 * - value: Pointer to an array of double values to set in the specified column.
 */
void mat_set_col(Mat* mat, size_t col_idx, double* value);

/* Create a new matrix of size row x col */
Mat* mat_creat(size_t row, size_t col);

/* Clone a matrix
 *
 * Parameters:
 * - mat: The matrix to clone
 *
 * Returns:
 * - pointer to the newly created mat if successful
 * - NULL if unsuccessful
 * */
Mat* mat_clone(Mat* mat);

/* Destroy the matrix mat */
void mat_destr(Mat* mat);

/* Attempt to resize matrix.
 * Resizing is destructive, meaning that after resizing, all values become
 * undefined. However if resizing is not successful then mat will not be
 * changed.
 *
 * Parameters:
 * - mat: Pointer to matrix to resize.
 * - new_row: new row count.
 * - new_col: new col count.
 *
 * Returns:
 * - the same pointer (mat) if successful
 * - NULL if unsuccessful.
 */
Mat* mat_resize(Mat* mat, size_t new_row, size_t new_col);

/* Set matrix's value from an array of values
 *
 * Parameters:
 * - mat: Pointer to the matrix
 *   arr: The array of values
 *   len: The array's length
 *
 * Returns:
 * - The same pointer passed to the function if successful
 *   NULL if unsuccessful
 */
Mat* mat_set_val(Mat* mat, double* arr, size_t len);

#endif
