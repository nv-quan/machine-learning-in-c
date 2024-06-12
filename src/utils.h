#ifndef UTILS_H
#define UTILS_H
#include <stdlib.h>

#define FALSE 0
#define TRUE 1

#define UNUSED(x) (void)(x)

void *safe_malloc(size_t size);
void safe_free(void **pointer);
void rp_err(char *msg);
void rp_warn(char *msg);
int double_eq(double a, double b);

#endif /* ifndef UTILS_H */
