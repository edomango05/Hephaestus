CC = gcc
CFLAGS = -Wall -Wextra -O2
SRC_DIR = src
OBJ_DIR = build
BIN_DIR = bin

TARGET = $(BIN_DIR)/Hephaestus
SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRCS))

UNAME_S := $(shell uname -s)

ifeq ($(UNAME_S), Darwin)
    BREW_PREFIX := $(shell brew --prefix)
    CFLAGS += -I$(BREW_PREFIX)/include
    LDFLAGS = -L$(BREW_PREFIX)/lib -lglew -lglfw -framework OpenGL -framework GLUT -lm
else
    LDFLAGS = -lGL -lGLU -lglut -lGLEW -lglfw -lm
endif

all: directories $(TARGET)

directories:
	@mkdir -p $(OBJ_DIR)
	@mkdir -p $(BIN_DIR)

$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $@ $(LDFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)

run: all
	./$(TARGET)

.PHONY: all directories clean runo