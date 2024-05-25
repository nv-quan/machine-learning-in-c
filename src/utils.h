#ifndef UTILS_H
#define UTILS_H

#define FALSE 0
#define TRUE 1

void *safe_malloc(size_t size);
void safe_free(void **pointer);

#endif /* ifndef UTILS_H */
