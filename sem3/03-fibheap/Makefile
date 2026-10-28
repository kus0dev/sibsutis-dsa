ROOT = ../..
CC = gcc
CFLAGS = -O0 -Wall -Wextra -Iinclude -I$(ROOT)/libs/utils/include -MMD
LDFLAGS =
LDLIBS = -lm

BUILD_DIR = build
BIN_DIR = $(BUILD_DIR)/bin
OBJ_DIR = $(BUILD_DIR)/obj

# Целевой исполняемый файл
TARGET = $(BIN_DIR)/fibheap

# Категории объектных файлов
MAIN_OBJ_DIR = $(OBJ_DIR)/main
SRC_OBJ_DIR = $(OBJ_DIR)/src
EXT_OBJ_DIR = $(OBJ_DIR)/external

# Исходные файлы
MAIN_SRC = src/main.c
LOCAL_SRCS = $(filter-out $(MAIN_SRC), $(wildcard src/*.c))
UTILS_SRC = $(ROOT)/libs/utils/src/utils.c

# Объектные файлы
MAIN_OBJ = $(MAIN_OBJ_DIR)/main.o
LOCAL_OBJS = $(addprefix $(SRC_OBJ_DIR)/, $(notdir $(LOCAL_SRCS:.c=.o)))
UTILS_OBJ = $(EXT_OBJ_DIR)/utils.o

# Все объектные файлы и зависимости
ALL_OBJS = $(MAIN_OBJ) $(LOCAL_OBJS) $(UTILS_OBJ)
DEPS = $(ALL_OBJS:.o=.d)

# Цель по умолчанию
all: $(TARGET)

# Линковка
$(TARGET): $(ALL_OBJS) | $(BIN_DIR)
	$(CC) $(LDFLAGS) -o $@ $^ $(LDLIBS)

# Компиляция main.c
$(MAIN_OBJ): $(MAIN_SRC)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# Компиляция остальных локальных .c файлов
$(SRC_OBJ_DIR)/%.o: src/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# Компиляция внешней библиотеки utils.c
$(UTILS_OBJ): $(UTILS_SRC)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# Правило для создания папки bin
$(BIN_DIR):
	mkdir -p $@

# Подключаем файлы зависимостей
-include $(DEPS)

# Очистка
clean:
	rm -rf $(BUILD_DIR)

.PHONY: all clean
