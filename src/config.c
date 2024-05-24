#include "config.h"
#include "io.h"
#include <string.h>

Config get_config() {
  Config config;
  config.algorithm = STOCHASTIC_GRADIENT_DESCENT;
  strcpy(config.input_path, "./input.csv");
  return config;
}
