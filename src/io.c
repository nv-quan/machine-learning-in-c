#include "io.h"
#include "utils.h"
#include <csv.h>
#include <stdio.h>

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

int load_data(struct data_loader *loader, size_t size, Point *buffer) {}

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
  return 0;

failed:
  safe_free((void **)&csv_prs);
  return -1;
}

int clear_data_loader(struct data_loader *dat_loader) {
  int retval = 0;
  if (!(dat_loader->fp) || fclose(dat_loader->fp))
    retval = -1;
  csv_fini(dat_loader->csv_prs, NULL, NULL, NULL);
  csv_free(dat_loader->csv_prs);
  return retval;
}
