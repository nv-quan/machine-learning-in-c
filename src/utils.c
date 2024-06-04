#include "utils.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

void *
safe_malloc(size_t size) {
  void *p = malloc(size);
  if (p == NULL) {
    fprintf(stderr, "Fatal: failed to allocate %zu bytes.\n", size);
    exit(-1);
  }
  return p;
}

void
safe_free(void **p) {
  if (!(*p)) return;
  free(*p);
  *p = NULL;
}

void
rp_err(char *msg) {
  fprintf(stderr, "%s\n", msg);
}

int
double_eq(double a, double b) {
  double epsilon = 5e-6;
  return fabs(a - b) < epsilon;
}
