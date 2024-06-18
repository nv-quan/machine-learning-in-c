#ifndef IO_H
#define IO_H

#include <csv.h>
#include <stdio.h>
#include <stdlib.h>

#include "config.h"
#include "data.h"

#define BUFFER_SIZE 1024

enum loader_err { NOERR, CSV_ERR, FILE_ERR, MEM_ERR };

/* Data loader options */
#define DL_MEM_BASED 1
#define DL_HAS_HEADER (1 << 1)
#define DL_INSERT_ONE (1 << 2)

typedef struct data_loader_config {
  char options;    /* Data loader options */
  void *mem;       /* If is_mem is non-zero then this will store all the data,
                      otherwise this is NULL */
  size_t mem_size; /* Size of data in memory if is_mem is non-zero,
                      otherwise this is 0 */
  size_t x_dim;    /* Dimension of X, including the first 1 when applicable */
  int y_col;       /* Column from table for y */
  /* Columns from table for x (features). When options has DL_INSERT_ONE,
   * the first member of x_cols is not read when data is loaded. */
  int x_cols[CF_MAX_DIM];
  char file_path[CF_PATH_LEN]; /* File path */
} DLConf;

/* Augmented data structure for point */
typedef struct point_aug {
  size_t x_length;
  int x_filled[CF_MAX_DIM];
  int y_filled;
} PointAug;

/* Data Loader */
typedef struct data_loader {
  FILE *fp;          /* File pointer, for file loader only */
  size_t mem_idx;    /* Index to the current memory block, for memory loader
                        only */
  int record_loaded; /* Number of loaded records, including headers */
  struct csv_parser *csv_prs; /* CSV Parser struct */
  enum loader_err err;        /* Error code */
  DLConf dl_conf;             /* Data loader config */
} DatLoader;

/* Check if data_loader has error
 * Return non-zero if true (returns error code)
 */
int ld_err(DatLoader *loader);

/* Check if the loader config is memory based */
int is_mem_based(DLConf *conf);

/* Check if the data source has header */
int has_header(DLConf *conf);

/* Check if 1 will be inserted into the first x field */
int is_one_insered(DLConf *conf);

/* Load Data
 *
 * Every call to this function load at most nsize points from a data loader to a
 * buffer. No allocation happens inside this function. After this is called,
 * calling ld_err(loader) will indicates if an error occurred. After an error
 * occurred, attempting to call this will result in an undefined behaviour.
 *
 * Parameters:
 * - loader: pointer to the data loader.
 * - nsize: length of the buffer array.
 * - buffer: pointer to the buffer array.
 *
 * Returns:
 * - Number of element loaded into buffer on success.
 * - 0 on failure.
 */
size_t load_data(DatLoader *loader, size_t nsize, Point *buffer);

/* Make data_loader
 *
 * Create (allocate) a new data loader. destroy_dat_loader must be called after
 * done using.
 *
 * Parameters:
 * - dl_conf: pointer to a data_loader_config, borrow from caller
 *
 * Return:
 * - NULL on error
 * - pointer to the new data_loader on success
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

#endif /* ifndef IO_H */
