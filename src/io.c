#include "io.h"
#include "utils.h"
#include <csv.h>
#include <stdio.h>
#include <string.h>

typedef struct csv_context {
  int field_idx;
  int row_idx;
  Point *point_buf; /* Buffer to save current point */
} CsvCtx;

/* Callback for each csv field */
static void csv_field_read(void *dat, size_t len, void *custom);
/* Callback for each end of row */
static void csv_eor(int c, void *custom);

static void init_csv_ctx(CsvCtx *ctx);

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

size_t load_data(DatLoader *loader, size_t nsize, Point *buffer) {
  size_t sz_read, sz_csv;
  static CsvCtx ctx;
  char read_buf[BUFFER_SIZE];
  size_t buf_len;

  init_csv_ctx(&ctx);
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

DatLoader *make_data_loader(DLConf *dl_conf) {
  size_t dl_sz = sizeof(DatLoader);
  DatLoader *dat_loader = (DatLoader *)safe_malloc(dl_sz);

  size_t csv_prs_sz = sizeof(struct csv_parser);
  struct csv_parser *csv_prs = (struct csv_parser *)safe_malloc(csv_prs_sz);

  FILE *fp;

  if (csv_init(csv_prs, 0)) {
    rp_err("make_data_loader: Can't init csv");
    goto failed;
  }
  if ((fp = fopen(dl_conf->file_path, "r")) == NULL) {
    fprintf(stderr, "make_data_loader: Can't open file %s\n",
            dl_conf->file_path);
    goto failed;
  }
  dat_loader->fp = fp;           /* Transfer ownership */
  dat_loader->csv_prs = csv_prs; /* Transfer ownership */
  dat_loader->err = NOERR;
  memcpy((void *)&dat_loader->dl_conf, dl_conf, sizeof(DLConf));
  return dat_loader;

failed:
  safe_free((void **)&csv_prs);
  safe_free((void **)&dat_loader);
  return NULL;
}

void destroy_dat_loader(DatLoader *dat_loader) {
  if (dat_loader->fp)
    fclose(dat_loader->fp);
  csv_fini(dat_loader->csv_prs, csv_field_read, csv_eor, NULL);
  csv_free(dat_loader->csv_prs);
  safe_free((void **)&dat_loader->csv_prs);
}

int ld_err(DatLoader *loader) { return loader->err; }

static void csv_field_read(void *dat, size_t len, void *custom) { CsvCtx ctx; }

static void csv_eor(int c, void *custom) { return; }
