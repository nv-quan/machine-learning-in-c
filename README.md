# A Machine Learning library written in C

## Why
I just want to write a ML framework in C to learn about ML. This is not intended
for real-world use.

### Features
- [x] Data loader for CSV
- [x] A naive implementation of matrix operations
- [ ] Autograd feature
- [ ] Config serialize/deserialize
- [ ] Checkpoint saving
- [ ] Example algorithms
  - [x] Gradient descent for linear regression

## Project structures
- `src`:
    - `data.c` & `data.h`: data definition and data handling, such as Point,
      Matrix, Dynamic string.
    - `io.c` & `io.h`: for read/writing configs, checkpoints, etc.
    - `custom_math.c` & `custom_math.h`: for math functions like matrix
    multiplications.
    - `gradient_descent.c` & `gradient_descent.h`: an example for gradient
      descent.
    - `utils.c` & `utils.h`: utility functions.
- `tests`: unit testing, using [Check](https://github.com/libcheck/check)

## How to build
- GNU Autotools are required to generate build files

```bash
./autogen.sh
make
# Run tests
make check
```

- To remove all build files, run `./autoremove.sh`

## Misc.

### Coding conventions
- C99 compatible.
- Make a best effort to follow C89, except in cases where it makes the code harder to read (e.g., the rule for 6 significant characters for external variable names).
- Only use goto for cleaning up in error handling, as C does not support exceptions.
