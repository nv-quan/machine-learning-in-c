#ifndef IO_H
#define IO_H
#include <stdlib.h>

#define MAX_DATA_LENGTH 10000
#define BUFFER_SIZE 1024

/* Function to get data
 * parameters:
 * (1) void*: points to an allocated buffer
 * (2) size_t: buffer size
 * Return size of the data
 * Return EOF when there is an error or EOF
 */
typedef int (*DataGetter)(void *, size_t);

#endif /* ifndef IO_H */
