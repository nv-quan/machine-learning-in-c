#include "io.h"

#include <assert.h>
#include <csv.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "data.h"
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

/* Finish parsing when all data in file/memory is read. This will submit all
 * data left to csv_parser. Return 0 on success and non-zero on failure */
static int finish_parsing(DatLoader *loader, CsvCtx *ctx);

static void init_csv_ctx(CsvCtx *ctx, Point *buf, int len, DatLoader *loader);
static int is_point_full(const PointAug *data, int is_one_inserted);
static int is_point_empty(const PointAug *data, int is_one_inserted);
static void reset_point_aug(PointAug *data, int is_one_inserted);

static int
is_ctx_full(CsvCtx *context) {
  return context->point_idx >= context->points_len;
}

/* TODO: Add insert 1 option */
static size_t
load_data_file(DatLoader *loader, size_t nsize, Point *points) {
  size_t sz_read, sz_parse; /* bytes count */
  CsvCtx ctx;
  char buffer[BUFFER_SIZE];
  FILE *fp = loader->fp;
  long n;
  int ferr, file_eof, is_one_isrt;

  if (nsize == 0) return 0;
  is_one_isrt = is_one_insered(&loader->dl_conf);

  /* Make the first field of the first point 1 */
  if (is_one_isrt) points[0].x[0] = 1.0f;
  /* pointers lifetime is same with load_data */
  init_csv_ctx(&ctx, points, nsize, loader);

  while (!is_ctx_full(&ctx) && (sz_read = fread(buffer, 1, BUFFER_SIZE, fp))) {
    ferr = ferror(fp);
    if ((sz_parse = parse_buf(loader, buffer, sz_read, &ctx))) {
      /* Seek fp in case sz_parse < sz_read. This is to make sure the next
       * call to load_data pick up where this finishes. When this case
       * happens, is_ctx_full also returns true on the next call so the
       * loop will be stopped. */
      if ((n = (long)sz_parse - (long)sz_read) && fseek(fp, n, SEEK_CUR) != 0) {
        loader->err = FILE_ERR;
        /* It's an undefined behaviour to call load_data after this point
         * so we don't need to care about fp position */
        break;
      }
    } else {
      loader->err = CSV_ERR;
      break;
    }

    if (feof(fp) && finish_parsing(loader, &ctx)) {
      rp_err("load_data_file: Can't finish parsing data");
      loader->err = CSV_ERR;
      break;
    }
    if (ferr) {
      loader->err = FILE_ERR;
      rp_err("load_data_file: Error while reading file.");
      break;
    }
  }
  /* Loop breaks mean ctx is full or sz_read is 0 or some error happened */

  /* This means eof is encountered but not all fields are filled */
  if (!is_ctx_full(&ctx) && !ld_err(loader) &&
      !is_point_empty(&ctx.point_aug, is_one_isrt)) {
    rp_err("load_data_file: Warning, file is not completed.");
    /* Allows program to continue */
  }
  /* At this point, point_aug must be empty because either ctx is full, in which
   * parse_buf already guarantee point_aug to be empty, or not, which makes
   * sz_read to be 0 and makes point_aug empty. */
  loader->record_loaded = ctx.row_idx;
  return ctx.point_idx;
}

/* Every call to load_data_mem will try to load data and increase mem_idx */
static size_t
load_data_mem(DatLoader *loader, size_t nsize, Point *points) {
  size_t n, mem_size, mem_idx;
  CsvCtx ctx;
  DLConf *conf;
  char *mem;
  int is_one_isrt;

  if (nsize == 0) return 0;

  conf = &loader->dl_conf;
  mem_idx = loader->mem_idx;
  mem_size = conf->mem_size;
  mem = (char *)conf->mem;

  /* All data is already read, return */
  if (mem_idx == mem_size) return 0;

  /* Invalid mem_idx */
  if (mem_idx > mem_size) {
    loader->err = MEM_ERR;
    return 0;
  }
  is_one_isrt = is_one_insered(conf);
  if (is_one_isrt) points[0].x[0] = 1.0f;
  init_csv_ctx(&ctx, points, nsize, loader);

  if (!(n = parse_buf(loader, mem + mem_idx, mem_size - mem_idx, &ctx))) {
    rp_err("load_data_mem: Can't parse data");
    loader->err = MEM_ERR;
    return 0;
  }
  loader->mem_idx += n;

  /* When all data is read, call finish_parsing. Next call to load_data will not
   * do anything. */
  if (loader->mem_idx == mem_size && finish_parsing(loader, &ctx)) {
    rp_err("load_data_mem: Can't finish parsing");
    loader->err = CSV_ERR;
    return 0;
  }

  /* Here, either ctx is full or not. If ctx is full, parse_buf guarantee
   * point_aug to be empty. If ctx is not full, that means the memory runs out
   * of data before all fields are filled */
  if (!is_point_empty(&ctx.point_aug, is_one_isrt)) {
    rp_err("load_data_mem: Data format issue");
    /* Allow continuing of program */
  }
  loader->record_loaded = ctx.row_idx;
  return ctx.point_idx;
}

size_t
load_data(DatLoader *loader, size_t nsize, Point *points) {
  size_t (*caller)(DatLoader *, size_t, Point *);
  caller = (is_mem_based(&loader->dl_conf) ? load_data_mem : load_data_file);
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

static int
finish_parsing(DatLoader *loader, CsvCtx *ctx) {
  void *vctx = (void *)ctx;
  struct csv_parser *parser = loader->csv_prs;
  if (csv_fini(parser, csv_eofld, csv_eor, ctx)) {
    return -1;
  }
  return 0;
}

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

  if (is_mem_based(conf)) {
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
  loader->record_loaded = 0;
  return loader;

failed:
  safe_free((void **)&parser);
  safe_free((void **)&loader);
  return NULL;
}

void
destroy_dat_loader(DatLoader *dat_loader) {
  if (!dat_loader) return; /* Nothing to destroy */
  if (!is_mem_based(&dat_loader->dl_conf) && dat_loader->fp) {
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

int
is_mem_based(DLConf *conf) {
  return conf->options & DL_MEM_BASED;
}

int
is_one_insered(DLConf *conf) {
  return conf->options & DL_INSERT_ONE;
}

int
has_header(DLConf *conf) {
  return conf->options & DL_HAS_HEADER;
}

static void
init_csv_ctx(CsvCtx *ctx, Point *points, int len, DatLoader *loader) {
  int i;

  ctx->begin_row = has_header(&loader->dl_conf) ? 1 : 0;
  ctx->field_idx = 0;
  ctx->row_idx = loader->record_loaded;
  ctx->point_idx = 0;
  ctx->points_len = len;
  ctx->points = points;
  ctx->loader = loader;
  ctx->point_aug.x_length = loader->dl_conf.x_dim;
  reset_point_aug(&ctx->point_aug, is_one_insered(&loader->dl_conf));
  /* ctx->point_aug = loader->unproc_dat_aug; */
}

static void
csv_eofld(void *dat, size_t len, void *custom) {
  CsvCtx *ctx;
  double value;
  DLConf *conf;
  int i, cur_field_idx;

  ctx = (CsvCtx *)custom;
  cur_field_idx = ctx->field_idx++;

  if (is_ctx_full(ctx)) {
    rp_err("csv_eofld called when ctx is full");
    /* Just return and run as normal, but this shouldn't happen */
    return;
  }
  if (ctx->row_idx < ctx->begin_row) return;

  conf = &ctx->loader->dl_conf;
  /* TODO: Do strtod error handling */
  value = strtod((char *)dat, NULL);

  if (cur_field_idx == conf->y_col) {
    if (ctx->point_aug.y_filled) {
      rp_err("csv_eofld: called on y but y is already filled.");
      return;
    }
    ctx->points[ctx->point_idx].y = value;
    ctx->point_aug.y_filled = 1;
    return;
  }
  /* Do not touch the first field, since it is always 1 when is_one_inserted is
   * true */
  for (i = is_one_insered(conf) ? 1 : 0; i < conf->x_dim; ++i) {
    if (cur_field_idx != conf->x_cols[i]) continue;
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
is_point_full(const PointAug *data, int is_one_inserted) {
  int i, result;

  result = data->y_filled;
  for (i = is_one_inserted ? 1 : 0; i < data->x_length; ++i) {
    result = result && data->x_filled[i];
  }
  return result;
}

static int
is_point_empty(const PointAug *data, int is_one_inserted) {
  int i, result;

  result = !data->y_filled;
  for (i = is_one_inserted ? 1 : 0; i < data->x_length; ++i) {
    result = result && !data->x_filled[i];
  }
  return result;
}

static void
reset_point_aug(PointAug *data, int is_one_inserted) {
  int i;

  data->y_filled = 0;
  for (i = 0; i < data->x_length; ++i) {
    data->x_filled[i] = 0;
  }
  if (is_one_inserted) {
    data->x_filled[0] = 1;
  }
}

static void
csv_eor(int c, void *custom) {
  CsvCtx *ctx = (CsvCtx *)custom;
  int is_one_isrt = is_one_insered(&ctx->loader->dl_conf);
  ctx->field_idx = 0;
  ctx->row_idx++;
  /* Most of the time, point will be full. But there are cases where this is not
   * true:
   * 1. After parsing header row
   * 2. The read buffer ends before all the fields are read. This is handled in
   * load_data
   */
  if (!is_point_full(&ctx->point_aug, is_one_isrt)) return;
  /* x_length must be set to complete filling point data */
  ctx->points[ctx->point_idx++].x_length = ctx->loader->dl_conf.x_dim;
  if (is_one_isrt) ctx->points[ctx->point_idx].x[0] = 1.0f;
  reset_point_aug(&ctx->point_aug, is_one_isrt);
}
