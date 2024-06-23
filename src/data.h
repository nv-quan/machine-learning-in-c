#ifndef DATA_H
#define DATA_H
#include <stdlib.h>

#include "config.h"

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
  double *val;
} Mat;

/* Print the value of a point */
void log_point(Point *point);

/* Print the values of a mat */
void log_mat(Mat *mat, const char *name);

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
Mat *mat_creat_from_val(double *val, size_t row, size_t col);

/* Set matrix element. Users are responsible for checking the validity of this
 * operation.
 *
 * Parameters:
 * - mat: Pointer to the Mat structure.
 * - row: Row index.
 * - col: Column index.
 * - val: Value to set at the specified position.
 */
void mat_set(Mat *mat, size_t row, size_t col, double val);

/* Get matrix element. Users are responsible for checking the validity of this
 * operation.
 *
 * Parameters:
 * - mat: Pointer to the Mat structure.
 * - row: Row index.
 * - col: Column index.
 *
 * Returns:
 * - The value of the element at the specified position.
 */
double mat_get(Mat *mat, size_t row, size_t col);

/* Set a row in a matrix.
 *
 * The values are copied into the matrix's internal memory.
 *
 * Parameters:
 * - mat: Pointer to the Mat structure.
 * - row_idx: Index of the row to be set.
 * - value: Pointer to an array of double values to set in the specified row.
 *
 * Returns:
 * - Pointer to the same matrix if operation is successful.
 * - NULL if it is unsuccessful.
 */
Mat *mat_set_row(Mat *mat, size_t row_idx, double *value);

/* Set a column in a matrix.
 *
 * The values are copied into the matrix's internal memory.
 *
 * Parameters:
 * - mat: Pointer to the Mat structure.
 * - col_idx: Index of the column to be set.
 * - value: Pointer to an array of double values to set in the specified column.
 *
 * Returns:
 * - Pointer to the same matrix if operation is successful.
 * - NULL if it is unsuccessful.
 */
Mat *mat_set_col(Mat *mat, size_t col_idx, double *value);

/* Create a new matrix of size row x col */
Mat *mat_creat(size_t row, size_t col);

/* Clone a matrix
 *
 * Parameters:
 * - mat: The matrix to clone
 *
 * Returns:
 * - pointer to the newly created mat if successful
 * - NULL if unsuccessful
 * */
Mat *mat_clone(Mat *mat);

/* Destroy matrices */
void mat_destr(Mat *mat);

/* Attempt to resize matrix.
 * Resizing is destructive, meaning that after resizing, all values become
 * undefined. However if resizing is not successful then mat will not be
 * changed. If the new size equals to the old size, mat will also not be
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
Mat *mat_resize(Mat *mat, size_t new_row, size_t new_col);

/* Set matrix's value from an array of values
 *
 * Parameters:
 * - mat: Pointer to the matrix
 * - arr: The array of values
 * - len: The array's length
 *
 * Returns:
 * - The same pointer passed to the function if successful
 * - NULL if unsuccessful
 */
Mat *mat_set_val(Mat *mat, double *arr, size_t len);

/* Safely get row count of a matrix. This function will return 0 if the matrix
 * is NULL.
 */
size_t mat_get_rowc(Mat *mat);

/* Safely get col count of a matrix. This function will return 0 if the matrix
 * is NULL.
 */
size_t mat_get_colc(Mat *mat);

/* Safely get the pointer to val of a matrix. This function will return NULL if
 * the matrix is NULL.
 */
double *mat_get_val(Mat *mat);

/* Dynamic string
 *
 * This is a struct that guarantees safe string manipulations. [raw] is a
 * pointer to a char array, [size] is the size of the memory that hold that
 * array. [raw] is not guaranteed to be null terminated.
 */
typedef struct sized_string {
  char *raw;
  size_t size;
  size_t capacity;
} Str;

/* Create a Str string with a predefined size
 *
 * This returns a Str on success and NULL on failure
 */
Str *str_creat(size_t size);

/* Destroy a Str string */
void str_destr(Str *string);

/*
Str *str_clone(Str *s);
Str *str_creat_from_raw(const char *raw, size_t size);
*/

/* Create a Str from a null-terminated string
 *
 * This is provided for convenience. However this function is unsafe when
 * passed a regular string.
 */
Str *str_creat_from_char_arr(const char *char_arr);

/* Safely get the raw character array from a Str
 *
 * If string is NULL then this will return NULL
 * Otherwise it returns the address of the raw character array
 */
char *str_get_raw(Str *string);

/* Safely get the size of a Str
 *
 * If string is NULL then this will return 0
 * Otherwise it returns the size of Str string
 */
size_t str_get_size(Str *string);
#endif
