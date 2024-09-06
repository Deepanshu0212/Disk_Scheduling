# Compiler to use
CC = gcc

# Compiler flags
CFLAGS = -Wall -Wextra

# Directories
IMPLEMENTATION_DIR = Implementation
TEST_DIR = Test
HEADER_DIR = Header
BIN_DIR = bin

# Source files
SOURCES = $(IMPLEMENTATION_DIR)/Disk_Scheduling.c $(IMPLEMENTATION_DIR)/main.c $(TEST_DIR)/test.c

# Object files
OBJ = $(patsubst %.c,$(BIN_DIR)/%.o,$(notdir $(SOURCES)))

# Default target
all: $(BIN_DIR)/disk_scheduler

# Rule to create the executable
$(BIN_DIR)/disk_scheduler: $(OBJ) | $(BIN_DIR)
	$(CC) $(CFLAGS) -o $@ $^

# Rule to compile Disk_Scheduling.c
$(BIN_DIR)/Disk_Scheduling.o: $(IMPLEMENTATION_DIR)/Disk_Scheduling.c $(HEADER_DIR)/Disk_Scheduling.h | $(BIN_DIR)
	$(CC) $(CFLAGS) -I$(HEADER_DIR) -c $< -o $@

# Rule to compile test.c
$(BIN_DIR)/test.o: $(TEST_DIR)/test.c $(HEADER_DIR)/Disk_Scheduling.h | $(BIN_DIR)
	$(CC) $(CFLAGS) -I$(HEADER_DIR) -c $< -o $@

# Rule to compile main.c
$(BIN_DIR)/main.o: $(IMPLEMENTATION_DIR)/main.c $(HEADER_DIR)/Disk_Scheduling.h | $(BIN_DIR)
	$(CC) $(CFLAGS) -I$(HEADER_DIR) -c $< -o $@

# Create bin directory if it doesn't exist
$(BIN_DIR):
	mkdir -p $(BIN_DIR)

# Clean up built files
clean:
	rm -rf $(BIN_DIR)

# Phony targets
.PHONY: all clean