CC = gcc
CFLAGS = -Wall -Wextra -O2
INCLUDES = -I.
EXAMPLES = fib spsc mpsc sched timer prime
BUILD_DIR = build

all: $(EXAMPLES)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

%: examples/%.c co.h | $(BUILD_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) -o $(BUILD_DIR)/$@ $<

.PHONY: clean
clean:
	rm -rf $(BUILD_DIR)
