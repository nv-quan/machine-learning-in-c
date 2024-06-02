
# Compiler
CC = gcc

# Compiler flags
CFLAGS = -std=c89 -I./src
TEST_CFLAGS = -std=c89 -I./test -I./src

# Linking flags
# -lcsv: Use libcsv
LINKING_FLAGS = -lcsv

# Directories
SRCDIR = src
BUILDDIR = build
TARGET = $(BUILDDIR)/ml_algos
TEST_SRCDIR = test
TEST_TARGET = $(BUILDDIR)/test
MAIN = main

# Source files
# SRC = $(wildcard $(SRCDIR)/*.c) $(wildcard $(SRCDIR)/algorithms/*.c)
SRC = $(shell find $(SRCDIR) -name '*.c')
TEST_SRC = $(shell find $(TEST_SRCDIR) -name '*.c')
HDR = $(shell find $(SRCDIR) -name '*.h')
TEST_HDR = $(shell find $(TEST_SRCDIR) -name '*.h')
ALL_SRC = $(SRC) $(TEST_SRC)


# Object files
OBJ = $(SRC:$(SRCDIR)/%.c=$(BUILDDIR)/%.o)
TEST_OBJ = $(OBJ) $(TEST_SRC:$(TEST_SRCDIR)/%.c=$(BUILDDIR)/%.o)
TEST_OBJ :=  $(filter-out %$(MAIN).o,$(TEST_OBJ))


# Dependency files
DEPS = $(OBJ:.o=.d)
TEST_DEPS = $(TEST_OBJ:.o=.d)

# Default target
all: debug format tags cscope $(TARGET) $(TEST_TARGET)

debug:

# Linking
$(TARGET): $(OBJ)
	@mkdir -p $(BUILDDIR)
	$(CC) $^ $(LINKING_FLAGS) -o $@
$(TEST_TARGET): $(TEST_OBJ)
	@mkdir -p $(BUILDDIR)
	$(CC) $^ $(LINKING_FLAGS) -o $@

# Compiling with automatic dependency generation
$(BUILDDIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $< $(CFLAGS) -MMD -c -o $@
$(BUILDDIR)/%.o: $(TEST_SRCDIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $< $(TEST_CFLAGS) -MMD -c -o $@

# Include dependency files
-include $(DEPS)
-include $(TEST_DEPS)

# Generate tags
tags: $(SRC) $(HDR)
	@ctags -R $(SRCDIR) $(TEST_SRCDIR)

# Generate cscope files
cscope: cscope.files
	@cscope -b -q -k

cscope.files: $(SRC) $(HDR)
	@echo $(SRC) $(HDR) | tr ' ' '\n' > cscope.files

# Format code
format: $(SRC) $(HDR) $(TEST_SRC) $(TEST_HDR)
	@if command -v clang-format >/dev/null 2>&1; then \
		mkdir -p $(BUILDDIR); \
		if [ -z "$$(ls -A $(BUILDDIR))" ]; then \
			clang-format -i --style=file $(SRC) $(HDR) $(TEST_SRC) $(TEST_HDR); \
		else \
			for file in $(SRC) $(HDR) $(TEST_SRC) $(TEST_HDR); do \
				if [ "$$(find "$${file}" -newer $(BUILDDIR))" ]; then \
					clang-format -i --style=file "$${file}"; \
				fi; \
			done \
		fi \
	else \
		echo "clang-format not found, skipping code formatting"; \
	fi

# Clean up
clean:
	rm -rf $(BUILDDIR) tags cscope.out cscope.in.out cscope.po.out cscope.files

# Phony targets
.PHONY: all clean tags format cscope
