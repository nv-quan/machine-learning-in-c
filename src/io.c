#include "io.h"

#include <assert.h>
#include <csv.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utils.h"

typedef struct csv_context {
  int begin_row; /* Row index where data start, normally 1 if csv has header
                    and 0 if not */
  int field_idx; /* CSV field idx in a row */
  int row_idx;   /* CSV row idx, including the header */
  int point_idx; /* Current point index */
  Point *points; /* Point to an array of points */
  PointAug point_aug; /* Augmented data for current point */
  int points_len;     /* Length of points array */
  DatLoader *loader;
} CsvCtx;

/* Callback for each end of field */
static void csv_eofld(void *dat, size_t len, void *custom);

/* Callback for each end of row */
static void csv_eor(int c, void *custom);

/* Parse buffer into csv until either the whole buffer is parsed or maximum
 * number of points are saved into ctx. Return number of bytes parsed. Return
 * can be smaller than len if ctx is full.
 */
static size_t parse_buf(DatLoader *loader, char *buf, size_t len, CsvCtx *ctx);

static void init_csv_ctx(CsvCtx *ctx, Point *buf, int len, DatLoader *loader);
static int is_point_full(const PointAug *data);
static int is_point_empty(const PointAug *data);
static void reset_point_aug(PointAug *data);

static int
is_ctx_full(CsvCtx *context) {
  return context->point_idx >= context->points_len;
}

/*
 * static void
 * init_first_point(DatLoader *loader, Point *point) {
 *   int i;
 *
 *   if (loader->unproc_dat_aug.y_filled) {
 *     point->y = loader->unproc_dat.y;
 *   }
 *   for (i = 0; i < loader->unproc_dat.x_length; ++i) {
 *     if (loader->unproc_dat_aug.x_filled[i]) {
 *       point->x[i] = loader->unproc_dat.x[i];
 *     }
 *   }
 * }
 */

static size_t
load_data_file(DatLoader *loader, size_t nsize, Point *points) {
  size_t sz_read, sz_parse, total_sz_read = 0; /* bytes count */
  CsvCtx ctx;
  char buffer[BUFFER_SIZE];
  FILE *fp = loader->fp;

  if (nsize == 0) return 0;
  /* init_first_point(loader, points); No need to do this, last unprocessed
   * point will never appear here if the logic is correct */

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
  if (!is_ctx_full(&ctx) && !sz_read && ferror(fp)) {
    rp_err("load_data_file: Error while reading file.");
    loader->err = FILE_ERR;
  } else if (!is_ctx_full(&ctx) && !sz_read && is_point_full(&ctx.point_aug)) {
    rp_err("load_data_file: Warning, file is not completed.");
    /* Allows program to continue */
    reset_point_aug(&ctx.point_aug);
  }
  /* At this point, point_aug must be empty because either ctx is full, in which
   * parse_buf already guarantee point_aug to be empty, or not, which makes
   * sz_read to be 0 and makes point_aug empty. */
  assert(is_point_empty(&ctx.point_aug));
  /* loader->unproc_dat_aug = ctx.point_aug; */
  return ctx.point_idx;
}

/* TODO: Handle buffer ends before all fields are read */
/* TODO: Handle unproc_dat */
size_t
load_data_mem(DatLoader *loader, size_t nsize, Point *points) {
  size_t sz_read, mem_size, mem_idx;
  CsvCtx ctx;
  DLConf *conf;
  char *mem;

  if (nsize == 0) return 0;

  conf = &loader->dl_conf;
  mem_idx = loader->mem_idx;
  mem_size = conf->mem_size;
  mem = (char *)conf->mem;
  if (mem_idx >= mem_size) {
    loader->err = MEM_ERR;
    return 0;
  }
  init_csv_ctx(&ctx, points, nsize, loader);
  sz_read = parse_buf(loader, mem + mem_idx, mem_size - mem_idx, &ctx);
  loader->mem_idx += sz_read;
  if (!is_ctx_full(&ctx) && sz_read == 0) {
    rp_err("load_data_file: Warning, file is not completed.");
    reset_point_aug(&ctx.point_aug);
  }
  assert(is_point_empty(&ctx.point_aug));
  return ctx.point_idx;
}

size_t
load_data(DatLoader *loader, size_t nsize, Point *points) {
  size_t (*caller)(DatLoader *, size_t, Point *);
  caller = (loader->dl_conf.is_mem ? load_data_mem : load_data_file);
  return caller(loader, nsize, points);
}

static size_t
parse_buf(DatLoader *loader, char *buf, size_t len, CsvCtx *ctx) {
  struct csv_parser *parser = loader->csv_prs;
  void *vctx = (void *)ctx;
  int i;

  for (i = 0; i < len && !is_ctx_full(ctx); i++) {
    if (csv_parse(parser, buf + i, 1, csv_eofld, csv_eor, vctx) < 1) {
      /* Error has occured */
      loader->err = CSV_ERR;
      break;
    }
  }
  return i;
}

/* TODO: Handle unproc_dat */
DatLoader *
make_data_loader(DLConf *conf) {
  size_t parser_sz;
  struct csv_parser *parser;
  DatLoader *loader;

  loader = (DatLoader *)safe_malloc(sizeof(DatLoader));
  parser_sz = sizeof(struct csv_parser);
  parser = (struct csv_parser *)safe_malloc(parser_sz);
  if (csv_init(parser, CSV_APPEND_NULL)) {
    rp_err("make_data_loader: Can't init csv");
    goto failed;
  }

  if (conf->is_mem) {
    loader->mem_idx = 0;
    loader->fp = NULL;
  } else {
    if ((loader->fp = fopen(conf->file_path, "r")) == NULL) {
      fprintf(stderr, "make_data_loader: Can't open file %s\n",
              conf->file_path);
      goto failed;
    }
    loader->mem_idx = 0;
  }
  loader->csv_prs = parser;
  loader->err = NOERR;
  loader->dl_conf = *conf;
  return loader;

failed:
  safe_free((void **)&parser);
  safe_free((void **)&loader);
  return NULL;
}

/* TODO: Handle unproc_dat */
void
destroy_dat_loader(DatLoader *dat_loader) {
  if (!dat_loader) return; /* Nothing to destroy */
  if (!dat_loader->dl_conf.is_mem && dat_loader->fp) {
    fclose(dat_loader->fp);
  }
  csv_fini(dat_loader->csv_prs, csv_eofld, csv_eor, NULL);
  csv_free(dat_loader->csv_prs);
  safe_free((void **)&dat_loader->csv_prs);
  safe_free((void **)&dat_loader);
}

int
ld_err(DatLoader *loader) {
  return loader->err;
}

static void
init_csv_ctx(CsvCtx *ctx, Point *points, int len, DatLoader *loader) {
  int i;

  ctx->begin_row = loader->dl_conf.has_header;
  ctx->field_idx = 0;
  ctx->row_idx = 0;
  ctx->point_idx = 0;
  ctx->points_len = len;
  ctx->points = points;
  ctx->loader = loader;
  ctx->point_aug.x_length = loader->dl_conf.x_dim;
  reset_point_aug(&ctx->point_aug);
  /* ctx->point_aug = loader->unproc_dat_aug; */
}

static void
csv_eofld(void *dat, size_t len, void *custom) {
  CsvCtx *ctx;
  double value;
  DLConf *loader_conf;
  int cur_field_idx;
  int i;

  ctx = (CsvCtx *)custom;
  cur_field_idx = ctx->field_idx++;

  if (is_ctx_full(ctx)) {
    rp_err("csv_eofld called when ctx is full");
    /* Just return and run as normal, but this shouldn't happen */
    return;
  }
  if (ctx->row_idx < ctx->begin_row) return;

  loader_conf = &ctx->loader->dl_conf;
  /* TODO: Do strtod error handling */
  value = strtod((char *)dat, NULL);

  if (cur_field_idx == loader_conf->y_col) {
    if (ctx->point_aug.y_filled) {
      rp_err("csv_eofld: called on y but y is already filled.");
      return;
    }
    ctx->points[ctx->point_idx].y = value;
    ctx->point_aug.y_filled = 1;
    return;
  }
  for (i = 0; i < loader_conf->x_dim; ++i) {
    if (cur_field_idx != loader_conf->x_cols[i]) continue;
    if (ctx->point_aug.x_filled[i]) {
      rp_err("csv_eofld: called on x but it is already filled.");
      return;
    }
    ctx->points[ctx->point_idx].x[i] = value;
    ctx->point_aug.x_filled[i] = 1;
    break;
  }
}

static int
is_point_full(const PointAug *data) {
  int i, result;

  result = data->y_filled;
  for (i = 0; i < data->x_length; ++i) {
    result = result && data->x_filled[i];
  }
  return result;
}

static int
is_point_empty(const PointAug *data) {
  int i, result;

  result = !data->y_filled;
  for (i = 0; i < data->x_length; ++i) {
    result = result && !data->x_filled[i];
  }
  return result;
}

static void
reset_point_aug(PointAug *data) {
  int i;

  data->y_filled = 0;
  for (i = 0; i < data->x_length; ++i) {
    data->x_filled[i] = 0;
  }
}

static void
csv_eor(int c, void *custom) {
  CsvCtx *ctx = (CsvCtx *)custom;
  ctx->field_idx = 0;
  ctx->row_idx++;
  /* Most of the time, point will be full. But there are cases where this is not
   * true:
   * 1. After parsing header row
   * 2. The read buffer ends before all the fields are read. This is handled in
   * load_data
   */
  if (!is_point_full(&ctx->point_aug)) return;
  /* x_length must be set to complete filling point data */
  ctx->points[ctx->point_idx++].x_length = ctx->loader->dl_conf.x_dim;
  reset_point_aug(&ctx->point_aug);
}
