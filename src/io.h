#ifndef IO_H
#define IO_H

#include "data.h"
#include <csv.h>
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

/* Data Loader
 *
 */
typedef struct data_loader {
  FILE *fp;                   /* File pointer */
  struct csv_parser *csv_prs; /* CSV Parser struct */
  char err_msg[ERR_MSG_SIZE]; /* Error message when data fail to load */
  /*union data_loader_content content;*/
} DatLoader;

/* Load data
 *
 * Every call will load data into a buffer (array of Point).
 * [buffer], [loader] is caller's, no allocation inside function.
 * [size] is the buffer size.
 *
 * Return amount of data loaded or 0 on error
 * On error, set err_msg on data_loader
 * On success, set err_msg to empty string
 */
int load_data(struct data_loader *loader, size_t size, Point *buffer);

/* Make data_loader
 *
 * Create a new data loader. This function will allocate a struct csv_parser.
 * Before freeing data_loader, clear_data_loader must be called to clear the
 * struct. However if the functions returns error, then no memory will be
 * allocated.
 *
 * Parameters:
 *  ld_conf: pointer to a data_loader_config, borrow from caller
 *  dat_loader: pointer to a data_loader, also borrow from caller
 *
 * Return:
 *  -1 on error
 *  0 on success
 */
int make_data_loader(struct data_loader_config *ld_conf,
                     struct data_loader *dat_loader);
/* Clear data_loader
 *
 * Clear a data loader. This must be called after a data_loader finishes its
 * job. This function does NOT free [dat_loader].
 *
 * Parameters:
 *  dat_loader: pointer to a data_loader, borrow from caller
 *
 * Return:
 *  -1 on error
 *  0 on success
 */
int clear_data_loader(struct data_loader *dat_loader);

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
