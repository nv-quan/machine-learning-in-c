#include <stdio.h>
#include "data.h"
#include "io.h"
#include "config.h"

int main(void) {
    Config config = get_config();
    //Data buffer[MAX_DATA_LENGTH];
    switch (config.algorithm) {
      case STOCHASTIC_GRADIENT_DESCENT:
        break;
      case BATCH_GRADIENT_DESCENT:
        break;
      default:
        break;
    }
    printf("Hello world\n");
    //stochastic_gradient_descent();
    //display_result();
    return 0;
}
