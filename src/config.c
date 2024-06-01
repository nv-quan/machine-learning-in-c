#include "config.h"

#include <string.h>

#include "io.h"

Config
get_config() {
  Config config;
  config.algorithm = STOCHASTIC_GRADIENT_DESCENT;
  strcpy(config.input_path, "./input.csv");
  return config;
}
