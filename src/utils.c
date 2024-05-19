#include <stdlib.h>
#include <stdio.h>

void* safe_malloc(size_t size) {
  void* p = malloc(size);
  if (p == NULL) {
    fprintf(stderr, "Fatal: failed to allocate %zu bytes.\n", size);
    exit(-1);
  }
  return p;
}
