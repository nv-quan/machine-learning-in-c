#ifndef IO_H
#define IO_H

#include "data.h"
#define MAX_DATA_LENGTH 10000

// Returns data length, put data in buffer
// If there is no more data, return 0
size_t get_data(Data* buffer, size_t buffer_size);

#endif /* ifndef IO_H */
