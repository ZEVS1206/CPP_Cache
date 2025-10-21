CMAKE = cmake
BUILD_DIR = build
INCLUDE = -I include
SOURCES = source/2q_main.cpp
EXEC_DIR_2Q = build/cache_2q
EXEC_DIR_IDEAL = build/ideal_cache

PROFILE_CFLAGS = -ggdb3 -std=c++17 -O0 -Wall -Wextra -fno-omit-frame-pointer
PROFILE_CFLAGS += -march=native -fno-pie
PROFILER_OUT_NAME = callgrind.out

file ?=

.PHONY: all build clean run
.SILENT:
all: build

build:
	mkdir -p $(BUILD_DIR)
	$(CMAKE) -S . -B $(BUILD_DIR) > /dev/null
	$(CMAKE) --build $(BUILD_DIR) -- -s

clean:
	rm -rf $(BUILD_DIR)

run_2q: build
	@if [ -n "$(file)" ]; then \
		./$(EXEC_DIR_2Q) $(file); \
	else \
		./$(EXEC_DIR_2Q); \
	fi

run_ideal: build
	@if [ -n "$(file)" ]; then \
		./$(EXEC_DIR_IDEAL) $(file); \
	else \
		./$(EXEC_DIR_IDEAL); \
	fi


profile_build:
	g++ $(PROFILE_CFLAGS) $(INCLUDE) $(SOURCES) -o $(EXEC_DIR_2Q)_profile -no-pie

callgrind: profile_build
	@if [ -n "$(file)" ]; then \
		/usr/bin/time -v valgrind --tool=callgrind --callgrind-out-file=$(PROFILER_OUT_NAME) ./$(EXEC_DIR_2Q)_profile $(file); \
	else \
		 /usr/bin/time -v valgrind --tool=callgrind --callgrind-out-file=$(PROFILER_OUT_NAME) ./$(EXEC_DIR_2Q)_profile; \
	fi
	@echo "Callgrind data saved to $(PROFILER_OUT_NAME). Use: kcachegrind $(PROFILER_OUT_NAME)"