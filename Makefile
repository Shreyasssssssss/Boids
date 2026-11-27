CC := gcc
CFLAGS := -Wall -Wextra -std=c11 -Iinclude
CPPFLAGS := -MMD -MP -Icglm/include

SRC_DIR := src
OBJ_DIR := build
TARGET := app.exe

SRCS := $(SRC_DIR)/main.c $(SRC_DIR)/glad.c $(SRC_DIR)/boids.c
OBJS := $(OBJ_DIR)/main.o $(OBJ_DIR)/glad.o $(OBJ_DIR)/boids.o
DEPS := $(OBJS:.o=.d)

ifeq ($(OS), Windows_NT)
LDLIBS := -Llib -lglfw3 -lopengl32 -lgdi32 -lwinmm -luser32 -lshell32
else
LDLIBS := -lglfw -lGL -lm -ldl -lpthread
endif

.PHONY: all clean run

all: $(TARGET)

$(TARGET): $(OBJS) glfw3.dll
	$(CC) $(OBJS) -o $@ $(LDLIBS)

glfw3.dll: lib/glfw3.dll
	@copy /y lib\glfw3.dll glfw3.dll >nul

$(OBJ_DIR)/main.o: $(SRC_DIR)/main.c
	mkdir -p "$(OBJ_DIR)"
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

$(OBJ_DIR)/glad.o: $(SRC_DIR)/glad.c
	mkdir -p "$(OBJ_DIR)"
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

$(OBJ_DIR)/boids.o: $(SRC_DIR)/boids.c
	mkdir -p "$(OBJ_DIR)"
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

run: all
	.\$(TARGET)

clean:
	@if exist "$(OBJ_DIR)" rmdir /s /q "$(OBJ_DIR)"
	@if exist "$(TARGET)" del /q "$(TARGET)"
	@if exist "glfw3.dll" del /q "glfw3.dll"

-include $(DEPS)
