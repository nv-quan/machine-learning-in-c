#ifndef CONFIG_H
#define CONFIG_H

enum Algorithm {
  STOCHASTIC_GRADIENT_DESCENT,
  BATCH_GRADIENT_DESCENT
};

typedef struct config_tag {
  enum Algorithm algorithm;
} Config;

Config get_config(void);

#endif /* ifndef CONFIG_H */
