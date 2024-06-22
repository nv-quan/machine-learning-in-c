#include "utils.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

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
  char prefix[MAX_NAME_LEN];
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

void
conf_tree_set(ConfTree *t, char *n, char *v, size_t s, ConfTree *c) {
  t->child_count = s;
  t->name = n;
  t->value = v;
  t->children = c;
}

ConfTree *
conf_tree_init(ConfTree *tree, const char *lit_name, const char *lit_value) {
  Str *tree_name, *tree_val;

  if (tree == NULL) return NULL;
  tree_name = str_creat_from_literal(lit_name);
  if (tree_name == NULL) return NULL;
  tree_val = str_creat_from_literal(lit_value);
  if (tree_val == NULL) {
    str_destr(tree_name);
  }
  tree->name = tree_name;
  tree->value = tree_value;
  tree->children = NULL;
  tree->children_capacity = 0;
  tree->child_count = 0;
  return tree;
}

ConfTree *
conf_tree_add(ConfTree *tree, char *name, char *val, ConfTree *children) {
  ConfTree *res = tree->children + tree->child_count++;
  conf_tree_set(res, name, val, 0, children);
  return res;
}

ConfTree *
conf_tree_add_child(ConfTree *parent, const char *name, size_t name_size) {
  ConfTree *temp, *child;
  size_t new_cap;
  if (parent == NULL) return NULL;
  if (parent->children_capacity > 0 && parent->children == NULL) {
    return NULL;
  }
  if (parent->children_capacity == 0) {
    if (parent->child_count != 0) return NULL;
    parent->children = conf_tree_creat(name, name_size);
    if (!parent->children) return NULL;
    parent->child_count++;
    parent->children_capacity++;
    return child;
  }
  if (parent->child_count > parent->children_capacity) return NULL;
  if (parent->child_count == parent->children_capacity) {
    new_cap = parent->children_capacity * 2;
    temp = (ConfTree *)realloc(parent->children, sizeof(ConfTree) * new_cap);
    if (!temp) return NULL;
    parent->children = temp;
  }
  child = parent->children + parent->child_count++;
}

void
conf_tree_destr(ConfTree *tree) {
  size_t i;

  if (tree == NULL) return;
  for (i = 0; i < tree->child_count && tree->children; ++i) {
    conf_tree_destr(tree->children + i);
  }
  if (tree->children) free(tree->children);
  safe_free((void **)&tree->name);
  safe_free((void **)&tree->value);
  safe_free((void **)&tree);
}

ConfTree *
conf_tree_creat(const char *name, size_t name_size) {
  ConfTree *res;
  char *res_name;

  if (name == NULL) {
    rp_err("conf_tree_creat: can't create conf tree with NULL name");
    return NULL;
  }
  res_name = (char *)malloc(name_size);
  if (!res_name) return NULL;
  if (!(res = (ConfTree *)malloc(sizeof(ConfTree)))) {
    free(res_name);
    return NULL;
  }
  res->name = res_name;
  res->value = NULL;
  res->children = NULL;
  res->child_count = 0;
  res->children_capacity = 0;
  return res;
}

ConfTree *
scan_conf_tree(FILE *fp) {
  char line[MAX_NAME_LEN];
  size_t i, level;
  ConfTree *root = ConfTree * parent;
  /* Read until end-of-file is reached */
  level = 0;
  parent = root;
  while (fgets(line, MAX_NAME_LEN, fp)) {
    while (line[i] == ' ') i++;
    if (i % 2 != 0) {
      rp_err("scan_conf_tree: Wrong number of space prefix");
      return NULL;
    }
    if (level == i / 2) {
    }
  }
}
