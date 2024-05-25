#include "io.h"
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
}
