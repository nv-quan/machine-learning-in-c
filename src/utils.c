#include "utils.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "data.h"

void *
safe_malloc(size_t size) {
  void *p = malloc(size);
  if (p == NULL) {
    fprintf(stderr, "Fatal: failed to allocate %zu bytes.\n", size);
    exit(-1);
  }
  return p;
}

void
safe_free(void **p) {
  if (!(*p)) return;
  free(*p);
  *p = NULL;
}

size_t
s_strcpy(char *dst, const char *src, size_t dstsize, int *is_trunc) {
  size_t src_len, res;
  if (dst == NULL || src == NULL) return 0;
  src_len = strnlen(src, dstsize);
  if (src_len == dstsize) {
    *is_trunc = 1;
  } else {
    *is_trunc = 0;
  }
  res = src_len < (dstsize - 1) ? src_len : (dstsize - 1);
  memcpy(dst, src, res);
  dst[res] = '\0';
  return res;
}

/* TODO: Write rp_err version that accepts format */
void
rp_err(char *msg) {
  fprintf(stderr, "[ERROR] %s\n", msg);
}

void
rp_warn(char *msg) {
  fprintf(stderr, "[WARNING] %s\n", msg);
}

int
double_eq(double a, double b) {
  double epsilon = 5e-6;
  return fabs(a - b) < epsilon;
}

size_t
conf_tree_get_node_count(ConfTree *tree) {
  size_t i, n;

  n = 1;
  for (i = 0; i < tree->child_count; ++i) {
    n += conf_tree_get_node_count(tree->children + i);
  }
  return n;
}

void
print_conf_tree(FILE *fp, ConfTree *tree, size_t level) {
  char prefix[SHORT_STR_LEN];
  size_t i;

  for (i = 0; i < level; ++i) {
    prefix[2 * i] = ' ';
    prefix[2 * i + 1] = ' ';
  }
  prefix[2 * i] = '\0';
  fprintf(fp, "%s%s %s\n", prefix, tree->name, tree->value);
  for (i = 0; i < tree->child_count; ++i) {
    print_conf_tree(fp, tree->children + i, level + 1);
  }
}

/*
void
conf_tree_set(ConfTree *t, char *n, char *v, size_t s, ConfTree *c) {
  t->child_count = s;
  t->name = n;
  t->value = v;
  t->children = c;
}
*/

int
conf_tree_init(ConfTree *tree, const char *name, const char *value) {
  if (tree == NULL || name == NULL) return -1;
  int is_trunc;
  s_strcpy(tree->name, name, sizeof(tree->name), &is_trunc);
  if (is_trunc) return -1;
  if (value == NULL) {
    tree->value[0] = '\0';
  } else {
    s_strcpy(tree->value, value, sizeof(tree->value), &is_trunc);
    if (is_trunc) rp_warn("conf_tree_init: value is truncated");
  }
  tree->children = NULL;
  tree->children_capacity = 0;
  tree->child_count = 0;
  return 0;
}

void
conf_tree_free(ConfTree *tree) {
  size_t i;

  if (tree == NULL) return;
  for (i = 0; i < tree->child_count && tree->children; ++i) {
    conf_tree_free(tree->children + i);
  }
  if (tree->children) free(tree->children);
}

/*
ConfTree *
conf_tree_add(ConfTree *tree, char *name, char *val, ConfTree *children) {
  ConfTree *res = tree->children + tree->child_count++;
  conf_tree_set(res, name, val, 0, children);
  return res;
}
*/

ConfTree *
conf_tree_add(ConfTree *parent, const char *name, const char *val) {
  ConfTree *child, *temp;
  size_t new_cap;
  if (parent == NULL) return NULL;
  if (parent->child_count > parent->children_capacity) return NULL;
  if (parent->children_capacity > 0 && parent->children == NULL) return NULL;
  if (parent->children_capacity == 0) {
    child = (ConfTree *)malloc(sizeof(ConfTree));
    if (child == NULL) return NULL;
    if (conf_tree_init(child, name, val) != 0) goto failed1;
    if (parent->children != NULL) goto failed2;
    parent->children = child;
    parent->child_count++;
    parent->children_capacity++;
    return child;
  failed2:
    conf_tree_free(child);
  failed1:
    free(child);
    return NULL;
  }
  if (parent->child_count == parent->children_capacity) {
    if (parent->children_capacity > SIZE_MAX / (size_t)2) return NULL;
    new_cap = parent->children_capacity * 2;
    temp = (ConfTree *)realloc(parent->children, sizeof(ConfTree) * new_cap);
    if (temp == NULL) return NULL;
    parent->children = temp;
    parent->children_capacity = new_cap;
  }
  /* parent->child_count < parent->children_capacity */
  if (conf_tree_init(parent->children + parent->child_count, name, val) != 0) {
    return NULL;
  }
  return parent->children + parent->child_count++;
}

/*
void
conf_tree_destr(ConfTree *tree) {
  size_t i;

  if (tree == NULL) return;
  for (i = 0; i < tree->child_count && tree->children; ++i) {
    conf_tree_destr(tree->children + i);
  }
  safe_free((void **)&tree->name);
  safe_free((void **)&tree->value);
  safe_free((void **)&tree);
}
*/

/*
ConfTree *
conf_tree_creat(const char *name, const char *val) {
  ConfTree *res;
  Str *tree_name, *tree_val;

  if (name == NULL) return NULL;
  tree_name = str_creat_from_char_arr(name);
  if (tree_name == NULL) return NULL;
  tree_val = NULL;
  res = NULL;
  if (value) {
    tree_val = str_creat_from_char_arr(value);
    if (tree_val == NULL) goto cleanup1;
  }
  res = (ConfTree *)malloc(sizeof(ConfTree));
  if (res == NULL) goto cleanup2;
  res->children = NULL;
  res->children_capacity = 0;
  res->child_count = 0;
  res->name = tree_name;
  res->value = tree_val;
  return res;
cleanup2:
  if (tree_val) str_destr(tree_val);
cleanup1:
  str_destr(tree_name);
  return res;
}
*/

ConfTree *
scan_conf_tree(FILE *fp) {
  char line[SHORT_STR_LEN];
  size_t i, level;
  ConfTree root, *parent;
  /* Read until end-of-file is reached */
  level = 0;
  parent = &root;
  i = 0;
  while (fgets(line, SHORT_STR_LEN, fp)) {
    while (line[i] == ' ') i++;
    if (i % 2 != 0) {
      rp_err("scan_conf_tree: Wrong number of space prefix");
      return NULL;
    }
    if (level == i / 2) {
    }
  }
}
