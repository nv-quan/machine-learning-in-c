#ifndef IO_H
#define IO_H

#include <csv.h>
#include <stdio.h>
#include <stdlib.h>

#include "data.h"

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

enum loader_err { NOERR, CSV_ERR, FILE_ERR };

/* TODO: Make memory data loader */
typedef struct data_loader_config {
  char file_path[MAX_FILE_PATH_LENGTH]; /* File path */
  size_t x_dim;                         /* Dimension of X */
  int x_cols[MAX_FEATURE_SIZE]; /* Columns from table for x (features) */
  int y_col;                    /* Column from table for y */
  int has_header;               /* 1 if the csv file has header, 0 if not */
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
  /* File pointer */
  FILE *fp;

  /* CSV Parser struct */
  struct csv_parser *csv_prs;

  /* Error code */
  enum loader_err err;

  /* Data loader config */
  DLConf dl_conf;
} DatLoader;

/* Check if data_loader has error
 * Return non-zero if true (returns error code)
 */
int ld_err(DatLoader *loader);

/* Load data
 *
 * Every call will load data into a buffer (array of Point).
 * [buffer], [loader] is caller's, no allocation inside function.
 * [nsize] is the buffer size (number of element).
 *
 * Return amount of data loaded (in number of element).
 * ld_err(loader) indicates if error happened.
 * After error happened, attempting to call load_data will result in undefined
 * behaviours.
 */
size_t load_data(DatLoader *loader, size_t nsize, Point *buffer);

/* Make data_loader
 *
 * Create (allocate) a new data loader. destroy_dat_loader must be called after
 * done using.
 *
 * Parameters:
 *  dl_conf: pointer to a data_loader_config, borrow from caller
 *
 * Return:
 *   NULL on error
 *   pointer to the new data_loader on success
 */
DatLoader *make_data_loader(DLConf *dl_conf);

/* Clear data_loader
 *
 * Clear a data loader and consume it. This must be called after
 * a data_loader finishes its job.
 *
 * Parameters:
 *  dat_loader: pointer to a data_loader, borrow from caller
 */
void destroy_dat_loader(DatLoader *dat_loader);

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
