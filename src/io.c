#include "io.h"
#include "utils.h"
#include <csv.h>
#include <stdio.h>
#include <string.h>

typedef struct csv_context {
  /* CSV field idx in a row */
  int field_idx;

  /* CSV row idx, including heading row */
  int row_idx;

  /* Current point index */
  int point_idx;

  /* Maximum point count stored by point_buF */
  int point_buf_len;

  /* Buffer to save current point */
  Point *point_buf;
} CsvCtx;

/* Callback for each csv field */
static void csv_field_read(void *dat, size_t len, void *custom);

/* Callback for each end of row */
static void csv_eor(int c, void *custom);

/* Parse buffer into csv until either the whole buffer is parsed or maximum
 * number of points are saved into ctx
 */
static size_t parse_buf(DatLoader *loader, char *buf, size_t len, CsvCtx *ctx);

static void init_csv_ctx(CsvCtx *ctx, Point *point_buf, int point_buf_len);

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

static int is_context_full(CsvCtx *context) {
  return context->point_idx >= context->point_buf_len; 
}

size_t load_data(DatLoader *loader, size_t nsize, Point *buffer) {
  size_t sz_read, sz_csv, total_sz_read = 0; /* bytes count */
  size_t ch_sz = sizeof(char);
  CsvCtx ctx;
  char read_buf[BUFFER_SIZE];

  init_csv_ctx(&ctx, buffer, nsize); /* buffer lifetime is this function */
  while (!is_context_full(ctx) && sz_read = fread(read_buf, ch_sz, BUFFER_SIZE, loader->fp)) {
    total_sz_read += sz_read; 
    if (parse_buf(loader, read_buf, sz_read, &ctx) == 0) {
      loader->err = CSV_ERR;
      total_sz_read -= sz_read;
      /* It's an undefined behaviour to call load_data after this point so we don't
       * need to care about loader->fp position
       */
      break;
    }
  }
  if (sz_read == 0 && ferror(loader->fp)) {
      loader->err = FILE_ERR;
  }
  return sz_read;
}

static size_t parse_buf(DatLoader *loader, char *buf, size_t len, CsvCtx *ctx) {
  struct csv_parser *parser = loader->csv_prs;
  void* vctx = (void*) ctx;
  int i;

  for (i = 0; i < len && !is_context_full(ctx); i++) {
    if (csv_parse(parser, buf + i, 1, csv_field_read, csv_eor, vctx) < 1) {
      /* Error has occured */
      loader->err = CSV_ERR;
      break;
    }
  }
  return i;
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

static void init_csv_ctx(CsvCtx *ctx, Point *point_buf, int point_buf_len) {
  ctx.field_idx = 0;
  ctx.row_idx = 0;
  ctx.point_idx = 0;
  ctx.point_buff_len = point_buff_len;
  ctx.point_buf = point_buf;
}

static void csv_field_read(void *dat, size_t len, void *custom) { CsvCtx ctx; }

static void csv_eor(int c, void *custom) { return; }
