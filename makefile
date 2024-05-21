# Compiler
CC = gcc

# Compiler flags for C99 standard
CFLAGS = -std=c99 -I./src

# Directories
SRCDIR = src
BUILDDIR = build
TARGET = $(BUILDDIR)/ml_algos

# Source files
SRC = $(wildcard $(SRCDIR)/*.c) $(wildcard $(SRCDIR)/algorithms/*.c)
HDR = $(wildcard $(SRCDIR)/*.h) $(wildcard $(SRCDIR)/algorithms/*.h)

# Object files
OBJ = $(SRC:$(SRCDIR)/%.c=$(BUILDDIR)/%.o)

# Dependency files
DEPS = $(OBJ:.o=.d)

# Default target
all: format $(TARGET) tags

# Linking
$(TARGET): $(OBJ)
	@mkdir -p $(BUILDDIR)
	$(CC) -o $@ $^

# Compiling with automatic dependency generation
$(BUILDDIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -MMD -c -o $@ $<

# Include dependency files
-include $(DEPS)

# Generate tags
tags: $(SRC) $(HDR)
	ctags -R $(SRCDIR)

# Format code
format: $(SRC) $(HDR)
	@if command -v clang-format >/dev/null 2>&1; then \
		echo "Running clang-format..."; \
		clang-format -i $^; \
	else \
		echo "clang-format not found, skipping code formatting"; \
	fi

# Clean up
clean:
	rm -rf $(BUILDDIR) tags

# Phony targets
.PHONY: all clean tags format

