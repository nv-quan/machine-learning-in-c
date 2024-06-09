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

/* Create a matrix using the x values from an array of points.
 *
 * Parameters:
 * - points: An array of points.
 * - len: The number of elements in the array.
 *
 * Returns:
 * - A pointer to the created Mat if successful. The created matrix has a size
 *   of x_length x len.
 * - NULL if unsuccessful.
 */
Mat* creat_mat_from_x(Point* points, size_t len);

/* Create a matrix using the y values from an array of points.
 *
 * Parameters:
 * - points: An array of points.
 * - len: The number of elements in the array.
 *
 * Returns:
 * - A pointer to the created Mat if successful. The created matrix has a size
 *   of 1 x len.
 * - NULL if unsuccessful.
 */
Mat* creat_mat_from_y(Point* points, size_t len);

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

/* Create a new matrix of size row x col */
Mat* creat_mat(size_t row, size_t col);

/* Destroy the matrix mat */
void destr_mat(Mat* mat);

#endif
