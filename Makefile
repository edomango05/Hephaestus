CC = gcc
CFLAGS = -Wall -Wextra -O2
LDFLAGS = -lGL -lGLU -lglut -lm

# Directories
SRC_DIR = src
OBJ_DIR = build
BIN_DIR = bin

# Files
TARGET = $(BIN_DIR)/elastic_sim
SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRCS))

# Default target
all: directories $(TARGET)

# Create necessary directories
directories:
	@mkdir -p $(OBJ_DIR)
	@mkdir -p $(BIN_DIR)

# Link the executable
$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $@ $(LDFLAGS)

# Compile source files to object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean build artifacts
clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)

# Run the simulation
run: all
	./$(TARGET)

.PHONY: all directories clean run