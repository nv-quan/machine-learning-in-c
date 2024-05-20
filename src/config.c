#include "config.h"

Config get_config() {
  Config config;
  config.algorithm = STOCHASTIC_GRADIENT_DESCENT;
  return config;
}
