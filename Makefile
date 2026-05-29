// Makefile

# Makefile for building the project
# Define variables
CC = g++
CFLAGS = -Wall -Wextra -std=c++17
LDFLAGS =
SOURCES = main.cpp
OBJECTS = $(SOURCES:.cpp=.o)
EXECUTABLE = my_program	
# Default target
all: $(EXECUTABLE)
# Rule to link the executable$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) -o $@ $^
# Rule to compile source files into object files%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@
# Clean target to remove object files and executableclean:
	rm -f $(OBJECTS) $(EXECUTABLE)
# Phony targets
.PHONY: all clean

