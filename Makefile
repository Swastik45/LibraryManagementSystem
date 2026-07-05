# Makefile for Library Management System
# Supports GCC/Clang on Linux, macOS, and WSL

# ==================== CONFIGURATION ====================

# Compiler and flags
CC ?= gcc
CFLAGS = -I include -Wall -Wextra -std=c99 -g

# Source files and build output
SRCDIR = script
INCDIR = include
BINDIR = .
EXECUTABLE = library_management_system

# Source and object files
SOURCES = $(SRCDIR)/main.c $(SRCDIR)/management.c $(SRCDIR)/issuereturn.c \
          $(SRCDIR)/search.c $(SRCDIR)/fine.c $(SRCDIR)/display.c
OBJECTS = $(SOURCES:.c=.o)

# ==================== TARGETS ====================

.PHONY: all build run clean help

# Default target
all: build

# Build the executable
build: $(EXECUTABLE)
	@echo "✓ Build complete: $(EXECUTABLE)"

# Link the executable
$(EXECUTABLE): $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^
	@echo "✓ Linked $(EXECUTABLE)"

# Compile source files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@
	@echo "✓ Compiled $<"

# Run the application
run: build
	@echo "Starting Library Management System..."
	@./$(EXECUTABLE)

# Clean build artifacts and data files
clean:
	@rm -f $(OBJECTS) $(EXECUTABLE)
	@echo "✓ Cleaned build artifacts"

# Clean everything including generated data
clean-all: clean
	@rm -f *.dat *.bin *.txt temp_*
	@echo "✓ Cleaned all generated files"

# Display help
help:
	@echo "Library Management System - Makefile"
	@echo ""
	@echo "Targets:"
	@echo "  make build      - Compile the project (default)"
	@echo "  make run        - Build and run the application"
	@echo "  make clean      - Remove build artifacts"
	@echo "  make clean-all  - Remove build artifacts and data files"
	@echo "  make help       - Show this help message"
	@echo ""
	@echo "Environment variables:"
	@echo "  CC              - C compiler (default: gcc)"
	@echo ""
	@echo "Examples:"
	@echo "  make                    # Build with default compiler"
	@echo "  CC=clang make build     # Build with clang"
	@echo "  make run                # Build and run"

