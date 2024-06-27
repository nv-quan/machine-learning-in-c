#include "utils.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "data.h"

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

size_t
s_strcpy(char *dst, const char *src, size_t dstsize, int *is_trunc) {
  size_t src_len, res;
  if (dst == NULL || src == NULL) return 0;
  src_len = strnlen(src, dstsize);
  if (src_len == dstsize) {
    *is_trunc = 1;
  } else {
    *is_trunc = 0;
  }
  res = src_len < (dstsize - 1) ? src_len : (dstsize - 1);
  memcpy(dst, src, res);
  dst[res] = '\0';
  return res;
}

/* TODO: Write rp_err version that accepts format */
void
rp_err(char *msg) {
  fprintf(stderr, "[ERROR] %s\n", msg);
}

void
rp_warn(char *msg) {
  fprintf(stderr, "[WARNING] %s\n", msg);
}

int
double_eq(double a, double b) {
  double epsilon = 5e-6;
  return fabs(a - b) < epsilon;
}
