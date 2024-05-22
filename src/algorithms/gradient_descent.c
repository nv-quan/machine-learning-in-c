#include "config.h"
#include <csv.h>
#include <stdio.h>
#define BUFFER_SIZE 1024

void stochastic_gradient_descent(Config config) {
  FILE *fp;
  struct csv_parser my_csv_parser;
  size_t i;
  char buffer[BUFFER_SIZE];

  csv_init(&my_csv_parser, 0);
  fp = fopen(config.input_path, "r");
  if (fp == NULL) {
    fprintf(stderr, "Can't open input %s", config.input_path);
    return;
  }
  while ((i = fread(buffer, sizeof(char), BUFFER_SIZE, fp)) > 0) {
  }
  /* csv_parse(&my_csv_parser, fp, */
}
