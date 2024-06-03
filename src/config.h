#ifndef CONFIG_H
#define CONFIG_H

/* Use this file to define config for the program to run */

/* Maximum size for path length character array */
#define CF_PATH_LEN 1000
/* Input csv file path */
#define CF_CSV_PATH "data/Real estate.csv"
/* Feature dimension */
#define CF_FEAT_DIM 3
/* Learning rate */
#define CF_LRATE 0.1
/* Batch size */
#define CF_BATCH_SZ 1

#define CF_FEAT_COLS \
  { 2, 3, 4 }

#define CF_OUTPUT_COL 7

#endif /* ifndef CONFIG_H */
