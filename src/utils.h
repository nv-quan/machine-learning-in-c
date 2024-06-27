#ifndef UTILS_H
#define UTILS_H
#include <stdio.h>
#include <stdlib.h>

#include "config.h"

#define FALSE 0
#define TRUE 1

#define UNUSED(x) (void)(x)

/* Return early when X is non zero.
 *
 * Useful when do error check on functions that return pointers.
 */
#define GUARD_NZERO_NEG(X) \
  if (X) return -1

/* Return early when X is zero.
 *
 * Useful when do error check on functions that return value.
 */
#define GUARD_ZERO_NEG(X) \
  if (!(X)) return -1

void *safe_malloc(size_t size);
void safe_free(void **pointer);
void rp_err(char *msg);
void rp_warn(char *msg);
int double_eq(double a, double b);

/* Copy a maximum of dstsize bytes from src to dst.
 *
 * This function never read more than dstsize from src. The end of the array
 * is null-terminated. This mimics the functionality of strscpy in linux kernel
 * but without reporting truncation. The return value is the size of the newly
 * copied string minus 1 (for the NUL character). When the src is truncated,
 * is_trunc will be set.
 */
size_t s_strcpy(char *dst, const char *src, size_t dstsize, int *is_trunc);

#define CONF_FIELD_MAX_COUNT 100

/* TODO: Add support for json-like objects and array */
/* Serialized data field types */
enum dat_field_type { INT, SIZE_T, FLOAT, DOUBLE, CHAR, STRING };

/* Data schema */
typedef struct dat_schema {
  size_t field_count;
  enum conf_field_type types[CONF_FIELD_MAX_COUNT];
  size_t sizes[CONF_FIELD_MAX_COUNT];
  char names[CONF_FIELD_MAX_COUNT][SHORT_STR_LEN];
} SrdSchema;

enum parser_type {
  KEY,
  STRING,
  NUMBER,
  CHAR,
  DIGIT,
};

enum ast_type {
  OBJECT,
  LEFT_BRACKET,
  RIGHT_BRACKET,
  COMMA,
  MEMBER,
  QUOTE,
  COLON,
  STRING,
};

/* Abstract syntax tree node */
typedef struct ast_node {
  char *start;
  char *end;
  enum ast_type type;
  struct ast_node *children;
  size_t child_count;
} AstNode;

typedef struct parser_result {
  char *raw_data;

} PrsrResult;

typedef struct parser_conf {
  enum parser_type type;
} PrsrConf;

int parse(FILE *fp, enum parser_type type, void *target, size_t size);

/* Parse serialized data into an object */
int deserialize(FILE *fp, SrdSchema *schema, void *object);
/*{
  if (fp == NULL || schema == NULL || object == NULL) return -1;
}
*/

int serialize(FILE *fp, SrdSchema *schema, void *object);
/*{
      return 0;
    }*/

#endif /* ifndef UTILS_H */
