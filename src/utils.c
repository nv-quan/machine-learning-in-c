#include <stdio.h>
#include <stdlib.h>

void *safe_malloc(size_t size) {
  void *p = malloc(size);
  if (p == NULL) {
    fprintf(stderr, "Fatal: failed to allocate %zu bytes.\n", size);
    exit(-1);
  }
  return p;
}

void safe_free(void **p) {
  free(*p);
  *p = NULL;
}
