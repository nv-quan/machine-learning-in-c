#ifndef IO_H
#define IO_H

#include <csv.h>
#include <stdio.h>
#include <stdlib.h>

#include "config.h"
#include "data.h"

#define BUFFER_SIZE 1024

enum loader_err { NOERR, CSV_ERR, FILE_ERR, MEM_ERR };

/* Data loader options
 *
 * DL_MEM_BASED:
 *  Whether the data comes from memory or not. If this option is set, then
 *  a user need to provide the config with a pointer to the memory segment that
 *  stures the data
 * DL_HAS_HEADER:
 *  Whether the data has a row of header or not. If this option is set, then the
 *  data loader will skip the first row when reading data.
 * DL_INSERT_ONE:
 *  Whether to insert x0 into (x1, x2, x3...) where x0 = 1. So the data point
 *  becomes (1, x1, x2, x3...)
 */
#define DL_MEM_BASED 1
#define DL_HAS_HEADER (1 << 1)
#define DL_INSERT_ONE (1 << 2)

typedef struct data_loader_config {
  /* Data loader options */
  char options;

  /* If is_mem is non-zero then this will store all the data, otherwise this is
   * NULL */
  char *mem;

  /* Size of data in memory if is_mem is non-zero, otherwise this is 0 */
  size_t mem_size;

  /* Dimension of X, including the first 1 when applicable */
  size_t x_dim;

  /* Column from table for y */
  int y_col;

  /* Columns from table for x (features). When options has DL_INSERT_ONE,
   * the first member of x_cols is not read when data is loaded. */
  int x_cols[CF_MAX_DIM];

  /* File path */
  char file_path[LONG_STR_LEN];
} DLConf;

/* Augmented data structure for point */
typedef struct point_aug {
  size_t x_length;
  int x_filled[CF_MAX_DIM];
  int y_filled;
} PointAug;

/* Data Loader */
typedef struct data_loader {
  /* File pointer, for file loader only */
  FILE *fp;

  /* Index to the current memory block, for memory loader only */
  size_t mem_idx;

  /* Number of loaded records, including headers */
  int record_loaded;

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

/* Check if the loader config is memory based */
int is_mem_based(DLConf *conf);

/* Check if the data source has header */
int has_header(DLConf *conf);

/* Check if 1 will be inserted into the first x field */
int is_one_insered(DLConf *conf);

/* Serialize data loader config into a string and save into a file
 *
 * The file will be overwritten by this function.
 *
 * Parameters:
 * - conf: a data loader config.
 * - file_path: path to a text file.
 *
 * Return:
 * - 0 on success
 * - non-zero on failure
 */
int save_dlconf(DLConf *conf, const char *file_path);

/* Load data loader config from a file
 *
 * Parameters:
 * - file_path: path to a text file that stores a data loader config
 *
 * Return:
 * - pointer to a newly allocated DLConf on success
 * - NULL on failure
 */
DLConf *load_dlconf(const char *file_path);

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
 * Return:
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
