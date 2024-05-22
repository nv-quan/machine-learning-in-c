# Compiler
CC = gcc

# Compiler flags
CFLAGS = -std=c89 -I./src

# Linking flags
# -lcsv: Use libcsv
LINKING_FLAGS = -lcsv

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
	$(CC) $^ $(LINKING_FLAGS) -o $@

# Compiling with automatic dependency generation
$(BUILDDIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $< $(CFLAGS) -MMD -c -o $@

# Include dependency files
-include $(DEPS)

# Generate tags
tags: $(SRC) $(HDR)
	ctags -R $(SRCDIR)

# Format code
format: $(SRC) $(HDR)
	@if command -v clang-format >/dev/null 2>&1; then \
		echo "Running clang-format..."; \
		mkdir -p $(BUILDDIR); \
		if [ -z "$$(ls -A $(BUILDDIR))" ]; then \
			echo "Formatting all files (first run)..."; \
			clang-format -i $(SRC) $(HDR); \
		else \
			for file in $(SRC) $(HDR); do \
				if [ "$$(find "$${file}" -newer $(BUILDDIR))" ]; then \
					echo "Formatting $${file}"; \
					clang-format -i "$${file}"; \
				fi; \
			done \
		fi \
	else \
		echo "clang-format not found, skipping code formatting"; \
	fi

# Clean up
clean:
	rm -rf $(BUILDDIR) tags

# Phony targets
.PHONY: all clean tags format

