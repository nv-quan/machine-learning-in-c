#include "io.h"
#include "utils.h"
#include <csv.h>
#include <stdio.h>

struct csv_context {
  int field_idx;
  int row_idx;
  Point *point_buf; /* Buffer to save current point */
};

/* Callback for each csv field */
static void csv_field_read(void *dat, size_t len, void *custom);
/* Callback for each end of row */
static void csv_eor(int c, void *custom);

DataGetter csv_file_getter(const char *file_path) {
  static FILE *fp;
  char buffer[BUFFER_SIZE];
  int i;
  if ((fp = fopen(file_path, "r")) == NULL) {
    fprintf(stderr, "Can't open file %s", file_path);
    exit(-1); /* Temporary error handling */
  }
  while ((i = fread(buffer, sizeof(char), BUFFER_SIZE, fp)) > 0) {
  }
  return NULL;
}

size_t load_data(struct data_loader *loader, size_t nsize, Point *buffer) {
  size_t sz_read, sz_csv;
  static struct csv_context ctx = {
      0, 0}; /* struct must be static so that callbacks can access to it */
  char read_buf[BUFFER_SIZE];
  size_t buf_len;

  ctx.point_buf = buffer;
  sz_read = fread(read_buf, sizeof(char), BUFFER_SIZE, loader->fp);
  if (sz_read == 0) {
    if (ferror(loader->fp))
      loader->err = FILE_ERR;
    return 0;
  }
  buf_len = sz_read * sizeof(char);
  sz_csv = csv_parse(loader->csv_prs, read_buf, buf_len, csv_field_read,
                     csv_eor, (void *)&ctx);
  if (sz_csv < buf_len) { /* Error has occured */
    loader->err = CSV_ERR;
    return 0;
  }
  return sz_read;
}

int make_data_loader(struct data_loader_config *ld_conf,
                     struct data_loader *dat_loader) {
  struct csv_parser *csv_prs =
      (struct csv_parser *)safe_malloc(sizeof(struct csv_parser));
  FILE *fp;

  if (csv_init(csv_prs, 0)) {
    rp_err("make_data_loader: Can't init csv");
    goto failed;
  }
  if ((fp = fopen(ld_conf->file_path, "r")) == NULL) {
    fprintf(stderr, "make_data_loader: Can't open file %s\n",
            ld_conf->file_path);
    goto failed;
  }
  dat_loader->csv_prs = csv_prs; /* Transfer ownership */
  dat_loader->fp = fp;           /* Transfer ownership */
  dat_loader->err = 0;
  return 0;

failed:
  safe_free((void **)&csv_prs);
  return -1;
}

int clear_data_loader(struct data_loader *dat_loader) {
  int retval = 0;
  if (!(dat_loader->fp) || fclose(dat_loader->fp))
    retval = -1;
  csv_fini(dat_loader->csv_prs, csv_field_read, csv_eor, NULL);
  csv_free(dat_loader->csv_prs);
  return retval;
}

int ld_err(struct data_loader *loader) { return loader->err; }

static void csv_field_read(void *dat, size_t len, void *custom) { return; }

static void csv_eor(int c, void *custom) { return; }
