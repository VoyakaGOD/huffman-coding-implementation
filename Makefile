PROGRAM := huff.exe
BUILD_DIR := build
SRC_DIR := src
CC := gcc

FILES := $(wildcard $(SRC_DIR)/*.c)
HEADERS := $(wildcard $(SRC_DIR)/*.h)

$(BUILD_DIR)/$(PROGRAM) : $(FILES) $(HEADERS)
ifeq ($(OS),Windows_NT)
	win_mkdir $(BUILD_DIR)
else
	mkdir -p $(BUILD_DIR)
endif
	$(CC) $(FILES) -o $@

.PHONY: clean

clean:
ifeq ($(OS),Windows_NT)
	win_rm $(BUILD_DIR)
else
	rm -rf $(BUILD_DIR)
endif
