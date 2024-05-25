#ifndef GRADIENT_DESCENT_H
#define GRADIENT_DESCENT_H

#include "io.h"

enum grad_desc_type { STOCHASTIC, BATCH };
enum comp_graph_type { LIN_REG /*Linear regression*/ };

/* Computational graph of cost function
 * Temporary use an enum to specify linear function.
 * TODO: Implement computational graph to handle different types of functions
 */
typedef struct computation_graph {
  enum comp_graph_type type;
} CompGraph;

/* Gradient descent config */
typedef struct grad_desc_config {
  enum grad_desc_type type;
  double learn_rate;
  struct computation_graph cost_func;
  struct data_source source;
  size_t dimension; /* Dimension of input, including the intercept */
} GDConfig;

/* Run Stochastic gradient descent algorithm
 * result is an array of double, owner is of caller
 */
void sgd(PointGetter point_getter, double *result);

/* Run gradient descent to minimize cost function and return result into
 * a double array.
 * config and result are allocated and owned by caller.
 *
 * Return 1 when success and 0 when fail.
 */
int grad_desc(struct grad_desc_config *config, double *result);

#endif /* ifndef GRADIENT_DESCENT_H */
