
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
all: format tags cscope $(TARGET) 

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

# Generate cscope files
cscope: cscope.files
	@cscope -b -q -k

cscope.files: $(SRC) $(HDR)
	@echo $(SRC) $(HDR) | tr ' ' '\n' > cscope.files

# Format code
format: $(SRC) $(HDR)
	@if command -v clang-format >/dev/null 2>&1; then \
		mkdir -p $(BUILDDIR); \
		if [ -z "$$(ls -A $(BUILDDIR))" ]; then \
			clang-format -i --style=file $(SRC) $(HDR); \
		else \
			for file in $(SRC) $(HDR); do \
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
