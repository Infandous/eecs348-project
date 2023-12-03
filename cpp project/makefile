CC = g++
CFLAGS = -std=c++11 -g
SRC_DIR = .
BUILD_DIR = .

# List your source files here
SRCS = $(wildcard $(SRC_DIR)/*.cpp)

# Derive object file names from source file names
OBJS = $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(SRCS))

# The target binary
TARGET = calculator

# Rule to build the target
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(BUILD_DIR)/$(TARGET) $(OBJS)

# Rule to build object files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CC) $(CFLAGS) -c -o $@ $<

# Clean rule
clean:
	rm -f $(BUILD_DIR)/$(TARGET) $(OBJS)
