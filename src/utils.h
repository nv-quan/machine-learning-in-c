#ifndef UTILS_H
#define UTILS_H
#include <stdio.h>
#include <stdlib.h>

#define FALSE 0
#define TRUE 1

#define UNUSED(x) (void)(x)

#define MAX_NAME_LEN 100

void *safe_malloc(size_t size);
void safe_free(void **pointer);
void rp_err(char *msg);
void rp_warn(char *msg);
int double_eq(double a, double b);

/* Config tree and its helper functions. Useful for writing/reading
 * configuration to and from files */

typedef struct conf_tree {
  struct conf_tree *children;
  size_t children_capacity;
  size_t child_count;
  Str *name;
  Str *value;
} ConfTree;

/* Initialize a ConfTree
 *
 * Parameters:
 * - tree: the ConfTree needs to be initialized
 * - lit_name: a literal string used for the ConfTree name
 * - lit_val: a literal string used for the ConfTree value
 *
 * lit_name and lit_val must be literal, otherwise this function is undefined
 */
void conf_tree_init(ConfTree *tree, const char *lit_name, const char *lit_val);

size_t conf_tree_get_node_count(ConfTree *tree);
void print_conf_tree(FILE *fp, ConfTree *tree, size_t level);
ConfTree *scan_conf_tree(FILE *fp, size_t level);
ConfTree *conf_tree_creat(const char *name, size_t name_size);
void conf_tree_destr(ConfTree *tree);
void conf_tree_set(ConfTree *t, char *n, char *v, size_t s, ConfTree *c);

ConfTree *conf_tree_add(ConfTree *tree, char *name, char *val,
                        ConfTree *children);
ConfTree *conf_tree_add_child(ConfTree *parent, char *name, char *val);

#endif /* ifndef UTILS_H */
