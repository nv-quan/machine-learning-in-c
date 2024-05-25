#ifndef IO_H
#define IO_H

#include <data.h>
#include <stdlib.h>

#define MAX_DATA_LENGTH 10000
#define BUFFER_SIZE 1024
#define MAX_FILE_PATH_LENGTH 4096

struct data_source {
  char file_path[MAX_FILE_PATH_LENGTH];
};

/* Function to get data
 * parameters:
 * (1) void*: points to an allocated buffer
 * (2) size_t: buffer size
 * Return size of the data
 * Return EOF when there is an error or EOF
 */
typedef int (*DataGetter)(void *, size_t);

/* Get data point
 * Usage:
 *  Point p;
 *  PointGetter(&p);
 * Provide the function with allocated Point
 * It will allocate p.x and move ownership to the caller
 * Return 1 when success and 0 when fail
 * If it fails then no allocation will be made, owner doesn't need to care
 */
typedef int (*PointGetter)(Point *);

#endif /* ifndef IO_H */
