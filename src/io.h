#ifndef IO_H
#define IO_H

#include "data.h"
#include <stdio.h>
#include <stdlib.h>

#define BUFFER_SIZE 1024
#define MAX_FILE_PATH_LENGTH 4096
#define ERR_MSG_SIZE 1024

/*enum source_type { CSV_FILE };
 *enum data_type { POINT };
 */
enum data_loader_type {
  DMEM, /* Memory */
  DFILE /* File */
};

/* TODO: Make memory data loader */
typedef struct data_loader_config {
  /* enum data_loader_type type; */

  /* File path */
  char file_path[MAX_FILE_PATH_LENGTH];
} DLConf;

/* File data loader
 * [fp]: file pointer, use fread to read next data
 */
struct file_data_loader {
  FILE *fp;
};

/* Memory data loader
 * [dp]: pointer to the memory block that stores data
 * when get data out, advance this pointer
 * using [type] to determine type of the pointer
struct memory_data_loader {
  enum memory_data_type type;
  void *dp;
};
*/

/* TODO: Make memory loader */
/*
union data_loader_content {
  struct file_data_loader file;
};
*/

typedef struct data_loader {
  struct data_loader_config conf;
  FILE *fp;                   /* File pointer */
  char err_msg[ERR_MSG_SIZE]; /* Error message when data fail to load */
  /*union data_loader_content content;*/
} DatLoader;

/* Function to load data
 * Every call will load data into a buffer (array of Point)
 *
 * buffer, loader owner is caller, the function will not allocate anything
 * size is the buffer size
 *
 * Return amount of data loaded or 0 on error
 * On error, sets err_msg on data_loader
 */
int load_data(struct data_loader *loader, size_t size, Point *buffer);

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
 *
 * Provide the function with allocated Point
 *  It will allocate p.x and move ownership to the caller
 *  Return 1 when success and 0 when fail
 *  If it fails then no allocation will be made, owner doesn't need to care
 */
typedef int (*PointGetter)(Point *);

#endif /* ifndef IO_H */
