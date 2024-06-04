#ifndef IO_H
#define IO_H

#include <csv.h>
#include <stdio.h>
#include <stdlib.h>

#include "config.h"
#include "data.h"

#define BUFFER_SIZE 1024
#define ERR_MSG_SIZE 1024

enum loader_err { NOERR, CSV_ERR, FILE_ERR };

/* TODO: Make memory data loader */
typedef struct data_loader_config {
  int is_mem;      /* Check if the data comes from the memory or disk */
  void *mem;       /* If is_mem is non-zero then this will store all the data,
                      otherwise this is NULL */
  size_t mem_size; /* Size of data in memory if is_mem is non-zero,
                      otherwise this is 0 */
  char file_path[CF_PATH_LEN]; /* File path */
  size_t x_dim;                /* Dimension of X */
  int x_cols[CF_FEAT_DIM];     /* Columns from table for x (features) */
  int y_col;                   /* Column from table for y */
  int has_header; /* Non-zero if the csv file has header, 0 if not */
} DLConf;

/* Data Loader */
typedef struct data_loader {
  /* ======= FILE LOADER ========== */
  FILE *fp;                   /* File pointer */
  struct csv_parser *csv_prs; /* CSV Parser struct */

  /* ======= MEMORY LOADER ======== */
  size_t mem_idx; /* Index to the current memory block */

  /* ======= COMMON =============== */
  enum loader_err err; /* Error code */
  DLConf dl_conf;      /* Data loader config */
} DatLoader;

/* Check if data_loader has error
 * Return non-zero if true (returns error code)
 */
int ld_err(DatLoader *loader);

/* Load Data
 *
 * This function loads data into a buffer (an array of Points) with every call.
 * The [buffer] and [loader] are provided by the caller. There is no allocation
 * inside this function. [nsize] represents the buffer's element count.
 *
 * Returns the number of elements loaded.
 * ld_err(loader) indicates if an error occurred.
 * After an error occurs, attempting to call load_data will result in
 * undefined behavior.
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

#endif /* ifndef IO_H */
