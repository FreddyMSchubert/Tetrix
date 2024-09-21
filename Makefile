EXECUTABLE = tetrix
CC = clang++

CFLAGS = -I/opt/homebrew/Cellar/raylib/5.0/include -std=c++11 -Wall -Wextra -Werror -Wshadow -g -fsanitize=address
LDFLAGS = -L/opt/homebrew/Cellar/raylib/5.0/lib -lraylib -framework IOKit -framework Cocoa -framework OpenGL

SRC_DIR = ./src
OBJ_DIR = ./obj
SOURCES = $(shell find $(SRC_DIR) -name '*.cpp')
OBJECTS = $(SOURCES:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR):
	mkdir -p $@

run: all
	./$(EXECUTABLE)

clean:
	rm -rf $(OBJ_DIR)
fclean: clean
	rm -f $(EXECUTABLE)

re: clean all

.PHONY: all run clean fclean re
