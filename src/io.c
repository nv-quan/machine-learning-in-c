#include "io.h"

#include <csv.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utils.h"

typedef struct csv_context {
  int begin_row;  /* Row index where data start, normally 1 if csv has header
                     and 0 if not */
  int field_idx;  /* CSV field idx in a row */
  int row_idx;    /* CSV row idx, including the header */
  int point_idx;  /* Current point index */
  Point *points;  /* Point to an array of points */
  int points_len; /* Length of points array */
  DatLoader *loader;
} CsvCtx;

/* Callback for each end of field */
static void csv_eofd(void *dat, size_t len, void *custom);

/* Callback for each end of row */
static void csv_eor(int c, void *custom);

/* Parse buffer into csv until either the whole buffer is parsed or maximum
 * number of points are saved into ctx. Return number of bytes parsed. Return
 * can be smaller than len if ctx is full.
 */
static size_t parse_buf(DatLoader *loader, char *buf, size_t len, CsvCtx *ctx);

static void init_csv_ctx(CsvCtx *ctx, Point *buf, int len, DatLoader *loader);

DataGetter
csv_file_getter(const char *file_path) {
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

static int
is_ctx_full(CsvCtx *context) {
  return context->point_idx >= context->points_len;
}

size_t
load_data(DatLoader *loader, size_t nsize, Point *points) {
  size_t sz_read, sz_parse, total_sz_read = 0; /* bytes count */
  CsvCtx ctx;
  char buffer[BUFFER_SIZE];
  FILE *fp = loader->fp;

  /* pointers lifetime is same with load_data */
  init_csv_ctx(&ctx, points, nsize, loader);

  while (!is_ctx_full(&ctx) && (sz_read = fread(buffer, 1, BUFFER_SIZE, fp))) {
    if ((sz_parse = parse_buf(loader, buffer, sz_read, &ctx))) {
      /* Seek fp in case sz_parse < sz_read. This is to make sure the next
       * call to load_data pick up where this finishes. When this case
       * happens, is_ctx_full also returns true on the next call so the
       * loop will be stopped. */

      if (fseek(fp, (long)sz_parse - (long)sz_read, SEEK_CUR) != 0) {
        loader->err = FILE_ERR;
        /* It's an undefined behaviour to call load_data after this point
         * so we don't need to care about fp position */
        break;
      }
      total_sz_read += sz_parse;
    } else {
      loader->err = CSV_ERR;
      break;
    }
  }
  if (!is_ctx_full(&ctx) && sz_read == 0 && ferror(fp)) {
    loader->err = FILE_ERR;
  }
  return ctx.point_idx;
}

static size_t
parse_buf(DatLoader *loader, char *buf, size_t len, CsvCtx *ctx) {
  struct csv_parser *parser = loader->csv_prs;
  void *vctx = (void *)ctx;
  int i;

  for (i = 0; i < len && !is_ctx_full(ctx); i++) {
    if (csv_parse(parser, buf + i, 1, csv_eofd, csv_eor, vctx) < 1) {
      /* Error has occured */
      loader->err = CSV_ERR;
      break;
    }
  }
  return i;
}

DatLoader *
make_data_loader(DLConf *dl_conf) {
  size_t dl_sz = sizeof(DatLoader);
  DatLoader *dat_loader = (DatLoader *)safe_malloc(dl_sz);

  size_t csv_prs_sz = sizeof(struct csv_parser);
  struct csv_parser *csv_prs = (struct csv_parser *)safe_malloc(csv_prs_sz);

  FILE *fp;

  if (csv_init(csv_prs, CSV_APPEND_NULL)) {
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

void
destroy_dat_loader(DatLoader *dat_loader) {
  if (dat_loader->fp) fclose(dat_loader->fp);
  csv_fini(dat_loader->csv_prs, csv_eofd, csv_eor, NULL);
  csv_free(dat_loader->csv_prs);
  safe_free((void **)&dat_loader->csv_prs);
}

int
ld_err(DatLoader *loader) {
  return loader->err;
}

static void
init_csv_ctx(CsvCtx *ctx, Point *points, int len, DatLoader *loader) {
  ctx->begin_row = loader->dl_conf.has_header;
  ctx->field_idx = 0;
  ctx->row_idx = 0;
  ctx->point_idx = 0;
  ctx->points_len = len;
  ctx->points = points;
  ctx->loader = loader;
}

static void
csv_eofd(void *dat, size_t len, void *custom) {
  CsvCtx *ctx = (CsvCtx *)custom;
  char *str = (char *)dat;
  char *endStr;
  double value = strtod(str, &endStr);
  DLConf *loader_conf = &ctx->loader->dl_conf;
  int i;

  if (is_ctx_full(ctx)) {
    rp_err("csv_eofd called when ctx is full");
    /* Just return and run as normal, but this shouldn't happen */
    return;
  }
  if (ctx->field_idx == loader_conf->y_col) {
    ctx->points[ctx->point_idx].y = value;
  }
  for (i = 0; i < loader_conf->x_dim; ++i) {
    if (ctx->field_idx == loader_conf->x_cols[i]) {
      ctx->points[ctx->point_idx].x[i] = value;
    }
  }
  ctx->field_idx++;
}

static void
csv_eor(int c, void *custom) {
  CsvCtx *ctx = (CsvCtx *)custom;
  ctx->field_idx = 0;
  ctx->row_idx++;
  ctx->point_idx++;
  return;
}
