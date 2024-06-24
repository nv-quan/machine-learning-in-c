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

/* Config tree.
 *
 * Useful for writing/reading configuration to and from files.
 */
typedef struct conf_tree {
  struct conf_tree *children;
  size_t children_capacity;
  size_t child_count;
  char name[SHORT_STR_LEN];
  char value[LONG_STR_LEN];
} ConfTree;

/* Initialize a ConfTree
 *
 * Parameters:
 * - tree: the ConfTree needs to be initialized
 * - name: a null-terminated string used for the ConfTree name. This must not be
 *   null.
 * - val: a null-terminated string used for the ConfTree value. This can be
 * null.
 *
 * Return:
 * - 0 on success.
 * - non-zero on failure.
 */
int conf_tree_init(ConfTree *tree, const char *name, const char *val);

/* Free the internal resources of a ConfTree.
 *
 * This does not free the tree itself.
 */
void conf_tree_free(ConfTree *tree);

/* Add a child to a ConfTree.
 *
 * On failure, no changes will be made to the parent.
 *
 * Parameters:
 * - parent: the target ConfTree.
 * - name: name of the child.
 * - val: value of the child.
 *
 * Return:
 * - A pointer to that child on success
 * - NULL pointer on failure
 */
ConfTree *conf_tree_add(ConfTree *parent, const char *name, const char *val);
void print_conf_tree(FILE *fp, ConfTree *tree, size_t level);
ConfTree *scan_conf_tree(FILE *fp);

#endif /* ifndef UTILS_H */
