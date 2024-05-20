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

# Object files
OBJ = $(SRC:$(SRCDIR)/%.c=$(BUILDDIR)/%.o)

# Dependency files
DEPS = $(OBJ:.o=.d)

# Default target
all: $(TARGET)

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

# Clean up
clean:
	rm -rf $(BUILDDIR)

# Phony targets
.PHONY: all clean

