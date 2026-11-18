CC := gcc
CFLAGS := -Wall -Wextra -std=c11 -Iinclude
CPPFLAGS := -MMD -MP

SRC_DIR := src
OBJ_DIR := build
TARGET := app.exe

SRCS := $(SRC_DIR)/main.c $(SRC_DIR)/glad.c
OBJS := $(OBJ_DIR)/main.o $(OBJ_DIR)/glad.o
DEPS := $(OBJS:.o=.d)

ifneq ($(filter Windows_NT,$(OS))$(findstring mingw,$(shell $(CC) -dumpmachine)),)
SHELL := cmd.exe
LDLIBS := -Llib -lglfw3 -lopengl32 -lgdi32 -lwinmm -luser32 -lshell32
MKDIR := if not exist "$(OBJ_DIR)" mkdir "$(OBJ_DIR)"
COPY_DLL := copy /y "lib\glfw3.dll" "glfw3.dll" >nul
RM_DIR := if exist "$(OBJ_DIR)" rmdir /s /q "$(OBJ_DIR)"
RM_TARGET := if exist "$(TARGET)" del /q "$(TARGET)"
RM_DLL := if exist "glfw3.dll" del /q "glfw3.dll"
else
LDLIBS := -lglfw -lGL -lm -ldl -lpthread
MKDIR := mkdir -p "$(OBJ_DIR)"
COPY_DLL := cp "lib/glfw3.dll" "glfw3.dll"
RM_DIR := rm -rf "$(OBJ_DIR)"
RM_TARGET := rm -f "$(TARGET)"
RM_DLL := rm -f "glfw3.dll"
endif

.PHONY: all clean run

all: $(TARGET)

$(TARGET): $(OBJS) glfw3.dll
	$(CC) $(OBJS) -o $@ $(LDLIBS)

glfw3.dll: lib/glfw3.dll
	@$(COPY_DLL)

$(OBJ_DIR)/main.o: $(SRC_DIR)/main.c
	$(MKDIR)
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

$(OBJ_DIR)/glad.o: $(SRC_DIR)/glad.c
	$(MKDIR)
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

run: all
	.\$(TARGET)

clean:
	$(RM_DIR)
	$(RM_TARGET)
	$(RM_DLL)

-include $(DEPS)
