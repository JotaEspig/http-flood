# From https://github.com/TheNetAdmin/Makefile-Templates

# tool macros
CC ?= gcc
CFLAGS := -pthread

# path macros
BIN_PATH := bin
SRC_PATH := src

# compile macros
TARGET_NAME := http-flood
ifeq ($(OS),Windows_NT)
	TARGET_NAME := $(addsuffix .exe,$(TARGET_NAME))
endif
TARGET := $(BIN_PATH)/$(TARGET_NAME)

# src files & obj files
SRC := $(shell find src/ -type f -name '*.c')

# clean files list
CLEAN_LIST := $(TARGET)

# default rule
default: makedir all

# non-phony targets
$(TARGET): $(SRC)
	$(CC) -o $@ $(SRC) $(CFLAGS)

# phony rules
.PHONY: makedir
makedir:
	@mkdir -p $(BIN_PATH)

.PHONY: all
all: $(TARGET)

.PHONY: clean
clean:
	@echo CLEAN $(CLEAN_LIST)
	@rm -f $(CLEAN_LIST)

# Print the source files and object files
print:
	echo $(SRC)
	echo $(OBJ)
