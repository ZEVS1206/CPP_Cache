CMAKE = cmake
BUILD_DIR = build
EXEC_DIR = $(BUILD_DIR)/my_program

test ?= test_1.txt

.PHONY: all build clean run
.SILENT:
all: build

build:
	mkdir -p $(BUILD_DIR)
	$(CMAKE) -S . -B $(BUILD_DIR) > /dev/null
	$(CMAKE) --build $(BUILD_DIR) -- -s

clean:
	rm -rf $(BUILD_DIR)

run: build
	./$(EXEC_DIR) $(test)
