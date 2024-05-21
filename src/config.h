#ifndef CONFIG_H
#define CONFIG_H
#define MAX_PATH_LENGTH 4096

enum Algorithm { STOCHASTIC_GRADIENT_DESCENT, BATCH_GRADIENT_DESCENT };

typedef struct config_tag {
  enum Algorithm algorithm;
  char input_path[MAX_PATH_LENGTH];
} Config;

Config get_config(void);

#endif /* ifndef CONFIG_H */
